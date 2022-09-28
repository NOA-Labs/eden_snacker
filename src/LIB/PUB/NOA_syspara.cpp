/*
  NOA_syspara.c - NOA system Parameters functions
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file.
*/
#include <Esp.h>
#include <WiFi.h>

#include "..\..\DRV\PDM\usb_pd.h"

#include "NOA_public.h"
#include "NOA_parameter_table.h"
#include "NOA_syspara.h"

static const char *TAG = "noa_syspara";

static xSemaphoreHandle parameter_system_mutex = NULL;

static int Para_Init_flag = 0;
// char produc_save_flag = 0;

extern uint32_t pd_src_caps[CONFIG_USB_PD_PORT_COUNT][PDO_MAX_OBJECTS];
extern uint8_t pd_src_cap_cnt[CONFIG_USB_PD_PORT_COUNT];
extern int pd_source_cap_current_index;
extern int pd_sink_port_default_valtage;
extern int pd_sink_port_default_current;

extern uint32_t pd_src_pdo[PDO_MAX_OBJECTS];
extern int pd_src_pdo_cnt;
extern int pd_source_port_default_valtage;
extern int pd_source_port_default_current;

extern uint32_t pd_src_pdo[PDO_MAX_OBJECTS];
extern int pd_src_pdo_cnt;

extern int wireless_charger_port_ready;
extern char strReleaseDate[16];
extern char strReleaseTime[16];
extern uint8_t nAP_Sta_Numbers;

void NOA_Parametr_Get(int number, char *databuf) {
  // lock
  xSemaphoreTake(parameter_system_mutex, portMAX_DELAY);
  char *buf_value = NULL;
  int len = 0;
  if (sizeof(databuf) > 0) {
    memset(databuf, '\0', (int)sizeof(databuf));
  } else {
    ESP_LOGE(TAG, "Sys para databuf size fail");
    /* unlock */
    xSemaphoreGive(parameter_system_mutex);
    return;
  }
//  ESP_LOGI(TAG, "NOA_Parametr_Get begin:%d!", number);
  int j = 0;

  for (j = 0; j < number_of_array_elements; j++) {
    if (number == NOA_PDT[j].num) {
//      APP_DEBUG("para_meter[%d]num = %ld \r\n", j, number);
      buf_value = (char *)memory_apply(sizeof(char) * NOA_PARAM_LEN);
      if (buf_value == NULL) {
        ESP_LOGE(TAG, "MEM Alloc Error");
        /* unlock */
        xSemaphoreGive(parameter_system_mutex);
        return;
      }
      memset(buf_value, 0, sizeof(char) * NOA_PARAM_LEN);
      if (number == 2 || number == 5 || number == 6\
          ||number == 11 || number == 16 || number == 17 || number == 18 || number == 19\
          || number == 22 || number == 28 || number == 29\
          || number == 30 || number == 31 || number == 32 || number == 33 || number == 34 || number == 35 || number == 36 || number == 37\
          || number == 44 || number == 46 || number == 48 || number == 49\
          || number == 50 || number == 51 || number == 52 || number == 56 || number == 57) {
        switch (number) {
          case 2:  // SN
            uint64_t chipid;
            chipid = ESP.getEfuseMac();
            sprintf(buf_value, "%" PRIu64, chipid);
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          case 5:  // Software Version
            strcpy(buf_value, Default_Para[j].para);
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          case 6:  // Hardware Version
            sprintf(buf_value, "%s Revision %d", ESP.getChipModel(), ESP.getChipRevision());
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          case 11:
            sprintf(buf_value, "%d", WiFi.softAPgetStationNum());
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          case 16:
          case 17:
          case 18:
          case 19:
          case 22: {  // Query sta IP/Mask/GW/DNS
            if (WiFi.isConnected()) {
              if (number == 16) {
                strcpy(buf_value, WiFi.localIP().toString().c_str());
              }
              if (number == 17) {
                strcpy(buf_value, WiFi.subnetMask().toString().c_str());
              }
              if (number == 18) {
                strcpy(buf_value, WiFi.gatewayIP().toString().c_str());
              }
              if (number == 19) {
                strcpy(buf_value, WiFi.dnsIP(0).toString().c_str());
              }
              if (number == 22) {
                strcpy(buf_value, WiFi.dnsIP(1).toString().c_str());
              }
            } else {
              strcpy(buf_value, "-");
            }
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          }
          case 28: { // PD SINK Voltage
              sprintf(buf_value, "%d", pd_sink_port_default_valtage);
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 29: { // PD SINK Current
              sprintf(buf_value, "%d", pd_sink_port_default_current);
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 30: { // PD SINK Capabilities
              if (pd_src_cap_cnt[0] != 0) {
                char strTemp[24] = {0};
                int nTemp_j = 0;
                for(nTemp_j = 0; nTemp_j < pd_src_cap_cnt[0]; nTemp_j++) {
//                  Serial.printf("Sink cap %d %d:%d \r\n", nTemp_j, ((pd_src_caps[0][nTemp_j] >> 10) & 0x3ff) * 50, (pd_src_caps[0][nTemp_j] & 0x3ff) * 10);
                  memset(strTemp, 0, 16);
                  sprintf(strTemp, "[%d:%d]", ((pd_src_caps[0][nTemp_j] >> 10) & 0x3ff) * 50, (pd_src_caps[0][nTemp_j] & 0x3ff) * 10);
                  if (nTemp_j == 0) {
                    strcpy(buf_value, strTemp);
                  } else {
                    strcat(buf_value, strTemp);
                  }
                }
              } else {
                strcpy(buf_value, "[-]");
              }
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 31: { // PD SRC Voltage
              sprintf(buf_value, "%d", pd_source_port_default_valtage);
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 32: { // PD SRC Current
              sprintf(buf_value, "%d", pd_source_port_default_current);
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 33: { // PD SRC Capabilities
              if (pd_src_pdo_cnt != 0) {
                char strTemp[24] = {0};
                int nTemp_j = 0;
                for(nTemp_j = 0; nTemp_j < pd_src_pdo_cnt; nTemp_j++) {
//                  Serial.printf("Src cap %d %d:%d \r\n", nTemp_j, ((pd_src_pdo[nTemp_j] >> 10) & 0x3ff) * 50, (pd_src_pdo[nTemp_j] & 0x3ff) * 10);
                  memset(strTemp, 0, 16);
                  sprintf(strTemp, "[%d:%d]", ((pd_src_pdo[nTemp_j] >> 10) & 0x3ff) * 50, (pd_src_pdo[nTemp_j] & 0x3ff) * 10);
                  if (nTemp_j == 0) {
                    strcpy(buf_value, strTemp);
                  } else {
                    strcat(buf_value, strTemp);
                  }
                }
              } else {
                strcpy(buf_value, "[-]");
              }
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 34:  // Wireless charge Voltage
            if (wireless_charger_port_ready == 1) {
              strcpy(buf_value, "9000");
            } else {
              strcpy(buf_value, "-");
            }
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            break;
          case 35:  // Wireless charge Temperature
            break;
          case 44: { // STA MAC
              strcpy(buf_value, WiFi.macAddress().c_str());
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 46: { // AP SSID
              strcpy(buf_value, WiFi.softAPSSID().c_str());
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 48: { // RSSI
              int nrssi = 2 * (WiFi.RSSI() + 100);
              if (nrssi > 100) {
                nrssi = 100;
              }
              sprintf(buf_value, "%d:%d", WiFi.RSSI(), nrssi);
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 49: { // Active STA SSID
              if (WiFi.isConnected()) {
                strcpy(buf_value, WiFi.SSID().c_str());
              } else {
                strcpy(buf_value, "-");
              }
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 50: { // AP Scan
              memory_release(buf_value);
              xSemaphoreGive(parameter_system_mutex);
              return;
            }
            break;
          case 51:  // release date
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], strReleaseDate, &len);
            break;
          case 52:  // release time
            memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
            NOA_PDT[j].wFunc(&NOA_PDT[j], strReleaseTime, &len);
            break;
          case 56: { // AP MAC
              strcpy(buf_value, WiFi.softAPmacAddress().c_str());
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          case 57:  { // AP IP
              strcpy(buf_value, WiFi.softAPIP().toString().c_str());
              memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
              NOA_PDT[j].wFunc(&NOA_PDT[j], buf_value, &len);
            }
            break;
          default:
            break;
        }
      }

      memset(buf_value, 0, sizeof(char) * NOA_PARAM_LEN);
      NOA_PDT[j].rFunc(&NOA_PDT[j], buf_value, &len);
      if (len > 0 && len <= NOA_PARAM_LEN) {  // limit parameter len
        if (sizeof(databuf) > 0 && sizeof(databuf) <= NOA_PARAM_LEN) {
          memset(databuf, '\0', NOA_PARAM_LEN);
          memcpy(databuf, buf_value, len);
        } else {
          ESP_LOGE(TAG, "Sys para databuf size fail");
        }
      } else {
        ESP_LOGE(TAG,"para fail!");
      }
      memory_release(buf_value);
      break;
    }
  }

//  ESP_LOGI(TAG, "index: %d totle: %d", j, number_of_array_elements);
  if (j >= number_of_array_elements) {
//    ESP_LOGI(TAG, "number %d index: %d totle: %d", number, j, number_of_array_elements);
    databuf = NULL;
  }
  // unlock
  xSemaphoreGive(parameter_system_mutex);
}

int NOA_Parametr_Set(int number, char *data) {
  int j = 0;
  int ret = 0x00;
  char *str = NULL;
  // lock
  xSemaphoreTake(parameter_system_mutex, portMAX_DELAY);
  for (j = 0; j < number_of_array_elements; j++) {
    if (number == NOA_PDT[j].num) {
//      ESP_LOGI(TAG, "para[%d] number is %ld Old value is %s", j, number, NOA_PDT[j].a);
//      ESP_LOGI(TAG, "para[%d] number is %ld New value will be %s", j, number, data);
      str = (char *)memory_apply(sizeof(char) * NOA_PARAM_LEN);
      if (str == NULL) {
        ESP_LOGE(TAG, "MEM Alloc Error");
        ret = 0x03;
        // unlock
        xSemaphoreGive(parameter_system_mutex);
        return ret;
      }
      memset(str, '\0', sizeof(char) * NOA_PARAM_LEN);
      int nlen = strlen((char *)data);  // limit para len
      if (nlen >= (NOA_PARAM_LEN - 1)) {
        nlen = NOA_PARAM_LEN - 1;
      }
      memcpy(str, (char *)data, nlen);
//      ESP_LOGI(TAG, "para[%d] number is %ld str %s", j, number, str);
      switch (number) {
        case 2:
        case 5:
        case 6:
        case 11:
        case 27:
        case 28:
        case 29:
        case 30:
        case 31:
        case 32:
        case 33:
        case 34:
        case 35:
        case 44:
        case 48:
        case 49:
        case 50:
        case 51:
        case 52:
        case 56:
          ESP_LOGI(TAG, "para_meter %d can't be set", number);
          ret = 0x02;
          break;
        default:
          break;
      }

      if (ret == 0x00) {
        ret = 0x00;
        // limit para len
        if (strlen(data) >= sizeof((&NOA_PDT[j])->a)) {
          ESP_LOGE(TAG, "para too long!");
          ret = 0x03;
          memory_release(str);
          break;
        }
        memset((&NOA_PDT[j])->a, 0, sizeof((&NOA_PDT[j])->a));
        NOA_PDT[j].wFunc(&NOA_PDT[j], str, &nlen);
//        ESP_LOGI(TAG, "PARA %d=%s", NOA_PDT[j].num, str);
        memory_release(str);
        break;
      }
      ESP_LOGI(TAG, "para_meter %ld is ready", number);
      memory_release(str);
    }
    if (j >= number_of_array_elements) {
      ESP_LOGE(TAG, "para_meter %ld is wrong", number);
      ret = 0x03;
    }
  }

  if (ret == 0x00) {
//  parameter_a_module();  // save para to flash or disk
  }
  // unlock
  xSemaphoreGive(parameter_system_mutex);
  return ret ;
}

void NOA_Parameter_Update(void) { // Update para with detault value for OTA
  int len = 0;
  int j = 0;
  int number = 0;
  int ret_update = 0;

  ESP_LOGI(TAG, "Checking NOA_Parameter_Update %d!", number_of_array_elements);
  for (j = 0; j < number_of_array_elements; j++) {
    number = NOA_PDT[j].num;
    if (number == 5) {
      len = strlen(Default_Para[j].para);
      if (memcmp(Default_Para[j].para, (&NOA_PDT[j])->a, len) != 0) {
        memset((&NOA_PDT[j])->a, '\0', sizeof((&NOA_PDT[j])->a));
        NOA_PDT[j].wFunc(&NOA_PDT[j], (void *)Default_Para[j].para, &len);
        ret_update = 1;
      }
    }
  }
  if (ret_update == 1) {
    ESP_LOGI(TAG, "NOA_Parameter_Update for version update!");
  }
}

int NOA_SysPara_init(void) {
  ESP_LOGI(TAG, "NOA_SysPara_init");
  Para_Init_flag = 0;
  // Create para mutex
  parameter_system_mutex = xSemaphoreCreateMutex();
  if (!parameter_system_mutex) {
    return -1;
  }
  // lock
  xSemaphoreTake(parameter_system_mutex, portMAX_DELAY);
  NOA_Parametr_Default();
  NOA_Parameter_Update();
  // unlock
  xSemaphoreGive(parameter_system_mutex);
  return 0;
}

int SysPara_Get_State(void) {
  return Para_Init_flag;
}

void NOA_Parametr_Default(void) {   // Reset NOA_PDT table from Default_Para table
  ESP_LOGD(TAG, "NOA_Parametr_Default");

  int len = 0;
  int i = 0;
  for (i = 0; i < number_of_array_elements; i++) {
    if (strlen(Default_Para[i].para) <= 0) {
      continue;
    }
    memset((&NOA_PDT[i])->a, 0, sizeof((&NOA_PDT[i])->a));
    len = strlen(Default_Para[i].para);
    NOA_PDT[i].wFunc(&NOA_PDT[i], (void *)Default_Para[i].para, &len);
  }
}

void NOA_Get_All_Data(void) {
  int j = 0;
  char *buf  = NULL;
  int len = NOA_PARAM_LEN;

  int number = 0;
  char buf_temp[NOA_PARAM_LEN] = {0};
  for (j = 0; j < number_of_array_elements; j++) {
    number = NOA_PDT[j].num;
    Serial.printf("%2d ", j);
    // Update some para that are active updated
    if (number == 2 || number == 5 || number == 6\
        ||number == 11 || number == 16 || number == 17 || number == 18 || number == 19\
        || number == 22 || number == 28 || number == 29\
        || number == 30 || number == 31 || number == 32 || number == 33 || number == 34 || number == 35 || number == 36 || number == 37\
        || number == 44 || number == 46 || number == 48 || number == 49\
        || number == 50 || number == 51 || number == 52 || number == 56 || number == 57) {
      NOA_Parametr_Get(number, (char *)&buf_temp);
    }
    buf = (char *)memory_apply(sizeof(char) * NOA_PARAM_LEN);
    memset(buf, 0, sizeof(char) * NOA_PARAM_LEN);
    NOA_PDT[j].rFunc(&NOA_PDT[j], buf, &len);
    if (strlen(buf) > 0) {
      Serial.printf("%d=%s\r\n", NOA_PDT[j].num, buf);
    }
    memory_release(buf);
  }
  Para_Init_flag = 1;
}
/******************************************************************************/

