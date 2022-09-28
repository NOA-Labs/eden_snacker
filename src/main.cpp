#include <Wire.h>

#include <Esp.h>
#include <ESP32AnalogRead.h>

#include "DRV\PDM\usb_pd.h"
#include "DRV\PDM\FUSB302.h"
#include "DRV\PDM\sc8721xx_drv.h"

#include "LIB\PUB\NOA_TimeDefs.h"
#include "LIB\PUB\NOA_public.h"

extern int const usb_pd_snk_sel_pin;

#ifdef NOA_PD_SNACKER
#include "LIB\PUB\NOA_syspara.h"
#include "DRV\NFC\Adafruit_OM9663.h"
#include "DRV\NFC\NOA_NFC.h"
#include "APP\NOA_App.h"
#include "NET\NOA_Net.h"
#include "UI\NEOPixel.h"

// #include "WEB\NOA_WebServer.h"

const int usb_pd_snk_int_pin = 32;    // init pin for PD snk
const int usb_pd_src1_int_pin = 23;   // init pin for PD src

extern int const ncp_bb_con9v_tempadc_pin;
const int ncp_bb_con9v_tempadc_pin = 15;   // Wireless Charger Coil Temperrature ADC Input
ESP32AnalogRead ncp_bb_con9v_tempadc;
const int ncp_bb_con9v_led_pin = 18;       // Wireless Charger Status Indication

int wireless_charger_port_ready = 0;
// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
  {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv}, // SNK
  {1, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv}, // SRC
};
// USB-C Specific - TCPM end 1

const uint8_t PD_ADDR = 0x22;
const uint8_t PM_ADDR = 0x74;
const uint8_t NFC_ADDR = 0x29;

const int mfrc_630_nfc_int_pin = 5;  // init pin for NFC
const int mfrc_630_nfc_pdown_pin = 4;  // init pin for NFC
Adafruit_OM9663 rfid_nfc = Adafruit_OM9663(1, OM9663_I2C_ADDR, mfrc_630_nfc_pdown_pin);
static bool NFC_Status_new = false;
static bool NFC_Status_old = false;
static uint32_t nTime_new = 0;
static uint32_t nTime_old = 0;
#else
const int usb_pd_snk_int_pin = 38;  // init pin for PD snk(P1)
const int usb_pd_snk_sel_pin = 14;  // sel pin for PD snk(P1)

const int usb_pd_src2_int_pin = 39; // init pin for PD src_2(P0)
int usb_pd_src2_sel_pin = 27;       // sel pin for PD src_2(P0)
const int ncp_bb_con2_int_pin = 13; // init pin for src_2 ncp81239(P0)
int ncp_bb_con2_en_pin = 4;         // enable pin for src_2 ncp81239(P0)

const int usb_pd_src1_int_pin = 34; // init pin for PD src_1(P2)
int usb_pd_src1_sel_pin = 19;       // sel pin for PD src_1(P2)
const int ncp_bb_con1_int_pin = 35; // init pin for src_1 ncp81239(P2)
int ncp_bb_con1_en_pin = 2;         // enable pin for src_1 ncp81239(P2)

const int usb_pd_src3_int_pin = 36; // init pin for PD src_3(P3)
int usb_pd_src3_sel_pin = 26;       // sel pin for PD src_3(P3)
const int ncp_bb_con3_int_pin = 37; // init pin for src_3 ncp81239(P3)
int ncp_bb_con3_en_pin = 25;        // enable pin for src_3 ncp81239(P3)

// USB-C Specific - TCPM start 1
const struct tcpc_config_t tcpc_config[CONFIG_USB_PD_PORT_COUNT] = {
  {0, fusb302_I2C_SLAVE_ADDR_B01, &fusb302_tcpm_drv},   // SNK(C0)(P1) bus0
  {1, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},       // SRC(C1)(P2) bus1
  {0, fusb302_I2C_SLAVE_ADDR, &fusb302_tcpm_drv},       // SRC(C2)(P0 UP) bus0
  {1, fusb302_I2C_SLAVE_ADDR_B01, &fusb302_tcpm_drv},   // SRC(C3)(P3) bus1
};
// USB-C Specific - TCPM end 1

const uint8_t P1D_ADDR = 0x23;

const uint8_t PUPD_ADDR = 0x22;
const uint8_t PUPM_ADDR = 0x74;

const uint8_t P2D_ADDR = 0x22;
const uint8_t P2M_ADDR = 0x74;

const uint8_t P3D_ADDR = 0x23;
const uint8_t P3M_ADDR = 0x75;
#endif

int pd_source_cap_current_index = 0, pd_source_cap_max_index = 0;
static int pd_sink_port_ready = 0;
static int pd_source_port_ready = 0;
int pd_sink_port_default_valtage = 0;
int pd_sink_port_default_current = 0;

int pd_source_port_default_valtage = 0;
int pd_source_port_default_current = 0;

char strReleaseDate[16] = {0};
char strReleaseTime[16] = {0};

// This banner is checked the memmory of MCU platform
const char NOA_Banner[] = {0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,\
                           0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,\
                           0xe2, 0x96, 0x88, 0x20, 0x0d, 0x0a, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,\
                           0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20,\
                           0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x0d, 0x0a, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2,\
                           0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2,\
                           0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x0d, 0x0a, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0xe2, 0x96, 0x88,\
                           0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88,\
                           0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x0d, 0x0a, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20,\
                           0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0xe2,\
                           0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x20, 0x20, 0x20, 0xe2, 0x96, 0x88, 0xe2, 0x96, 0x88, 0x0d, 0x0a};

void Uart_Print_Info() {
  Serial.print(__DATE__);
  Serial.print(" ");
  Serial.print(__TIME__);
  Serial.println();
//  Serial.print(NOA_Banner);
//  char building_time[17] = {0};
//  sprintf(building_time, "%04d%02d%02d%02d%02d%02d", BUILD_DATE_YEAR_INT,BUILD_DATE_MONTH_INT,BUILD_DATE_DAY_INT,BUILD_TIME_HOURS_INT,BUILD_TIME_MINUTES_INT,BUILD_TIME_SECONDS_INT);
//  Serial.println(building_time);
  Serial.println("==========================================");
#ifdef NOA_PD_SNACKER
  Serial.printf(" NOA PD SNACKER Firmware %s\r\n", NOA_ESP32_PD_VERSION);
#else
  Serial.printf(" NOA PD STATION Firmware %s\r\n", NOA_ESP32_PD_VERSION);
#endif
  memset(strReleaseDate, 0, 16);
  memset(strReleaseTime, 0, 16);
  sprintf(strReleaseDate, "%04d%02d%02d", BUILD_DATE_YEAR_INT,BUILD_DATE_MONTH_INT,BUILD_DATE_DAY_INT);
  sprintf(strReleaseTime, "%02d%02d%02d", BUILD_TIME_HOURS_INT,BUILD_TIME_MINUTES_INT,BUILD_TIME_SECONDS_INT);
//  Serial.printf(" Building Time %04d%02d%02d%02d%02d%02d\r\n", BUILD_DATE_YEAR_INT,BUILD_DATE_MONTH_INT,BUILD_DATE_DAY_INT,BUILD_TIME_HOURS_INT,BUILD_TIME_MINUTES_INT,BUILD_TIME_SECONDS_INT);
  Serial.printf(" Building Time %s%s\r\n", strReleaseDate, strReleaseTime);
  Serial.printf(" ESP Chip Model %s Revision %d\r\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf(" ESP Chip Cores %d CPUFrea %luMHz\r\n", ESP.getChipCores(), (unsigned long)ESP.getCpuFreqMHz());
  Serial.printf(" ESP SDK Version %s\r\n", ESP.getSdkVersion());
  Serial.printf(" ESP Heap Free(%d)/Size(%d)Bytes\r\n", ESP.getFreeHeap(), ESP.getHeapSize());
  Serial.printf(" ESP Flash Size %dMB Mode %d Speed %dMHz\r\n", ESP.getFlashChipSize()/1024/1024, ESP.getFlashChipMode(), ESP.getFlashChipSpeed()/1000/1000);

  char deviceid[21] = {0};
  uint64_t chipid;
  chipid = ESP.getEfuseMac();
  sprintf(deviceid, "%" PRIu64, chipid);
  Serial.printf(" ESP Device ID %s\r\n", deviceid);
  Serial.printf(" ESP Core Numbers %d Arduino core %d\r\n", xPortGetCoreID() + 1, ARDUINO_RUNNING_CORE);
  Serial.printf(" NOA Sketch MD5 %s\r\n", ESP.getSketchMD5().c_str());
  Serial.printf(" NOA Sketch Used %d All %d\r\n", ESP.getSketchSize(), ESP.getFreeSketchSpace()); 

  NOA_PUB_PartScan(ESP_PARTITION_TYPE_APP);
  NOA_PUB_PartScan(ESP_PARTITION_TYPE_DATA);

//  Serial.printf(" NOA ESP APP Starting on %s board\r\n",ARDUINO_BOARD);
//  Serial.printf(" CPU Frequency = %ld MHz\r\n", (F_CPU / 1000000));
  Serial.println("==========================================");  
}

#ifdef NOA_PD_SNACKER
TaskHandle_t  Main_Task = NULL;
StackType_t   TaskStackBuffer[SIZE_OF_STACK];
StaticTask_t  TaskTCBBuffer;
static bool bMain_Task = false;

void start_vTask(void * pvParameters) {
  NEO_Pixel_init();   // Init RGB pixel
  vTaskDelay(1000/portTICK_PERIOD_MS);
  NOA_App_init();     // Init NOA Main APP
  vTaskDelay(1000/portTICK_PERIOD_MS);
  NOA_Net_init();     // Init NOA Net App
  vTaskDelay(1000/portTICK_PERIOD_MS);
  bMain_Task = true;
  vTaskDelete(NULL);
}
#endif

void setup() {
  Serial.begin(115200);
  while(!Serial) {
    delay(100);
  }
  sc87xx_drv_init();
  Uart_Print_Info();
  NOA_PUB_ESP32DebugInit();
  delay(50);
  pinMode(usb_pd_snk_int_pin, INPUT_PULLUP);  // snk

  pinMode(usb_pd_src1_int_pin, INPUT_PULLUP);  // src1

#ifdef NOA_PD_SNACKER
  NOA_SysPara_init();
  delay(50);

  ncp_bb_con9v_tempadc.attach(ncp_bb_con9v_tempadc_pin);
  digitalWrite(ncp_bb_con9v_led_pin, HIGH);
  
  Wire.begin(26,27, 400000);
  Wire1.begin(21,22, 400000);

  NOA_PUB_I2C_Scanner(0);
  NOA_PUB_I2C_Scanner(1);

  
  sc87xx_drv_set_output_vol(0, SC87XX_OUTPUT_TYPE_9V);

  if (!(rfid_nfc.begin())) {
    ESP_LOGD("main","Unable to initialize the MFRC630. Check wiring?");
  } else {
    ESP_LOGI("main","Initialize the MFRC630 Success!");
    OM9663_fifo_clear_test();
    OM9663_fifo_write_test();
//    OM9663_fifo_read_test();
  }
  Main_Task = xTaskCreateStaticPinnedToCore(start_vTask,
    "startvTask",
    SIZE_OF_STACK,
    ( void * ) NULL,
    tskIDLE_PRIORITY + 1,
    (StackType_t *const)TaskStackBuffer,
    (StaticTask_t *const)&TaskTCBBuffer,
    ARDUINO_RUNNING_CORE);
  if (Main_Task == NULL || &TaskTCBBuffer == NULL) {
    ESP_LOGE("main", "Create Main_Task fail");
  }

  pd_init(0); // init pd snk
//  delay(50);
  vTaskDelay(50/portTICK_PERIOD_MS);
#else  
  pinMode(usb_pd_src3_int_pin, INPUT_PULLUP);  // SRC 3
  pinMode(ncp_bb_con3_int_pin, INPUT_PULLUP);
  pinMode(ncp_bb_con3_en_pin, OUTPUT);
  digitalWrite(ncp_bb_con3_en_pin, LOW);

  pinMode(usb_pd_src3_sel_pin, OUTPUT); // SEL for SRC 3
  digitalWrite(usb_pd_src3_sel_pin, HIGH);

  pinMode(usb_pd_src2_int_pin, INPUT_PULLUP);  // SRC 2
  pinMode(ncp_bb_con2_int_pin, INPUT_PULLUP);
  pinMode(ncp_bb_con2_en_pin, OUTPUT);
  int cc1 = 0, cc2 = 0;
  tcpm_get_cc(2, &cc1, &cc2);
  DBGLOG(Info, "Port %d CC1 %d CC2 %d", 2, cc1, cc2);
  if (cc1 == 0 && cc2 == 0) {
    digitalWrite(ncp_bb_con2_en_pin, LOW);  // when port2 cc1 cc2 is open
  } else {
    digitalWrite(ncp_bb_con2_en_pin, HIGH);
  }

  pinMode(usb_pd_src2_sel_pin, OUTPUT); // SEL for SRC 2
  digitalWrite(usb_pd_src2_sel_pin, HIGH);
  
  Wire.begin(23,18);  // P1 SNK(C 0), P3 UP SRC(C 2)
  Wire.setClock(600000);

  Wire1.begin(21,22); // P2 SRC(C 1) P3 SRC(C 3)
  Wire1.setClock(600000);

  NOA_PUB_I2C_Scanner(0);
  NOA_PUB_I2C_Scanner(1);

  pd_init(0); // init pd snk
  delay(50);

//  Serial.printf(" P1 SNK C0\r\n");
//  NOA_PUB_I2C_PD_RreadAllRegs(0, P1D_ADDR);
//  Serial.printf(" P0 UP SRC C2\r\n");
//  NOA_PUB_I2C_PD_RreadAllRegs(0, PUPD_ADDR);
//  NOA_PUB_I2C_PM_RreadAllRegs(0, PUPM_ADDR);

//  Serial.printf(" P2 SRC C1\r\n");
//  NOA_PUB_I2C_PD_RreadAllRegs(1, P2D_ADDR);
//  NOA_PUB_I2C_PM_RreadAllRegs(1, P2M_ADDR);
//  Serial.printf(" P3 SRC C3\r\n");
//  NOA_PUB_I2C_PD_RreadAllRegs(1, P3D_ADDR);
//  NOA_PUB_I2C_PM_RreadAllRegs(1, P3M_ADDR);
#endif
}

void loop() {
#ifdef NOA_PD_SNACKER
  int reset = 0;
 
  if (LOW == digitalRead(usb_pd_snk_int_pin)) {
    tcpc_alert(0);
//    DBGLOG(Info, "PD init pin LOW");
  } 
  pd_run_state_machine(0, reset);
 
  if (pd_sink_port_ready == 1) {
//    if (wireless_charger_port_ready == 0 && pd_sink_port_default_valtage != 5) {
    if (wireless_charger_port_ready == 0) {
      printf("Wireless charger port is ready!\n");
      sc87xx_drv_set_ce_pin(0, 1);
      wireless_charger_port_ready = 1;
    }
    if (pd_source_port_ready == 0) {
      pd_init(1); // init pd src
      pd_source_port_ready = 1;
    } else {
      if (LOW == digitalRead(usb_pd_src1_int_pin)) {
        tcpc_alert(1);
//      DBGLOG(Info, "PD init pin LOW");
      }
      pd_run_state_machine(1, 0);
    }
//     if (HIGH == digitalRead(ncp_bb_con1_int_pin)) {
// //      DBGLOG(Info, "PM 1 init pin HIGH");
//     } else {
// //      DBGLOG(Info, "PM 1 init pin LOW");
//     }
    
//     if (HIGH == digitalRead(ncp_bb_con9v_int_pin)) {
// //      DBGLOG(Info, "PM wireless init pin HIGH");
//     } else {
// //      DBGLOG(Info, "PM wireless init pin LOW");
//     }

    if (bMain_Task == true) {
      nTime_new = millis()/1000;
      if (nTime_new != nTime_old) {
        NOA_PUB_MSG msg;
        nTime_old = nTime_new;
        NFC_Status_new = radio_mifare1K_dump_minimal();
        if (NFC_Status_new != NFC_Status_old) {
          NFC_Status_old = NFC_Status_new;
          memset(&msg, 0, sizeof(NOA_PUB_MSG));
          if (NFC_Status_old == true) {
            msg.message = NFC_MSG_READY;
          } else {
            msg.message = NFC_MSG_NOTREADY;
            NOA_Parametr_Set(60, (char *)"-");
            NOA_Parametr_Set(61, (char *)"-");
            NOA_Parametr_Set(62, (char *)"-");
            NOA_Parametr_Set(63, (char *)"-");
            NOA_Parametr_Set(64, (char *)"-");
          }
          if (NOA_APP_TASKQUEUE != NULL) {
            xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
          }
        }
        memset(&msg, 0, sizeof(NOA_PUB_MSG));
        if (wireless_charger_port_ready == 1) {
          msg.message = APP_MSG_WIRELESSREADY;
        } else {
          msg.message = APP_MSG_WIRELESSNOTREADY;
        }
        if (NOA_APP_TASKQUEUE != NULL) {
          xQueueSend(NOA_APP_TASKQUEUE, (void *)&msg, (TickType_t)0);
        }
//        DBGLOG(Info, "Temperrature = %d %lu", ncp_bb_con9v_tempadc.readMiliVolts(), ncp_bb_con9v_tempadc.readVoltage());
//        ncp81239_pmic_get_tatus(1);
//        ncp81239_pmic_get_tatus(2);
//       analogReadResolution(12);
//       DBGLOG(Info, "Temperrature = %d", analogRead(ncp_bb_con9v_tempadc_pin));
      }
    }
  }

  // For some reason, a delay of 4 ms seems to be best
  // My guess is that spamming the I2C bus too fast causes problems
  vTaskDelay(1/portTICK_PERIOD_MS);
#else
  int reset = 0;
  if (LOW == digitalRead(usb_pd_snk_int_pin)) {
    tcpc_alert(0);
//    DBGLOG(Info, "PD SNK init pin LOW");
  }  
  pd_run_state_machine(0, reset);
 
  if (pd_sink_port_ready == 1) {
    if (pd_source_port_ready == 0) {
//      delay(1000);
      pd_init(1); // init pd src 1
      delay(50);

      ncp81239_pmic_init(1);
      ncp81239_pmic_set_tatus(1);

      pd_init(2); // init pd src 2
      delay(50);

      int cc1 = 0, cc2 = 0;
      tcpm_get_cc(2, &cc1, &cc2);
      DBGLOG(Info, "Port %d CC1 %d CC2 %d", 2, cc1, cc2);
      if (cc1 == 0 && cc2 == 0) {
        digitalWrite(ncp_bb_con2_en_pin, LOW);  // when port2 cc1 cc2 is open
      } else {
        digitalWrite(ncp_bb_con2_en_pin, HIGH);
      }
      
      ncp81239_pmic_init(2);
      ncp81239_pmic_set_tatus(2);
  
      pd_init(3); // init pd src 3
      delay(50);

      ncp81239_pmic_init(3);
      ncp81239_pmic_set_tatus(3);
//      delay(50);
      pd_source_port_ready = 1;
    } else {
      if (LOW == digitalRead(usb_pd_src1_int_pin)) {
        tcpc_alert(1);
//      DBGLOG(Info, "PD SRC 1 init pin LOW");
      }
      pd_run_state_machine(1, 0);

      if (LOW == digitalRead(usb_pd_src2_int_pin)) {
        tcpc_alert(2);
//      DBGLOG(Info, "PD SRC 2 init pin LOW");
      }
      pd_run_state_machine(2, 0);
    
      if (LOW == digitalRead(usb_pd_src3_int_pin)) {
        tcpc_alert(3);
//      DBGLOG(Info, "PD SRC 3 init pin LOW");
      }
      pd_run_state_machine(3, 0);
    }
  }
  delay(1);
#endif
}

void pd_process_source_cap_callback(int port, int cnt, uint32_t *src_caps)
{
  printf("Port %d HIGH cnt %d %04X: %d mv %d ma\n", port, cnt, *src_caps, ((*src_caps >> 10) & 0x3ff) * 50, (*src_caps & 0x3ff) * 10);
  if (port == 0) {
    pd_sink_port_ready = 1;
    pd_source_cap_max_index = cnt - 1;
    // pd_source_cap_current_index = cnt - 1;
    pd_sink_port_default_valtage = ((*src_caps >> 10) & 0x3ff) * 50;
    pd_sink_port_default_current = (*src_caps & 0x3ff) * 10;
  }
}
