/*
  NOA_App.h - Library for Main APP.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NOA_APP_H
#define NOA_APP_H

#ifdef __cplusplus
extern "C" {
#endif

#define  MSG_ID_USER_APP_START    0x1000

#define  APP_MSG_TIMER_ID             MSG_ID_USER_APP_START+0xFA1
#define  APP_MSG_WDG_ID               MSG_ID_USER_APP_START+0xFA2

#define  APP_MSG_SRCREADY             MSG_ID_USER_APP_START+0xFA3
#define  APP_MSG_SRCNOTREADY          MSG_ID_USER_APP_START+0xFA4
#define  APP_MSG_APREADY              MSG_ID_USER_APP_START+0xFA5
#define  APP_MSG_APNOTREADY           MSG_ID_USER_APP_START+0xFA6
#define  APP_MSG_WIRELESSREADY        MSG_ID_USER_APP_START+0xFA7
#define  APP_MSG_WIRELESSNOTREADY     MSG_ID_USER_APP_START+0xFA8
#define  APP_MSG_WIFIREADY            MSG_ID_USER_APP_START+0xFA9
#define  APP_MSG_WIFINOTREADY         MSG_ID_USER_APP_START+0xFAA
#define  APP_MSG_NFCREADY             MSG_ID_USER_APP_START+0xFAB
#define  APP_MSG_NFCNOTREADY          MSG_ID_USER_APP_START+0xFAC

void NOA_App_init();

#ifdef __cplusplus
}
#endif

#endif /* NOA_APP_H */
