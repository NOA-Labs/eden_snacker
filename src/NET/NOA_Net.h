/*
  NOA_Net.h - Library for Net APP.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NOA_NET_H
#define NOA_NET_H

#ifdef __cplusplus
extern "C" {
#endif

#define  MSG_ID_USER_NET_START    0x3000

#define  NET_MSG_READY              MSG_ID_USER_NET_START+0xFC1
#define  NET_MSG_NOTREADY           MSG_ID_USER_NET_START+0xFC2
#define  APNET_MSG_READY            MSG_ID_USER_NET_START+0xFC3
#define  APNET_MSG_NOTREADY         MSG_ID_USER_NET_START+0xFC4
#define  APNET_MSG_RECONNECT        MSG_ID_USER_NET_START+0xFC5

void NOA_Net_init();

#ifdef __cplusplus
}
#endif

#endif /* NOA_NET_H */
