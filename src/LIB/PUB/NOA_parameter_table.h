/*
  NOA_parameter_table.h - NOA Parameters Table
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file.
*/
#ifndef __NOA_PUB_PARAMETER_TABLE_H__
#define __NOA_PUB_PARAMETER_TABLE_H__

#ifdef __cplusplus
extern "C" {
#endif

#define NOA_PARAM_LEN  256
typedef struct
{
    int num;
    char para[NOA_PARAM_LEN];
}DefaultPara_List_t;

typedef struct _ParamDescripTable_t{
    int num;
    char a[NOA_PARAM_LEN];
    int (*wFunc)(struct _ParamDescripTable_t *, void *, int *);
    int (*rFunc)(struct _ParamDescripTable_t *, void *, int *);
}name;

extern int number_of_array_elements;
extern  struct _ParamDescripTable_t NOA_PDT[];
extern const DefaultPara_List_t Default_Para[];

#ifdef __cplusplus
}
#endif

#endif //__NOA_PUB_PARAMETER_TABLE_H__
