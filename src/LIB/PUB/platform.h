#ifndef __CROS_PLATFORM_H
#define __CROS_PLATFORM_H

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  uint16_t size;
  uint16_t lenght;
  uint8_t *payload;
} Buffer_t;

void platform_usleep(uint64_t us) {
  delayMicroseconds(us);
}

#ifdef __cplusplus
}
#endif

#endif /* __CROS_EC_USB_PD_TCPM_H */
