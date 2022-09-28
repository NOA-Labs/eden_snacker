/*
  NEOPixel.h - Library for interacting with the Adafruit_NeoPixel library.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#ifdef __cplusplus
extern "C" {
#endif

#define  MSG_ID_USER_RGB_START    0x2000

#define  RGB_MSG_READY          MSG_ID_USER_RGB_START+0xFB1
#define  RGB_MSG_CLEAR          MSG_ID_USER_RGB_START+0xFB2

void NEO_Pixel_init();
void NEO_Pixel_Simple_Test();
void NEO_Pixel_Clear();
void NEO_Pixel_Status();
uint32_t Wheel(byte WheelPos);
void rainbow(uint8_t wait);
void rainbowCycle(uint8_t wait);
void theaterChaseRainbow(uint8_t wait);
void colorWipe(uint32_t c, uint8_t wait);
void theaterChase(uint32_t c, uint8_t wait);

#ifdef __cplusplus
}
#endif

#endif /* NEOPIXEL_H */
