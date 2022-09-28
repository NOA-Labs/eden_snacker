/*
  NOA_parameter_table.cpp - NOA Parameters Table
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file.
*/
#include "..\..\DRV\PDM\tcpm_driver.h"

#include "NOA_public.h"
#include "NOA_parameter_table.h"

/*****************************************************************************
                            functions
*****************************************************************************/
// Setting function
static int setPara(struct _ParamDescripTable_t *element, void *buf, int *paramLen) {
  *paramLen = strlen((char *)buf);
  memcpy(element->a, buf, strlen((char *)buf));
  return 0;
}

// Getting function
static int getPara(struct _ParamDescripTable_t *element, void *buf, int *paramLen) {
  *paramLen = strlen(element->a);
  memcpy(buf, element->a, strlen(element->a));
  return 0;
}

const DefaultPara_List_t Default_Para[] = {  // "" seting will be canceled when restore to default setting
  // 0 Reserved
  {1,   "",                       },         /* Type     */
  {2,   "",                       },         /* SN       */
  {3,   "NOA FAB",                },         /* Factory Name */
  {4,   "0",                      },         /* Reboot Flag */
  {5,   NOA_ESP32_PD_VERSION,     },         /* Soft Ver */
  {6,   "",                       },         /* Hard Ver */
  {7,   "",                       },         /* Factory Time */
  // 8-10 Reserved
  {11,  "-",                      },         /* AP Load Number */
  // 12-14 Reserved
  {15,  "",                       },         /* STA IP Mode */
  {16,  "0.0.0.0",                },         /* STA IP Addr */
  {17,  "0.0.0.0",                },         /* STA IP SubMask */
  {18,  "0.0.0.0",                },         /* STA IP Gateway */
  {19,  "0.0.0.0",                },         /* STA IP DNS1 */
  {20,  "5001",                   },         /* STA Local Port */
  {21,  "80",                     },         /* STA Remote Sever IP */
  {22,  "0.0.0.0",                },         /* STA IP DNS2 */
  // 23 Reserved
  {24,  "",                       },         /* STA Remote Port */
  {25,  "",                       },         /* TimeZone */
  {26,  "",                       },         /* LocalTime */
  {27,  "-",                      },         /* Network Load(iperf bitrate) */
  {28,  "",                       },         /* PD SNK Voltage */
  {29,  "",                       },         /* PD SNK Current */
  {30,  "",                       },         /* PD SNK Capabilities */
  {31,  "",                       },         /* PD SRC Voltage */
  {32,  "",                       },         /* PD SRC Current */
  {33,  "",                       },         /* PD SRC Capabilities */
  {34,  "9000",                   },         /* Wireless Charge Voltage*/
  {35,  "-",                      },         /* Wireless Charge Temperature */
  {36,  "0",                      },         /* OTA status1 */
  {37,  "0",                      },         /* OTA status2 */
  {38,  "0",                      },         /* OTA file size */
  // 39-40 Reserved
  {41,  "KSK_Eden",               },         /* STA Router SSID */  /* NOARDTest */ /* KSK_Eden */ /* NOA Labs (2.4GHz) */
  {42,  "WPA",                    },         /* STA Router Encryption Mode */
  {43,  "EdenTest123",            },         /* STA Router Key */  /* 12345678 */ /* EdenTest123 */ /* noa-labs.com */
  {44,  "",                       },         /* MAC Of STA */
  {45,  "",                       },         /* RSSI Of STA */
  {46,  "",                       },         /* AP SSID */
  {47,  "87654321",               },         /* AP SSID Passwd */
  {48,  "",                       },         /* Net Status */
  {49,  "-",                      },         /* Active STA SSID */
  {50,  "-",                      },         /* AP Scan */
  {51,  "",                       },         /* FW Release Date */
  {52,  "",                       },         /* FW Release Time */
  {53,  "admin",                  },         /* Web admin */
  {54,  "admin",                  },         /* Web admin passwd */
  {55,  "",                       },         /* RSSI Of AP */
  {56,  "",                       },         /* MAC Of AP */
  {57,  "192.168.88.1",           },         /* AP IP Addr */
  // 58-59 Reserved
  {60,  "-",                      },         /* NFC UUID */
  {61,  "-",                      },         /* NFC Sector0 Addr0 */
  {62,  "-",                      },         /* NFC Sector0 Addr1 */
  {63,  "-",                      },         /* NFC Sector0 Addr2 */
  {64,  "-",                      },         /* NFC Sector0 Addr3 */
  // 65-70 Reserved
  {71,  "",                       },         /* AP Remote Sever IP */
  // 72-73 Reserved
  {74,  "",                       },         /* AP Remote Port */
  // 75-98 Reserved
  {99,  "0",                      },         /* Web Set Status */
};

/* Parameters I/O API */
struct _ParamDescripTable_t NOA_PDT[] = {
  // 0 Reserved
  {1,   "1",                      setPara,      getPara},         /* Type     */
  {2,   "0000000000000000",       setPara,      getPara},         /* SN       */
  {3,   "NOA FAB",                setPara,      getPara},         /* Factory Name */
  {4,   "0",                      setPara,      getPara},         /* Reboot Flag */
  {5,   "5",                      setPara,      getPara},         /* Soft Ver */
  {6,   "6",                      setPara,      getPara},         /* Hard Ver */
  {7,   "0000-00-00 00:00:00",    setPara,      getPara},         /* Factory Time */
  // 8-10 Reserved
  {11,  "11",                     setPara,      getPara},         /* AP Load Number */
  // 10-14 Reserved
  {15,  "DHCP",                   setPara,      getPara},         /* STA IP Mode */
  {16,  "16",                     setPara,      getPara},         /* STA IP Addr */
  {17,  "17",                     setPara,      getPara},         /* STA IP SubMask */
  {18,  "18",                     setPara,      getPara},         /* STA IP Gateway */
  {19,  "19",                     setPara,      getPara},         /* STA IP DNS1 */
  {20,  "5001",                   setPara,      getPara},         /* STA Local Port */
  {21,  "21",                     setPara,      getPara},         /* STA Remote Sever IP */
  {22,  "22",                     setPara,      getPara},         /* STA IP DNS2 */
  // 23 Reserved
  {24,  "24",                     setPara,      getPara},         /* STA Remote Port */
  {25,  "00",                     setPara,      getPara},         /* TimeZone */
  {26,  "0000-00-00 00:00:00",    setPara,      getPara},         /* LocalTime */
  {27,  "27",                     setPara,      getPara},         /* Network Load(iperf bitrate) */
  {28,  "28",                     setPara,      getPara},         /* PD SNK Voltage */
  {29,  "29",                     setPara,      getPara},         /* PD SNK Current */
  {30,  "30",                     setPara,      getPara},         /* PD SNK Capabilities */
  {31,  "31",                     setPara,      getPara},         /* PD SRC Voltage */
  {32,  "32",                     setPara,      getPara},         /* PD SRC Current */
  {33,  "33",                     setPara,      getPara},         /* PD SRC Capabilities */
  {34,  "34",                     setPara,      getPara},         /* Wireless Charge Voltage*/
  {35,  "35",                     setPara,      getPara},         /* Wireless Charge Temperature */
  {36,  "0",                      setPara,      getPara},         /* OTA status1 */
  {37,  "0",                      setPara,      getPara},         /* OTA status2 */
  {38,  "0",                      setPara,      getPara},         /* OTA file size */
  // 39-40 Reserved
  {41,  "NOARDTest",              setPara,      getPara},         /* STA Router SSID */
  {42,  "WPA",                    setPara,      getPara},         /* STA Router Encryption Mode */
  {43,  "12345678",               setPara,      getPara},         /* STA Router Key */
  {44,  "44",                     setPara,      getPara},         /* MAC Of STA */
  {45,  "45",                     setPara,      getPara},         /* RSSI Of STA */
  {46,  "46",                     setPara,      getPara},         /* AP SSID */
  {47,  "47",                     setPara,      getPara},         /* AP SSID Passwd */
  {48,  "48",                     setPara,      getPara},         /* Net Status */
  {49,  "-",                      setPara,      getPara},         /* Active STA SSID */
  {50,  "-",                      setPara,      getPara},         /* AP Scan */
  {51,  "51",                     setPara,      getPara},         /* FW Release Date */
  {52,  "52",                     setPara,      getPara},         /* FW Release Time */
  {53,  "admin",                  setPara,      getPara},         /* Web admin */
  {54,  "admin",                  setPara,      getPara},         /* Web admin passwd */
  {55,  "55",                     setPara,      getPara},         /* RSSI Of AP */
  {56,  "56",                     setPara,      getPara},         /* MAC Of AP */
  {57,  "57",                     setPara,      getPara},         /* AP IP Addr */
  // 58-59 Reserved
  {60,  "60",                     setPara,      getPara},         /* NFC UUID */
  {61,  "61",                     setPara,      getPara},         /* NFC Sector0 Addr0 */
  {62,  "62",                     setPara,      getPara},         /* NFC Sector0 Addr1 */
  {63,  "63",                     setPara,      getPara},         /* NFC Sector0 Addr2 */
  {64,  "64",                     setPara,      getPara},         /* NFC Sector0 Addr3 */
  // 65-70 Reserved
  {71,  "71",                     setPara,      getPara},         /* AP Remote Sever IP */
  // 72-73 Reserved
  {74,  "74",                     setPara,      getPara},         /* AP Remote Port */
  // 75-98 Reserved
  {99,  "0",                      setPara,      getPara},         /* Web Set Status */
};
/*----------------------------------------------------------------------------*
**                             Global Vars                                    *
**----------------------------------------------------------------------------*/
int number_of_array_elements = sizeof(NOA_PDT) / sizeof(NOA_PDT[0]);

