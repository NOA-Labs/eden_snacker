/*
  NOA_WebServer.h - Library for Web Server.
  Copyright 2012 NOA Labs
  Copyright 2021 Mike mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef NOA_WEB_SERVER_H
#define NOA_WEB_SERVER_H

#ifdef __cplusplus
extern "C" {
#endif

void NOA_WebServer_init();
void NOA_UpdateParaJson();
void NOA_UpdateAPListJson();

#ifdef __cplusplus
}
#endif

#endif /* NOA_WEB_SERVER_H */
