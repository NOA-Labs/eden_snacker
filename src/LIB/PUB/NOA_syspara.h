/*
  NOA_syspara.h - NOA system Parameters functions
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file.
*/
#ifndef __NOA_SYSPARA_H_
#define __NOA_SYSPARA_H_

#ifdef __cplusplus
extern "C" {
#endif

/* buf :para read or write char point, return : 0(opt OK)*/
typedef int (*paraFun)(char *buf);

int NOA_SysPara_init(void);
int NOA_SysPara_Get_State(void);

void NOA_Parametr_Get(int number, char *databuf);
int NOA_Parametr_Set(int number, char *data);
void NOA_Parameter_Update(void);

void NOA_Parametr_Default(void);
void NOA_Get_All_Data(void);

#ifdef __cplusplus
}
#endif

#endif  // __NOA_SYSPARA_H_
/************************************************/

