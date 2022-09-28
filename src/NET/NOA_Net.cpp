/*
  NOA_Net.cpp - Library for Net APP..
  Copyright 2021 NOA
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/
#include <Esp.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "..\LIB\PUB\NOA_public.h"
#include "..\LIB\PUB\NOA_syspara.h"
#include "..\LIB\PUB\NOA_parameter_table.h"

#include "..\DRV\PDM\usb_pd.h"
#include "..\APP\NOA_App.h"
#include "..\WEB\NOA_WebServer.h"

#include "NOA_Net.h"

#define SIZE_OF_NET_STACK  SIZE_OF_STACK * 4

TaskHandle_t NOA_Net_Task = NULL;
StaticTask_t xTaskBuffer_Net;
StackType_t xStack_Net[SIZE_OF_NET_STACK/2];
xQueueHandle NOA_NET_TASKQUEUE = NULL;
static int nStatus_WiFiAP = 0;
static int nStatus_WiFiSTA = 0;
static int nStatus_WiFiWebServer = 0;

TaskHandle_t WIFI_Test_Task = NULL;
StaticTask_t xTaskBuffer_WIFI_Test;
StackType_t xStack_WIFI_Test[SIZE_OF_NET_STACK];
static int nStatus_WiFiTesting = 0;

TaskHandle_t WIFI_Http_Task = NULL;
StaticTask_t xTaskBuffer_WIFI_Http;
StackType_t xStack_WIFI_Http[SIZE_OF_NET_STACK];
static int nStatus_WiFiHttp = 0;
// const char* http_url="https://blob.wi-whisper.com/videos/video0_480_272.avi";
// const char* http_url="https://downloads.arduino.cc/arduino-1.8.16-windows.zip";
// const char* http_url="https://mirrors.gigenet.com/ubuntu/20.04.3/ubuntu-20.04.3-live-server-amd64.iso";
// const char* http_url="https://mirrors.tuna.tsinghua.edu.cn/ubuntu-releases/21.04/ubuntu-21.04-live-server-amd64.iso";

const char* http_url="http://192.168.1.102/video0_480_272.avi";

IPAddress sip(192,168,1,29);      //Sta IP
IPAddress sip1(192,168,1,1);      //Sta gateway
IPAddress sip2(255,255,255,0);    //Sta netmask
IPAddress serverip(192,168,1,4);  //Remote IP for Local network

IPAddress lxip(192,168,88,1);     //AP IP
IPAddress lxip1(192,168,88,1);    //AP gateway
IPAddress lxip2(255,255,255,0);   //AP netmask
IPAddress xip(192,168,88,2);      //Remote IP for AP network

WiFiServer Iperf_Server(5001);

unsigned int localPort = 0;        //Local port
unsigned int remoteport = 0;       //Remote port
#define NOA_ESP32_HOST_NAME  "NOA_SNACKER_ESP32"
uint8_t nAP_Sta_Numbers = 0;

static const char *TAG = "noa_net";
//****************************************************************************
// CODE TABLES
//****************************************************************************
void WiFiEvent(WiFiEvent_t event){
  NOA_PUB_MSG msg;
  memset(&msg, 0, sizeof(NOA_PUB_MSG));
  switch(event) {
    case SYSTEM_EVENT_AP_START: {
        Serial.println("AP Started");
        WiFi.softAPConfig(lxip, lxip1, lxip2);  // Set AP Net parameters
//      WiFi.softAPsetHostname(NOA_ESP32_HOST_NAME);
//      WiFi.setHostname(NOA_ESP32_HOST_NAME);
        String strAPSSID = WiFi.softAPSSID();
        WiFi.softAPsetHostname(strAPSSID.c_str());
        WiFi.setHostname(strAPSSID.c_str());
        msg.message = APNET_MSG_READY;
        if (NOA_APP_TASKQUEUE != NULL) {
          xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
        }
        nStatus_WiFiAP = 1;
      }
      break;
    case SYSTEM_EVENT_AP_STOP: {
        Serial.println("AP Stopped");
        msg.message = APNET_MSG_NOTREADY;
        if (NOA_APP_TASKQUEUE != NULL) {
          xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
        }
        nStatus_WiFiAP = 0;
      }
      break;
    case SYSTEM_EVENT_AP_STACONNECTED:
      Serial.println("AP Sta Connected");
      nAP_Sta_Numbers = WiFi.softAPgetStationNum();
//      if (nAP_Sta_Numbers > 0) {
//        nAP_Sta_Numbers = nAP_Sta_Numbers - 1;
//      }
      Serial.print("AP Sta Num:");
      Serial.println(nAP_Sta_Numbers);
      break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
      Serial.println("AP Sta Disconnected");
      nAP_Sta_Numbers = WiFi.softAPgetStationNum();
//      if (nAP_Sta_Numbers > 0) {
//        nAP_Sta_Numbers = nAP_Sta_Numbers - 1;
//      }
      Serial.print("AP Sta Num:");
      Serial.println(nAP_Sta_Numbers);
      break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
      Serial.println("AP Sta Passigned");
      break;
    case SYSTEM_EVENT_STA_START:
//      Serial.println("STA Started");
      break;
    case SYSTEM_EVENT_STA_CONNECTED:
//      Serial.println("STA Connected");
//      WiFi.enableIpV6();
      break;
    case SYSTEM_EVENT_AP_STA_GOT_IP6:
      Serial.print("STA IPv6: ");
      Serial.println(WiFi.localIPv6());
      break;
    case SYSTEM_EVENT_STA_GOT_IP: {
        Serial.println("STA IPv4: ");
//      Serial.println(WiFi.localIP());
//      Serial.println(WiFi.subnetMask());
//      Serial.println(WiFi.gatewayIP());
//      Serial.println(WiFi.dnsIP(0));
//      Serial.println(WiFi.dnsIP(1));
        msg.message = NET_MSG_READY;
        if (NOA_APP_TASKQUEUE != NULL) {
          xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
        }
        nStatus_WiFiSTA = 1;
      }
      break;
    case SYSTEM_EVENT_STA_LOST_IP: {
      Serial.println("STA IPv4: Lost IP");
      }
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED: {
        Serial.println("STA Disconnected");
        msg.message = NET_MSG_NOTREADY;
        if (NOA_APP_TASKQUEUE != NULL) {
          xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
        }
        nStatus_WiFiSTA = 0;
      }
      break;
    case SYSTEM_EVENT_STA_STOP:
//      Serial.println("STA Stopped");
      break;
    case SYSTEM_EVENT_WIFI_READY:
      Serial.println("WIFI Ready");
      break;
    case SYSTEM_EVENT_SCAN_DONE:
      Serial.println("Scan Done");
      break;
    default:
      Serial.println(event, HEX);
      break;
  }
}

void WIFI_Test_Task_Loop( void * pvParameters ){
  ESP_LOGI(TAG, "WIFI_Test_Task_Loop running on core %d", xPortGetCoreID());
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  NOA_UpdateAPListJson();
  WiFi.disconnect(true);
  WiFi.softAPdisconnect(true);

  char deviceid[21] = {0};
  uint64_t chipid;
  chipid = ESP.getEfuseMac();
  sprintf(deviceid, "%" PRIu64, chipid);
  uint16_t idlen = strlen(deviceid);
//  Serial.printf(" ESP Device ID %s %d\r\n", deviceid, idlen);
  char strWIFIap[32] = {0};
  memset(strWIFIap,'\0', 32);
  snprintf(strWIFIap, 32, "NOA_SNACKER_%-6.6s", &deviceid[idlen - 6]);
//  ESP_LOGI(TAG, "AP_SSID %s", strWIFIap);
  const char* ssid1 = strWIFIap;

  char strWIFIappass[64] = {0};
  memset(strWIFIappass, 0, 64);
  NOA_Parametr_Get(47, (char *)&strWIFIappass);
  if (strlen(strWIFIappass) <= 0) {
    memcpy(strWIFIappass, "87654321", 8);
  }
  const char* password1=strWIFIappass;

  char strSSID[64] = {0};
  memset(strSSID, 0, 64);
  NOA_Parametr_Get(41, (char *)&strSSID);
  if (strlen(strSSID) <= 0) {
    memcpy(strSSID, "NOARDTest", 9);
  }
  const char* ssid = strSSID;

  char strSSIDpass[64] = {0};
  memset(strSSIDpass, 0, 64);
  NOA_Parametr_Get(43, (char *)&strSSIDpass);
  if (strlen(strSSIDpass) <= 0) {
    memcpy(strSSIDpass, "12345678", 8);
  }
  const char *password=strSSIDpass;

  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_AP_STA);
  WiFi.setAutoReconnect(false);

  WiFi.softAP(ssid1, password1, 2);  // Set AP SSID and Passwd
//  WiFi.softAPConfig(lxip, lxip1, lxip2);  // Set AP Net parameters

  WiFi.begin(ssid, password);  // Set STA SSID and Passwd
//  WiFi.config(sip, sip1, sip2);  // Set STA Net parameters

  Serial.println("Please wait for Iperf client...");
//  Serial.println(WiFi.getTxPower());
//  Serial.println(WiFi.softAPSSID().c_str());
//  Serial.println(WiFi.SSID().c_str());
//  Serial.println(WiFi.getHostname());
//  Serial.println(WiFi.softAPgetHostname());
//  Serial.printf("STA mac: %s\r\n", WiFi.macAddress().c_str());
//  Serial.printf(" AP mac: %s\r\n", WiFi.softAPmacAddress().c_str());
  nStatus_WiFiTesting = 1;
  char strValue[64] = {0};
  memset(strValue, 0, 64);
  NOA_Parametr_Get(20, (char *)&strValue);
  if (strlen(strValue) > 0) {
    localPort = NOA_PUB_Swap_charNum(strValue);
//    ESP_LOGI(TAG, "%s %d", strValue, localPort);
  } else {
    localPort=5001;
  }

  memset(strValue, 0, 64);
  NOA_Parametr_Get(21, (char *)&strValue);
  if (strlen(strValue) > 0) {
    remoteport = NOA_PUB_Swap_charNum(strValue);
//    ESP_LOGI(TAG, "%s %d", strValue, remoteport);
  } else {
    remoteport=80;
  }

  ESP_LOGI(TAG, "%d %d", localPort, remoteport);

  Iperf_Server.begin(localPort);
  uint8_t data_buffer[1024] = {0};
  while(true) {
    // listen for incoming clients
    WiFiClient Iperf_Client = Iperf_Server.available();
    if (Iperf_Client) {
      Serial.printf("%ld Get a New Iperf Client\r\n", millis()/1000);
      while (Iperf_Client.connected()) {
        if (Iperf_Client.available()) {
          Iperf_Client.read(data_buffer, 1024);
//          memset(&data_buffer, 0, SIZE_OF_STACK);
        }
      }
      // close the connection:
      Iperf_Client.stop();
      Serial.printf("%ld Iperf Client disconnected\r\n", millis()/1000);
    }
//    vTaskDelay(1/portTICK_PERIOD_MS);
  }

  ESP_LOGI(TAG, "WIFI_Test_Task_Loop Exit from core %d", xPortGetCoreID());
  vTaskDelete(NULL);
}

#define NOA_HTTP_BUFFER_SIZE  512
uint8_t http_buff[NOA_HTTP_BUFFER_SIZE] = { 0 };
// WIFI_Http_Task_Loop: Wifi http download task
void WIFI_Http_Task_Loop(void * pvParameters) {
  ESP_LOGI(TAG, "WIFI_Http_Task_Loop running on core %d", xPortGetCoreID());
  HTTPClient http_down;
  int32_t  http_down_size = -1;
  WiFiClient *http_stream = NULL;
  while(true){
    if (nStatus_WiFiSTA == 1 && nStatus_WiFiHttp == 1) {
      ESP_LOGI(TAG, "Http Download begin %d", millis()/1000);
      http_down.begin(http_url);
      http_down.setFollowRedirects(HTTPC_FORCE_FOLLOW_REDIRECTS);
      ESP_LOGI(TAG, "Http Download GET %d", millis()/1000);
      int httpCode = http_down.GET();
      if(httpCode > 0) {
        ESP_LOGI(TAG, "Http Download GET code %d", httpCode);
        if(httpCode == HTTP_CODE_OK) {
          // get lenght of document (is -1 when Server sends no Content-Length header)
          http_down_size = http_down.getSize();
          ESP_LOGI(TAG, "Http Download file size %d", http_down_size);
          // get tcp stream
          http_stream = http_down.getStreamPtr();
          uint32_t nTemp_new = millis() / 1000;
          uint32_t nTemp_old = 0;
          uint32_t nDownload_size = 0;
          uint32_t nDownload_size_old = 0;
          uint32_t stream_size = 0;
          uint32_t nReadnumbers = 0;
          // read all data from server
          while(http_down.connected() && (http_down_size > 0 || http_down_size == -1)) {
            stream_size = http_stream->available();
            if(stream_size) {
              nTemp_new = millis() / 1000;
              nReadnumbers = http_stream->readBytes(http_buff, ((stream_size > sizeof(http_buff)) ? sizeof(http_buff) : stream_size));;
              nDownload_size = nDownload_size + nReadnumbers;
              if (nTemp_new >= (nTemp_old + 5)) {
                ESP_LOGI(TAG, "%d %d %d %d/s %d-%d",nTemp_new, stream_size, nReadnumbers, (nDownload_size - nDownload_size_old)/5, nDownload_size, http_down_size);
                nTemp_old = nTemp_new;
                nDownload_size_old = nDownload_size;
              }
              if (http_down_size > 0) {
                http_down_size = http_down_size - nReadnumbers;
              }
            }
          }
          ESP_LOGI(TAG, "Http Download closed or file end %d.", nDownload_size);
        }
      } else {
        ESP_LOGE(TAG, "Http Download GET failed, error: %s", http_down.errorToString(httpCode).c_str());
      }
      http_down.end();
    }
    vTaskDelay(1000/portTICK_PERIOD_MS);
  }
  ESP_LOGI(TAG, "WIFI_Http_Task_Loop Exit from core %d", xPortGetCoreID());
  vTaskDelete(NULL);
}

// Net_APP_Task_Loop: Net APP task
void Net_APP_Task_Loop(void * pvParameters) {
  ESP_LOGI(TAG, "Net_APP_Task_Loop running on core %d", xPortGetCoreID());
  BaseType_t xStatus = 0;
  NOA_PUB_MSG msg;
  char strSSID_temp[NOA_PARAM_LEN] = {0};
  char strSSIDpass_temp[NOA_PARAM_LEN] = {0};

  while(true){
    memset(&msg, 0, sizeof(NOA_PUB_MSG));
    xStatus = xQueueReceive(NOA_NET_TASKQUEUE, &msg, portMAX_DELAY);
    if (xStatus == pdPASS) {
//    ESP_LOGI(TAG, "Net_APP_Task_Loop get a message %d QueueSpaces %d", msg.message, uxQueueSpacesAvailable(NOA_NET_TASK));
    }
    switch(msg.message) {
      case APP_MSG_TIMER_ID:
//        ESP_LOGI(TAG, "Net task APP_MSG_TIMER_ID, StackSize %ld", uxTaskGetStackHighWaterMark(NULL));
        if (nStatus_WiFiAP == 1 || nStatus_WiFiSTA == 1) {
          if (nStatus_WiFiWebServer == 0) {
            nStatus_WiFiWebServer = 1;
            NOA_Get_All_Data();
            NOA_WebServer_init();
          }
        }
        break;
      case APNET_MSG_RECONNECT: {
//          ESP_LOGI(TAG, "Net task Get APNET_MSG_RECONNECT, StackSize %ld", uxTaskGetStackHighWaterMark(NULL));
//          WiFi.reconnect();
          memset(strSSID_temp, 0, NOA_PARAM_LEN);
          NOA_Parametr_Get(41, (char *)&strSSID_temp);
          if (strlen(strSSID_temp) <= 0) {
            memcpy(strSSID_temp, "NOARDTest", 9);
          }
          const char* ressid = strSSID_temp;
          memset(strSSIDpass_temp, 0, NOA_PARAM_LEN);
          NOA_Parametr_Get(43, (char *)&strSSIDpass_temp);
          if (strlen(strSSIDpass_temp) <= 0) {
            memcpy(strSSIDpass_temp, "12345678", 8);
          }
          const char *repassword=strSSIDpass_temp;
//          ESP_LOGI(TAG, "%s, %s", strSSID_temp, strSSIDpass_temp);
          WiFi.disconnect(true);
          WiFi.begin(ressid, repassword);  // ReSet STA SSID and Passwd
         //  WiFi.config(sip, sip1, sip2);  // Set STA Net parameters
        }
        break;
      default:
//        ESP_LOGI(TAG, "Net task runing");
        break;
    }
  }
  ESP_LOGI(TAG, "Net_APP_Task_Loop Exit from core %d", xPortGetCoreID());
  vTaskDelete(NULL);
}

void NOA_Net_init() {
  nStatus_WiFiTesting = 0;
  WIFI_Test_Task = xTaskCreateStaticPinnedToCore(
                   WIFI_Test_Task_Loop,       // Function that implements the task.
                   "WIFITestTask",          // Text name for the task.
                   SIZE_OF_NET_STACK,       // Stack size in bytes, not words.
                   NULL,                    // Parameter passed into the task.
                   tskIDLE_PRIORITY + 1,        // Priority at which the task is created.
                   xStack_WIFI_Test,          // Array to use as the task's stack.
                   &xTaskBuffer_WIFI_Test,   // Variable to hold the task's data structure.
                   ARDUINO_RUNNING_CORE);
  if (WIFI_Test_Task == NULL || &xTaskBuffer_WIFI_Test == NULL) {
    ESP_LOGE(TAG, "Create WIFI_Test_Task fail");
  }
  vTaskDelay(1000/portTICK_PERIOD_MS);
//  nStatus_WiFiHttp = 0;
//  WIFI_Http_Task = xTaskCreateStaticPinnedToCore(
//                   WIFI_Http_Task_Loop,       // Function that implements the task.
//                   "WIFIHttpTask",          // Text name for the task.
//                   SIZE_OF_NET_STACK,       // Stack size in bytes, not words.
//                   NULL,                    // Parameter passed into the task.
//                   tskIDLE_PRIORITY + 1,        // Priority at which the task is created.
//                   xStack_WIFI_Http,          // Array to use as the task's stack.
//                   &xTaskBuffer_WIFI_Http,   // Variable to hold the task's data structure.
//                   ARDUINO_RUNNING_CORE);
//  if (WIFI_Http_Task == NULL || &xTaskBuffer_WIFI_Http == NULL) {
//    ESP_LOGE(TAG, "Create WIFI_Http_Task fail");
//  }
//  vTaskDelay(1000/portTICK_PERIOD_MS);
  NOA_NET_TASKQUEUE = xQueueCreate(SIZE_OF_TASK_QUEUE, sizeof(NOA_PUB_MSG));
  if (NOA_NET_TASKQUEUE == NULL) {
    ESP_LOGE(TAG, "Create NOA_NET_TASKQUEUE fail");
  }

  NOA_Net_Task = xTaskCreateStaticPinnedToCore(
                   Net_APP_Task_Loop,       // Function that implements the task.
                   "NetAPPTask",            // Text name for the task.
                   SIZE_OF_NET_STACK/2,       // Stack size in bytes, not words.
                   NULL,                    // Parameter passed into the task.
                   tskIDLE_PRIORITY + 1,    // Priority at which the task is created.
                   xStack_Net,          // Array to use as the task's stack.
                   &xTaskBuffer_Net,    // Variable to hold the task's data structure.
                   ARDUINO_RUNNING_CORE);
  if (NOA_Net_Task == NULL || &xTaskBuffer_Net == NULL) {
    ESP_LOGE(TAG, "Create Net_App_Task fail");
  }
  nStatus_WiFiWebServer = 0;
}
