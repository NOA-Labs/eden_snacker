/*
  NOA_WebServer.cpp - Library for Web Server
  Copyright 2021 NOA
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/
#include <Esp.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Update.h>
#include <ArduinoJson.h>

#include "..\DRV\PDM\usb_pd.h"
#include "..\APP\NOA_App.h"

#include "..\LIB\PUB\NOA_public.h"
#include "..\LIB\PUB\NOA_syspara.h"
#include "..\LIB\PUB\NOA_parameter_table.h"

#include "..\LIB\PUB\NOA_TimeDefs.h"

#include "NOA_WebPage.h"
#include "NOA_WebServer.h"

#define SIZE_OF_WEBSERVER_STACK  SIZE_OF_STACK * 4

static const char* TAG = "webserver";

TaskHandle_t NOA_WebServer_Task = NULL;
StaticTask_t xTaskBuffer_WebServer;
StackType_t xStack_WebServer[SIZE_OF_WEBSERVER_STACK];
xQueueHandle NOA_WEBSERVER_TASKQUEUE = NULL;
static int nStatus_WebServer = 0;

String _username = "admin";
String _password = "admin";

WebServer NOAServer(80);
static xSemaphoreHandle noaserver_parajson_mutex = NULL;

bool shouldreboot = false;
bool shouldresetsta = false;
extern int nreboot_delay_timer;
static uint32_t nFile_Size = 0;
static uint32_t nFile_Counts = 0;
bool bUpdate = false;

DynamicJsonDocument docPara(4096);
#define NOA_PARA_SIZE 100

void handleNotFound() {
  NOAServer.send(404, "text/plain", "404: Not found");
}

void handleResponse() {      // callback
//  NOAServer.sendHeader("Connection", "close");
//  NOAServer.sendHeader("Cache-Control", "no-cache");
//  NOAServer.sendHeader("X-Content-Type-Options", "nosniff");

  int nArgs_num = NOAServer.args();
//  ESP_LOGI(TAG, "Para Response:%d", nArgs_num);
  if (nArgs_num > 0) {
    for (int nTemp_i = 0; nTemp_i < nArgs_num; nTemp_i ++) {
//      ESP_LOGI(TAG, "Para[%d] Response:%s", nTemp_i, NOAServer.arg((nTemp_i)).c_str());
      if (nFile_Size == 0 && nTemp_i == 1) {
        nFile_Size = NOA_PUB_Swap_charNum((char *)NOAServer.arg((nTemp_i)).c_str());
//        ESP_LOGI(TAG, "nFile_Size %d", nFile_Size);
      }
    }
  } else {
    NOAServer.send(200);
    return;
  }

  NOAServer.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");

// NOAServer.sendHeader("Cache-Control", "no-cache");
//      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
//      NOAServer.send(200, PSTR("application/json;charset=utf-8"), system_para_page_file);
}

void handleFileupload() {  // callback
  if (bUpdate == false) {
    ESP_LOGE(TAG, "No in Upgrade Status!");
    return;
  }
  
  HTTPUpload &upload = NOAServer.upload();  // file upload object
  if (upload.status == UPLOAD_FILE_START) {  // Begin file upload
//    ESP_LOGI(TAG, "Begin upload file:%s %d %d %d", upload.filename.c_str(), upload.currentSize, upload.totalSize, nFile_Counts);
    if (nFile_Size == 0) {
      nFile_Counts = 0;
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;
      ESP_LOGI(TAG, "FreeSketchSpace size %d", maxSketchSpace);
      if (!Update.begin(maxSketchSpace, U_FLASH)) {//start with max available size
        ESP_LOGE(TAG, "Update begin error");
        NOA_Parametr_Set(37, (char *)"2");
      }
    }
  } else if (upload.status == UPLOAD_FILE_WRITE) {
    if (nFile_Counts == 0) {
      if (upload.currentSize > 512) {
        int nIndex = 0;
        for(int nTemp_i = 0; nTemp_i < upload.currentSize; nTemp_i++) {
           if (upload.buf[nTemp_i] == 0x00 && upload.buf[nTemp_i - 1] == 0x3d && upload.buf[nTemp_i - 2] == 0x3d) {
              nIndex = nTemp_i + 1;
              break;
           }
        }
        char strflag[32] = {0};
        memcpy(strflag, &upload.buf[nIndex + 9], 23);
        ESP_LOGI(TAG, "File flag is %s in %d of first %d data", strflag, nIndex, upload.currentSize);
        char strver[8] = {0};
        int nver = 0;
        snprintf(strver, 8, "%c%c%c%c", upload.buf[nIndex], upload.buf[nIndex + 2], upload.buf[nIndex + 4], upload.buf[nIndex + 6]);
        nver = NOA_PUB_Swap_charNum(strver);
        ESP_LOGI(TAG, "Version flag is %s in first %d data = %d", strver, upload.currentSize, nver);
#ifdef NOA_PD_SNACKER
        if (memcmp(strflag, "NOA PD SNACKER Firmware", 23) != 0) {
          ESP_LOGE(TAG, "Snacker OTA file error");
          NOA_Parametr_Set(37, (char *)"2");
          bUpdate = false;
          return;
        } else {
          if (nver <= 12) {
            ESP_LOGE(TAG, "Snacker OTA version error");
            NOA_Parametr_Set(37, (char *)"2");
            bUpdate = false;
            return;
          }
        }
#else
#endif
      }
    }
    nFile_Counts = nFile_Counts + upload.currentSize;
//    ESP_LOGI(TAG, "uploaded %d %d %d bytes of file size ", upload.currentSize, upload.totalSize, nFile_Counts);
    if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
      ESP_LOGE(TAG, "Update write error");
      NOA_Parametr_Set(37, (char *)"2");
    }
  } else if (upload.status == UPLOAD_FILE_END) {
    ESP_LOGI(TAG, "Write %d %d %d bytes to OTA partition",upload.currentSize, upload.totalSize, nFile_Counts);
    if (nFile_Counts == nFile_Size) {
      if (Update.end(true)) { //true to set the size to the current progress
        shouldreboot = true;
        ESP_LOGI(TAG, "Upgrade is complete. Device will reboot in 5 seconds!");
        NOA_Parametr_Set(37, (char *)"3");
      } else {
        ESP_LOGE(TAG, "Update end error");
        NOA_Parametr_Set(37, (char *)"2");
      }
      nFile_Size = 0;
      nFile_Counts = 0;
    } else if (nFile_Counts < nFile_Size){
//      ESP_LOGI(TAG, "Upload is finished. Counts %d is less than file size %d!", nFile_Counts, nFile_Size);
    } else {
      if (nFile_Size != 0) {
        ESP_LOGI(TAG, "Upload is finished. Counts %d is large than file size %d!", nFile_Counts, nFile_Size);
        Update.end();
        nFile_Size = 0;
        nFile_Counts = 0;
        NOA_Parametr_Set(37, (char *)"2");
      }
    }
  } else {
    ESP_LOGI(TAG, "Upload firmware fail: status=%d", upload.status);
  }
}

DynamicJsonDocument docPara_Temp(4096);
void handleResponse_para() {      // callback
  int nArgs_num = NOAServer.args();
  ESP_LOGI(TAG, "Para Response:%d", nArgs_num);
  if (nArgs_num > 0) {
    ESP_LOGI(TAG, "Para Response:%s", NOAServer.arg((nArgs_num - 1)).c_str());
  } else {
//    NOAServer.send(200, PSTR("application/json;charset=utf-8"), system_para_page_file);
    NOAServer.send(200);
    return;
  }
  deserializeJson(docPara_Temp, NOAServer.arg((nArgs_num - 1)).c_str());
  JsonObject rootPara = docPara_Temp.as<JsonObject>();
  JsonObject stringPara = rootPara["string"];
  nArgs_num = stringPara.size();
  if (nArgs_num > 0) {
    ESP_LOGI(TAG, "JsonObject string size: %d", nArgs_num);
  } else {
//    NOAServer.send(200, PSTR("application/json;charset=utf-8"), system_para_page_file);
    NOAServer.send(200);
    return;
  }

  // Loop through all the key-value pairs in obj
  String strTemp = "";
  int nTemp_i = 0;
  for (JsonPair pPara : stringPara) {
//    Serial.println(pPara.key().c_str());
//    Serial.println(pPara.value().as<const char *>());
    strTemp = pPara.key().c_str();
    nTemp_i = strTemp.toInt();
//    Serial.println(nTemp_i);
    if (nTemp_i < 0 || nTemp_i > 99) {
      ESP_LOGE(TAG, "Para %d Error!", nTemp_i);
      break;
    }
    strTemp = pPara.value().as<const char *>();
//    Serial.println(strTemp);
    NOA_Parametr_Set(nTemp_i, (char *)strTemp.c_str()); // 需要清长度
    if (nTemp_i == 36) {
      int nTemp_value = NOA_PUB_Swap_charNum((char *)strTemp.c_str());
      if (nTemp_value == 0) {
        nFile_Size = 0;
        nFile_Counts = 0;
        bUpdate = false;
        Update.end();
      } else {
        bUpdate = true;
      }
      NOA_Parametr_Set(nTemp_i + 1, (char *)strTemp.c_str());
    }
    if (nTemp_i == 4) {  // reboot
      int nTemp_value = NOA_PUB_Swap_charNum((char *)strTemp.c_str());
      if (nTemp_value >= 5) {
        nTemp_value = 5;
      }
      shouldreboot = true;
      nreboot_delay_timer = nTemp_value;
    }
    if (nTemp_i == 41 || nTemp_i == 42 || nTemp_i == 43) {  // reset STA link
      shouldresetsta = true;
    }
  }
//  for (JsonObject::iterator itPara=stringPara.begin(); itPara!= stringPara.end(); ++ itPara) {    
//    Serial.println(itPara->key().c_str()); // is a JsonString
//    Serial.println(itPara->value().as<const char *>()); // is a JsonValue
//  }
  docPara_Temp.clear();
  ESP_LOGI(TAG, "docPara_Temp is %d after Response para", !(docPara_Temp.isNull()));
  NOA_Parametr_Set(99, (char *)"1");
  NOA_UpdateParaJson();
//  char lenPara[32] = {0};
//  memset(lenPara, 0, 32);
//  snprintf(lenPara, 32, "%d", strlen(system_para_page_file));
//  NOAServer.sendHeader("Content-length:", lenPara);
  NOAServer.send(200, PSTR("application/json;charset=utf-8"), system_para_page_file);
  NOA_Parametr_Set(99, (char *)"0");
}

void handleUpload_para() {  // callback
  ESP_LOGI(TAG, "Para upload:%d", NOAServer.args());
}

void handleResponse_APlist() {      // callback
  NOA_UpdateAPListJson();
  NOAServer.sendHeader("Cache-Control", "no-cache");
  NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
  NOAServer.setContentLength(strlen(wlan_ap_list_file));
  NOAServer.send(200, PSTR("application/json;charset=utf-8"), wlan_ap_list_file);
}

//****************************************************************************
// CODE TABLES
//****************************************************************************
// NOA_WebServer_Task_Loop: Wifi Web Server task
void NOA_WebServer_Task_Loop(void * pvParameters) {
  ESP_LOGI(TAG, "NOA_WebServer_Task_Loop running on core %d", xPortGetCoreID());

  uint32_t nTemp_new = millis() / 1000;
  uint32_t nTemp_old = 0;
  char localtime[24] = {0};
  
  while(true){
    if (nStatus_WebServer == 1) {
      NOAServer.handleClient();
      nTemp_new = millis() / 1000;
      if (nTemp_new != nTemp_old) {
        memset(localtime, 0, 24);
//        sprintf(localtime, "%d", nTemp_new);
        sprintf(localtime, "%d %02d:%02d:%02d", nTemp_new/(3600 * 24), (nTemp_new/3600)%24, (nTemp_new%3600)/60, (nTemp_new%3600)%60);
        NOA_Parametr_Set(26, (char *)localtime);
        NOA_UpdateParaJson();
        nTemp_old = nTemp_new;
      }
    }
    vTaskDelay(1/portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "NOA_WebServer_Task_Loop Exit from core %d", xPortGetCoreID());
  vSemaphoreDelete(noaserver_parajson_mutex);
  vTaskDelete(NULL);
}

void NOA_ParserParaJson() {
  deserializeJson(docPara, system_para_page_file);
  JsonObject rootPara = docPara.as<JsonObject>();
//  serializeJson(rootPara, Serial);
//  Serial.println();
//  Serial.println(docPara.memoryUsage());
//  Serial.println(measureJson(docPara));
//  Serial.println(measureJsonPretty(docPara));
  Serial.println("System para root has a JsonObject string:");
  JsonObject stringPara = rootPara["string"];
  serializeJson(stringPara, Serial);
  Serial.println();
//  Serial.println(stringPara.memoryUsage());
//  Serial.println(measureJson(stringPara));
//  Serial.println(measureJsonPretty(stringPara));

  char res[8] = {0};
  int nTemp_i = 0;
  for(nTemp_i = 0; nTemp_i < NOA_PARA_SIZE; nTemp_i++) {
    memset(res, 0, 8);
    snprintf(res, 8, "%d", nTemp_i);
    serializeJson(stringPara[res], Serial);
  }
  docPara.clear();
  Serial.println();
  Serial.printf("docPara is %d\r\n", !(docPara.isNull()));
}

void NOA_GeneratorParaJson() {
  // lock
  xSemaphoreTake(noaserver_parajson_mutex, portMAX_DELAY);

  JsonObject rootPara = docPara.to<JsonObject>();
  JsonObject stringPara = rootPara.createNestedObject("string");
  char res[8] = {0};
  int nTemp_i = 0;
  char strValue[NOA_PARAM_LEN] = {0};
  for(nTemp_i = 0; nTemp_i < NOA_PARA_SIZE; nTemp_i++) {
    memset(res, 0, 8);
    memset(strValue, 0, NOA_PARAM_LEN);
    snprintf(res, 8, "%d", nTemp_i);
    NOA_Parametr_Get(nTemp_i, (char *)&strValue);
    if (strlen(strValue) > 0) {
      stringPara[res] = strValue;
    } else {
      stringPara[res] = res;
    }
  }
  char strPara[measureJson(docPara) + 1] = {0};
  serializeJson(docPara, strPara, measureJson(docPara) + 1);
//  Serial.println(strPara);
  memset(system_para_page_file, 0, 4096);
  memcpy(system_para_page_file, strPara, measureJson(docPara) + 1);
  docPara.clear();
  ESP_LOGI(TAG, "docPara is %d", !(docPara.isNull()));
  // unlock
  xSemaphoreGive(noaserver_parajson_mutex);
}

void NOA_UpdateParaJson() {
  // lock
  xSemaphoreTake(noaserver_parajson_mutex, portMAX_DELAY);
  JsonObject rootPara = docPara.to<JsonObject>();
  JsonObject stringPara = rootPara.createNestedObject("string");
  char res[8] = {0};
  int nTemp_i = 0;

  char strValue[NOA_PARAM_LEN] = {0};
  for(nTemp_i = 0; nTemp_i < NOA_PARA_SIZE; nTemp_i++) {
    memset(res, 0, 8);
    memset(strValue, 0, NOA_PARAM_LEN);
    snprintf(res, 8, "%d", nTemp_i);
    NOA_Parametr_Get(nTemp_i, (char *)&strValue);
    if (strlen(strValue) > 0) {
      stringPara[res] = strValue;
    } else {
      stringPara[res] = res;
    }
  }  
  char strPara[measureJson(docPara) + 1] = {0};
  serializeJson(docPara, strPara, measureJson(docPara) + 1);
//  Serial.println(strPara);
  memset(system_para_page_file, 0, 4096);
  memcpy(system_para_page_file, strPara, measureJson(docPara) + 1);
  docPara.clear();
  // unlock
  xSemaphoreGive(noaserver_parajson_mutex);
}

void NOA_UpdateAPListJson() {
  JsonArray rootPara = docPara.to<JsonArray>();

  char strValue[64] = {0};

//  WiFi.mode(WIFI_STA);
//  WiFi.disconnect();
  int n = WiFi.scanNetworks();
  ESP_LOGI(TAG, "Scan Done");
  if (n == 0) {
    ESP_LOGI(TAG, "No networks found");
  } else {
    ESP_LOGI(TAG, "%d WIFI(2.4G)Networks Found", n);
    for (int i = 0; i < n; ++i) {
      DynamicJsonDocument tempPara(256);
      JsonObject listObject = tempPara.to<JsonObject>();
      memset(strValue, 0, 64);
      snprintf(strValue, 64, "%s", WiFi.SSID(i).c_str());
      listObject["name"] = strValue;
      memset(strValue, 0, 64);
      // Print SSID and RSSI for each network found
      int nrssi = 2 * (WiFi.RSSI(i) + 100);
      if (nrssi > 100) {
        nrssi = 100;
      }
      Serial.printf("%2d:%-24.24s %s (%d:%-3d) CH%02d", i + 1, WiFi.SSID(i).c_str(), WiFi.BSSIDstr(i).c_str(), WiFi.RSSI(i), nrssi, WiFi.channel(i));
      switch(WiFi.encryptionType(i)){
        case WIFI_AUTH_OPEN:
          Serial.println(" OPEN");
          sprintf(strValue, "%s_%d_%d_OPEN", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        case WIFI_AUTH_WEP:
          Serial.println(" WEP");
          sprintf(strValue, "%s_%d_%d_WEP", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        case WIFI_AUTH_WPA_PSK:
          Serial.println(" WPA");
          sprintf(strValue, "%s_%d_%d_WPA", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        case WIFI_AUTH_WPA2_PSK:
          Serial.println(" WPA2");
          sprintf(strValue, "%s_%d_%d_WPA2", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        case WIFI_AUTH_WPA_WPA2_PSK:
          Serial.println(" WPA/WPA2");
          sprintf(strValue, "%s_%d_%d_WPA/WAP2", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        case WIFI_AUTH_WPA2_ENTERPRISE:
          Serial.println(" WPA2_EN");
          sprintf(strValue, "%s_%d_%d_WAP2_EN", WiFi.BSSIDstr(i).c_str(), nrssi, WiFi.channel(i));
          break;
        default:
          sprintf(strValue, "%s_%d_%d_UNKNOW", WiFi.BSSIDstr(i).c_str(),  nrssi, WiFi.channel(i));
          break;
      }
      listObject["value"] = strValue;
      rootPara.add(listObject);
      tempPara.clear();
    }
  }

  WiFi.scanDelete();
//  WiFi.disconnect(true);
//  WiFi.softAPdisconnect(true);

  char strPara[measureJson(docPara) + 1] = {0};
  serializeJson(docPara, strPara, measureJson(docPara) + 1);
//  Serial.println(strPara);
  memset(wlan_ap_list_file, 0, 4096);
  memcpy(wlan_ap_list_file, strPara, measureJson(docPara) + 1);
//  serializeJson(rootPara, Serial);
//  Serial.println();

  docPara.clear();
}

void NOA_WebServer_init() {
  // Create parajson mutex
  noaserver_parajson_mutex = xSemaphoreCreateMutex();
  if (!noaserver_parajson_mutex) {
    return;
  }
//  NOA_ParserParaJson();
  NOA_GeneratorParaJson();
  nStatus_WebServer = 0;
  NOA_WebServer_Task = xTaskCreateStaticPinnedToCore(
                   NOA_WebServer_Task_Loop,        // Function that implements the task.
                   "NOAWebServerTask",             // Text name for the task.
                   SIZE_OF_WEBSERVER_STACK,         // Stack size in bytes, not words.
                   NULL,                            // Parameter passed into the task.
                   tskIDLE_PRIORITY + 1,            // Priority at which the task is created.
                   xStack_WebServer,                // Array to use as the task's stack.
                   &xTaskBuffer_WebServer,          // Variable to hold the task's data structure.
                   ARDUINO_RUNNING_CORE);
  if (NOA_WebServer_Task == NULL || &xTaskBuffer_WebServer == NULL) {
    ESP_LOGE(TAG, "Create NOA_WebServer_Task fail");
  }
  vTaskDelay(1000/portTICK_PERIOD_MS);
  if (nStatus_WebServer == 0) {
    NOAServer.on("/cjson/system_para.json", HTTP_GET, [&]() {
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.setContentLength(strlen(system_para_page_file));
      NOAServer.send(200, PSTR("application/json;charset=utf-8"), system_para_page_file);
    });

    NOAServer.on("/cjson/wlan_ap_list.json", HTTP_GET, handleResponse_APlist);

    NOAServer.on("/lang/cn.json", HTTP_GET, [&]() {
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.setContentLength(strlen(cn_page_file));
      NOAServer.send_P(200, PSTR("application/json;charset=utf-8"), cn_page_file);
    });

    NOAServer.on("/lang/en.json", HTTP_GET, [&]() {
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.setContentLength(strlen(en_page_file));
      NOAServer.send_P(200, PSTR("application/json;charset=utf-8"), en_page_file);
    });

    NOAServer.on("/", HTTP_GET, [&]() {
      if (_username != emptyString && _password != emptyString && !NOAServer.authenticate(_username.c_str(), _password.c_str())) {
        return NOAServer.requestAuthentication();
      } else {
        NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
        NOAServer.sendHeader("Cache-Control", "no-cache");
        NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
        NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), ServerIndex);
      }
    });

    NOAServer.on("/SystemInfo.html", HTTP_GET, [&]() {
      NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), SystemInfo_page_file);
    });

    NOAServer.on("/wifiAPSet.html", HTTP_GET, [&]() {
      NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), wifiAPSet_page_file);
    });

    NOAServer.on("/STASettings.html", HTTP_GET, [&]() {
      NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), STASettings_page_file);
    });

    NOAServer.on("/DeviceMonitor.html", HTTP_GET, [&]() {
      NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), DeviceMonitor_page_file);
    });

    NOAServer.on("/firmware.html", HTTP_GET, [&]() {
      NOAServer.sendHeader("device", NOA_PD_DEVICENAME);
      NOAServer.sendHeader("Cache-Control", "no-cache");
      NOAServer.sendHeader("X-Content-Type-Options", "nosniff");
      NOAServer.send_P(200, PSTR("text/html;charset=utf-8"), firmware_page_file);
    });

    NOAServer.on("/update", HTTP_POST, handleResponse, handleFileupload);
    NOAServer.on("/cjson/system_para.json", HTTP_POST, handleResponse_para, handleUpload_para);

    NOAServer.onNotFound(handleNotFound);

    NOAServer.begin(); //enable Webserver
    nStatus_WebServer = 1;
    ESP_LOGI(TAG, "Web Server started");
  }
}
