/*
  NEOPixel.cpp - Library for interacting with the Adafruit_NeoPixel library.
  Copyright 2021 NOA
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/
#include <Esp.h>
#include <Adafruit_NeoPixel.h>

#include "..\DRV\PDM\usb_pd.h"
#include "..\DRV\PDM\sc8721xx_drv.h"
#include "..\LIB\PUB\NOA_public.h"
#include "..\APP\NOA_App.h"

#include "NEOPixel.h"

static const char* TAG = "neo_pixel";
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1
const int neo_pixel_rgb_pin = 19;

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS      5
// static int nStatus_SRC = 0, nStatus_WIFI_AP = 0, nStatus_Wireless = 0, nStatus_WIFI = 0, nStatus_NFC = 0;
static int nStatus[NUMPIXELS] = {0};
// When we setup the NeoPixel library, we tell it how many pixels, and which pin to use to send signals.
// Note that for older NeoPixel strips you might need to change the third parameter--see the strandtest
// example for more information on possible values.
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, neo_pixel_rgb_pin, NEO_GRB + NEO_KHZ800);

// int delayval = 5; // delay for half a second
int delayval = 1;

xQueueHandle NOA_RGB_TASKQUEUE = NULL;

TaskHandle_t RGB_Light_Task = NULL;
StaticTask_t xTaskBuffer_RGB_Light;
StackType_t xStack_RGB_Light[SIZE_OF_STACK];

TaskHandle_t RGB_Light_Test_Task = NULL;
StaticTask_t xTaskBuffer_RGB_Light_Test;
StackType_t xStack_RGB_Light_Test[SIZE_OF_STACK] = {0};

static int nStatus_RGB_Testing = 0;

// extern StructNCP81239RegisterMap g_stPMICData[CONFIG_NCP_PM_PORT_COUNT];
extern uint8_t nAP_Sta_Numbers;
//****************************************************************************
// CODE TABLES
//****************************************************************************
void RGB_Light_Test_Task_Loop( void * pvParameters ){
  ESP_LOGI(TAG, "RGB_Light_Test_Task_Loop running on core %d", xPortGetCoreID());
  NEO_Pixel_Clear();

  NEO_Pixel_Simple_Test();
//  rainbow(20);
  rainbowCycle(20);
//  theaterChaseRainbow(50);
//  NEO_Pixel_Clear();
  
  ESP_LOGI(TAG, "RGB_Light_Test_Task_Loop Exit from core %d", xPortGetCoreID());

//  uint32_t msg = 0;
//  msg = RGB_MSG_READY;
  NOA_PUB_MSG msg;
  memset(&msg, 0, sizeof(NOA_PUB_MSG));
  msg.message = RGB_MSG_READY;
  if (NOA_APP_TASKQUEUE != NULL) {
    xQueueSend(NOA_APP_TASKQUEUE, &msg, portMAX_DELAY);
  }
  nStatus_RGB_Testing = 1;
  vTaskDelete(NULL);
}

// RGB_Light_Task_Loop: blinks an RGB LED
void RGB_Light_Task_Loop( void * pvParameters ){
  ESP_LOGI(TAG, "RGB_Light_Task_Loop running on core %d", xPortGetCoreID());
//  const TickType_t xTicksToWait = pdMS_TO_TICKS(50);
  BaseType_t xStatus = 0;
//  uint32_t msg;
  NOA_PUB_MSG msg;

  NEO_Pixel_Clear();
  static bool toggle_status = false;

  while(true){
//    xStatus = xQueueReceive(NOA_RGB_TASKQUEUE, &msg, xTicksToWait);  //Get message from the queue
//    msg = 0;
    memset(&msg, 0, sizeof(NOA_PUB_MSG));
    xStatus = xQueueReceive(NOA_RGB_TASKQUEUE, &msg, portMAX_DELAY);
    if (xStatus == pdPASS) {
//      ESP_LOGI(TAG, "RGB_Light_Task_Loop get a message %d QueueSpaces %d", msg.message, uxQueueSpacesAvailable(NOA_RGB_TASK));
    }
    switch(msg.message) {
      case RGB_MSG_READY:
        break;
      case APP_MSG_SRCREADY:
//        ESP_LOGI(TAG, "SRC port voltage = %d", g_stPMICData[1].ucCR01DacTarget);
        // if (g_stPMICData[1].ucCR01DacTarget > 10 && g_stPMICData[1].ucCR01DacTarget <= 50) {
        //   nStatus[0] = 1;
        // }
        // if (g_stPMICData[1].ucCR01DacTarget > 50 && g_stPMICData[1].ucCR01DacTarget <= 90) {
        //   nStatus[0] = 3;
        // }
        // if (g_stPMICData[1].ucCR01DacTarget > 90 && g_stPMICData[1].ucCR01DacTarget <= 120) {
        //   nStatus[0] = 4;
        // }
        // if (g_stPMICData[1].ucCR01DacTarget > 120 && g_stPMICData[1].ucCR01DacTarget <= 150) {
        //   nStatus[0] = 5;
        // }
        // if (g_stPMICData[1].ucCR01DacTarget > 150 && g_stPMICData[1].ucCR01DacTarget <= 200) {
        //   nStatus[0] = 2;
        // }
        // if (g_stPMICData[1].ucCR01DacTarget > 200) {
        //   nStatus[0] = 6;
        // }
        break;
      case APP_MSG_SRCNOTREADY:
        nStatus[0] = 0;
        break;
      case APP_MSG_APREADY:
        if (nAP_Sta_Numbers > 0) {
          nStatus[1] = 7;
        } else {
          nStatus[1] = 1;
        }
        break;
      case APP_MSG_APNOTREADY:
        nStatus[1] = 0; 
        break;
      case APP_MSG_WIRELESSREADY:
        nStatus[2] = 3; // default is 9V
        break;
      case APP_MSG_WIRELESSNOTREADY:
        nStatus[2] = 0;
        break;
      case APP_MSG_WIFIREADY:
        nStatus[3] = 1;
        break;
      case APP_MSG_WIFINOTREADY:
        nStatus[3] = 0;
        break;
      case APP_MSG_NFCREADY:
        nStatus[4] = 1;
        break;
      case APP_MSG_NFCNOTREADY:
        nStatus[4] = 0;
        break;
      case APP_MSG_TIMER_ID:
        if (nStatus_RGB_Testing == 0) {
          break;
        }
        toggle_status = !toggle_status;
//        ESP_LOGI(TAG, "RGB task APP_MSG_TIMER_ID StackSize %ld", uxTaskGetStackHighWaterMark(NULL));
        if (toggle_status == true) {
          NEO_Pixel_Status();
        } else {
          NEO_Pixel_Clear();
        }
        break;
      default:
        break;
    }
  }
  ESP_LOGI(TAG, "RGB_Light_Task_Loop Exit from core %d", xPortGetCoreID());
  vTaskDelete(NULL);
}

void NEO_Pixel_init() {
  nStatus_RGB_Testing = 0;
  pixels.begin();
//  pixels.setBrightness(10); // set bright down for debug

// Create the task without using any dynamic memory allocation.
  RGB_Light_Test_Task = xTaskCreateStaticPinnedToCore(
                   RGB_Light_Test_Task_Loop,       // Function that implements the task.
                   "RGBLightTest",          // Text name for the task.
                   SIZE_OF_STACK,           // Stack size in bytes, not words.
                   NULL,                    // Parameter passed into the task.
                   tskIDLE_PRIORITY + 1,    // Priority at which the task is created.
                   xStack_RGB_Light_Test,          // Array to use as the task's stack.
                   &xTaskBuffer_RGB_Light_Test,   // Variable to hold the task's data structure.
                   ARDUINO_RUNNING_CORE);
  if (RGB_Light_Test_Task == NULL || &xTaskBuffer_RGB_Light_Test == NULL) {
    ESP_LOGE(TAG, "Create RGB_Light_Test_Task fail");
  }

//  NOA_RGB_TASKQUEUE = xQueueCreate(SIZE_OF_TASK_QUEUE, sizeof(uint32_t));
  NOA_RGB_TASKQUEUE = xQueueCreate(SIZE_OF_TASK_QUEUE, sizeof(NOA_PUB_MSG));
  if (NOA_RGB_TASKQUEUE == NULL) {
    ESP_LOGE(TAG, "Create NOA_RGB_TASKQUEUE fail");
  }

  RGB_Light_Task = xTaskCreateStaticPinnedToCore(
                   RGB_Light_Task_Loop,       // Function that implements the task.
                   "RGBLightTask",          // Text name for the task.
                   SIZE_OF_STACK,           // Stack size in bytes, not words.
                   NULL,                    // Parameter passed into the task.
                   tskIDLE_PRIORITY + 1,    // Priority at which the task is created.
                   xStack_RGB_Light,          // Array to use as the task's stack.
                   &xTaskBuffer_RGB_Light,    // Variable to hold the task's data structure.
                   ARDUINO_RUNNING_CORE);
  if (RGB_Light_Task == NULL || &xTaskBuffer_RGB_Light == NULL) {
    ESP_LOGE(TAG, "Create RGB_Light_Task fail");
  }
}

void NEO_Pixel_Simple_Test() {
  for (int j = 1; j <= pixels.numPixels(); j++) {
    for(int i=0; i < j; i++){
      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Moderately bright red color.
      pixels.show(); // This sends the updated pixel color to the hardware.
      vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
    }
    vTaskDelay(500);
  }
  // For a set of NeoPixels the first NeoPixel is 0, second is 1, all the way up to the count of pixels minus one.
//  for(int i=0; i < pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
//    pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Moderately bright red color.
//    pixels.show(); // This sends the updated pixel color to the hardware.
//    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
//  }
//  vTaskDelay(500);
  for(int i=0; i < pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 255, 0)); // Moderately bright green color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
  }
  vTaskDelay(500);
  for(int i=0; i < pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright blue color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
  }
  vTaskDelay(500);
  for(int i=0; i < pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Moderately bright white color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
  }
  vTaskDelay(500);

  colorWipe(pixels.Color(255, 0, 0), 50); // Red
  colorWipe(pixels.Color(0, 255, 0), 50); // Green
  colorWipe(pixels.Color(0, 0, 255), 50); // Blue
  colorWipe(pixels.Color(255, 255, 255), 50); // White

  // Send a theater pixel chase in...
  theaterChase(pixels.Color(127, 127, 127), 50);  // White
  theaterChase(pixels.Color(127, 0, 0), 50);      // Red
  theaterChase(pixels.Color(0, 127, 0), 50);      // Green
  theaterChase(pixels.Color(0, 0, 127), 50);      // Blue
}

void NEO_Pixel_Clear() {
//  colorWipe(pixels.Color(255, 255, 255), 50); // White

  for(int i=0; i<pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright black color.
    pixels.show(); // This sends the updated pixel color to the hardware.
    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
  }
}

void NEO_Pixel_Status() {
  for(int i=0; i<pixels.numPixels(); i++){
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
/*    if (nStatus[i] == 0) {
      pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright black color.
    } else {
      pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Moderately bright black color.
    } */
    switch(nStatus[i]) {
      case 0:
        pixels.setPixelColor(i, pixels.Color(0, 0, 0)); // Moderately bright black color.
        break;
      case 2:
        pixels.setPixelColor(i, pixels.Color(255, 0, 0)); // Moderately bright red color.
        break;
      case 3:
        pixels.setPixelColor(i, pixels.Color(255, 255, 0)); // Moderately bright yellow color.
        break;
      case 4:
        pixels.setPixelColor(i, pixels.Color(255, 165, 0)); // Moderately bright orange color.
        break;
      case 5:
        pixels.setPixelColor(i, pixels.Color(255, 69, 0)); // Moderately bright orangered color.
        break;
      case 6:
        pixels.setPixelColor(i, pixels.Color(255, 0, 255)); // Moderately bright fushsia color.
        break;
      case 7:
        pixels.setPixelColor(i, pixels.Color(0, 0, 255)); // Moderately bright blue color.
        break;
      case 1:
      default:
        pixels.setPixelColor(i, pixels.Color(255, 255, 255)); // Moderately bright white color.
//        pixels.setPixelColor(i, pixels.Color(255, 215, 0)); // Moderately bright gold color.
        break;
    }
    pixels.show(); // This sends the updated pixel color to the hardware.
    vTaskDelay(delayval); // Delay for a period of time (in milliseconds).
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<pixels.numPixels(); i++) {
    pixels.setPixelColor(i, c);
    pixels.show();
    vTaskDelay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j < 256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel((i+j) & 255));
    }
    pixels.show();
    vTaskDelay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

//  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
  for(j=0; j<256; j++) {
    for(i=0; i<pixels.numPixels(); i++) {
      pixels.setPixelColor(i, Wheel(((i * 256 / pixels.numPixels()) + j) & 255));
    }
    pixels.show();
    vTaskDelay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q<3; q++) {
      for (uint16_t i=0; i<pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, c);    //turn every third pixel on
      }
      pixels.show();

      vTaskDelay(wait);

      for (uint16_t i=0; i<pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j<256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q<3; q++) {
      for (uint16_t i=0; i<pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      pixels.show();

      vTaskDelay(wait);

      for (uint16_t i=0; i<pixels.numPixels(); i=i+3) {
        pixels.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return pixels.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return pixels.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return pixels.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
