/*
  NOA_NFC.h - Library for interacting NFC chip.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NOA_NFC_H
#define NOA_NFC_H

#ifdef __cplusplus
extern "C" {
#endif

#define  MSG_ID_USER_NFC_START    0x4000

#define  NFC_MSG_READY             MSG_ID_USER_NFC_START+0xFD1
#define  NFC_MSG_NOTREADY          MSG_ID_USER_NFC_START+0xFD2
#define  NFC_MSG_CLEAR             MSG_ID_USER_NFC_START+0xFD3

// void NOA_NFC_init();

void OM9663_fifo_read_test(void);
void OM9663_fifo_write_test(void);
void OM9663_fifo_clear_test(void);
void OM9663_status_test(void);

bool radio_mifare1K_dump_minimal(void);
bool radio_ntag156b_dump_minimal(void);
void radio_iso1443A_106_scan();

#ifdef __cplusplus
}
#endif

#endif /* NOA_NFC */
