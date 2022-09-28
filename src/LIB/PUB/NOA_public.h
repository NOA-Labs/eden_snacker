/*
  NOA_public.h - NOA public library head file
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file. 
*/
#ifndef __NOA_PUBLIC_H
#define __NOA_PUBLIC_H

#define ESP32DEBUGGING
// #include <ESP32Logger.h>
#include <esp_partition.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NOA_PD_SNACKER
#define NOA_ESP32_PD_VERSION "0.0.1.3"
#define NOA_PD_DEVICENAME "NOA-Snacker-ESP-32"
#else
#define NOA_ESP32_PD_VERSION "0.1.1.2"
#define NOA_PD_DEVICENAME "NOA-Station-ESP-32"
#endif

#define SIZE_OF_TASK_QUEUE  16
#define SIZE_OF_STACK       2048

#define ARDUINO_RUNNING_CORE xPortGetCoreID()

// Task Queue Mesage struct
extern xQueueHandle NOA_APP_TASKQUEUE;
extern xQueueHandle NOA_RGB_TASKQUEUE;
extern xQueueHandle NOA_NET_TASKQUEUE;

typedef struct {
  uint32_t  message;
  uint32_t  param1;
  uint32_t  param2;
  uint32_t  srcTaskId;
} NOA_PUB_MSG;

void NOA_PUB_ESP32DebugInit(void);
void NOA_PUB_Print_Buf_Hex(uint8_t *buf, uint16_t len);
uint16_t NOA_PUB_Swap_hexChar(char *buf, uint8_t *hex, uint16_t len, char fill);
uint32_t NOA_PUB_Swap_charNum(char *buf);

void NOA_PUB_I2C_Scanner(uint8_t nIndex);
void NOA_PUB_I2C_ReceiveBytes(uint8_t nIndex, uint8_t PD_ADDR, uint8_t addr, uint8_t *data, uint16_t length);
void NOA_PUB_I2C_SendBytes(uint8_t nIndex, uint8_t PD_ADDR, uint8_t addr, uint8_t *data, uint16_t length);

void NOA_PUB_I2C_PD_RreadAllRegs(uint8_t nIndex, uint8_t PD_ADDR);
void NOA_PUB_I2C_PM_RreadAllRegs(uint8_t nIndex, uint8_t PD_ADDR);
void NOA_PUB_I2C_PD_Testing(uint8_t nIndex, uint8_t PD_ADDR);
void NOA_PUB_I2C_NFC_RreadAllRegs(uint8_t nIndex, uint8_t NFC_ADDR);

void NOA_PUB_PartScan(esp_partition_type_t part_type);

void memory_init(void *memAddr, int memSize);
void *memory_apply(int size);
void memory_release(void *addr);

#ifdef __cplusplus
}
#endif

#endif
