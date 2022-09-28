/*
  NOA_TimeDefs.h - NOA _TIME__ and __DATE__ formatting macros head file
  Copyright 2012 NOA Labs
  Copyright 2021 Mike Mao
  Released under an MIT license. See LICENSE file. 
*/

#ifndef __NOA_TIMEDEFS_H
#define __NOA_TIMEDEFS_H

#ifdef __cplusplus
extern "C" {
#endif

// Example of __DATE__ string: "Jul 27 2012"
//                              01234567890

#define BUILD_DATE_YEAR_CH0 (__DATE__[ 7])
#define BUILD_DATE_YEAR_CH1 (__DATE__[ 8])
#define BUILD_DATE_YEAR_CH2 (__DATE__[ 9])
#define BUILD_DATE_YEAR_CH3 (__DATE__[10])


#define BUILD_DATE_MONTH_IS_JAN (__DATE__[0] == 'J' && __DATE__[1] == 'a' && __DATE__[2] == 'n')
#define BUILD_DATE_MONTH_IS_FEB (__DATE__[0] == 'F')
#define BUILD_DATE_MONTH_IS_MAR (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'r')
#define BUILD_DATE_MONTH_IS_APR (__DATE__[0] == 'A' && __DATE__[1] == 'p')
#define BUILD_DATE_MONTH_IS_MAY (__DATE__[0] == 'M' && __DATE__[1] == 'a' && __DATE__[2] == 'y')
#define BUILD_DATE_MONTH_IS_JUN (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'n')
#define BUILD_DATE_MONTH_IS_JUL (__DATE__[0] == 'J' && __DATE__[1] == 'u' && __DATE__[2] == 'l')
#define BUILD_DATE_MONTH_IS_AUG (__DATE__[0] == 'A' && __DATE__[1] == 'u')
#define BUILD_DATE_MONTH_IS_SEP (__DATE__[0] == 'S')
#define BUILD_DATE_MONTH_IS_OCT (__DATE__[0] == 'O')
#define BUILD_DATE_MONTH_IS_NOV (__DATE__[0] == 'N')
#define BUILD_DATE_MONTH_IS_DEC (__DATE__[0] == 'D')


#define BUILD_DATE_MONTH_CH0 \
((BUILD_DATE_MONTH_IS_OCT || BUILD_DATE_MONTH_IS_NOV || BUILD_DATE_MONTH_IS_DEC) ? '1' : '0')

#define BUILD_DATE_MONTH_CH1 \
( \
(BUILD_DATE_MONTH_IS_JAN) ? '1' : \
(BUILD_DATE_MONTH_IS_FEB) ? '2' : \
(BUILD_DATE_MONTH_IS_MAR) ? '3' : \
(BUILD_DATE_MONTH_IS_APR) ? '4' : \
(BUILD_DATE_MONTH_IS_MAY) ? '5' : \
(BUILD_DATE_MONTH_IS_JUN) ? '6' : \
(BUILD_DATE_MONTH_IS_JUL) ? '7' : \
(BUILD_DATE_MONTH_IS_AUG) ? '8' : \
(BUILD_DATE_MONTH_IS_SEP) ? '9' : \
(BUILD_DATE_MONTH_IS_OCT) ? '0' : \
(BUILD_DATE_MONTH_IS_NOV) ? '1' : \
(BUILD_DATE_MONTH_IS_DEC) ? '2' : \
/* error default */    '?' \
)

#define BUILD_DATE_DAY_CH0 ((__DATE__[4] >= '0') ? (__DATE__[4]) : '0')
#define BUILD_DATE_DAY_CH1 (__DATE__[ 5])



// Example of __TIME__ string: "21:06:19"
//                              01234567

#define BUILD_TIME_HOUR_CH0 (__TIME__[0])
#define BUILD_TIME_HOUR_CH1 (__TIME__[1])

#define BUILD_TIME_MIN_CH0 (__TIME__[3])
#define BUILD_TIME_MIN_CH1 (__TIME__[4])

#define BUILD_TIME_SEC_CH0 (__TIME__[6])
#define BUILD_TIME_SEC_CH1 (__TIME__[7])

#define CHAR_TO_INT(character) ((character) -'0')
#define CHAR_MULTIPLY_TO_INT(character, multiplier) (CHAR_TO_INT(character)*(multiplier))

#define BUILD_TIME_SECONDS_INT (CHAR_MULTIPLY_TO_INT(BUILD_TIME_SEC_CH0,10) + CHAR_MULTIPLY_TO_INT(BUILD_TIME_SEC_CH1,1))
#define BUILD_TIME_MINUTES_INT (CHAR_MULTIPLY_TO_INT(BUILD_TIME_MIN_CH0,10) + CHAR_MULTIPLY_TO_INT(BUILD_TIME_MIN_CH1,1))
#define BUILD_TIME_HOURS_INT (CHAR_MULTIPLY_TO_INT(BUILD_TIME_HOUR_CH0,10) + CHAR_MULTIPLY_TO_INT(BUILD_TIME_HOUR_CH1,1))

#define BUILD_DATE_DAY_INT (CHAR_MULTIPLY_TO_INT(BUILD_DATE_DAY_CH0,10) + CHAR_MULTIPLY_TO_INT(BUILD_DATE_DAY_CH1,1))
#define BUILD_DATE_MONTH_INT (CHAR_MULTIPLY_TO_INT(BUILD_DATE_MONTH_CH0,10) + CHAR_MULTIPLY_TO_INT(BUILD_DATE_MONTH_CH1,1))
#define BUILD_DATE_YEAR_INT (CHAR_MULTIPLY_TO_INT(BUILD_DATE_YEAR_CH0,1000) + CHAR_MULTIPLY_TO_INT(BUILD_DATE_YEAR_CH1,100) +  \
                             CHAR_MULTIPLY_TO_INT(BUILD_DATE_YEAR_CH2,10) + CHAR_MULTIPLY_TO_INT(BUILD_DATE_YEAR_CH3,1))

#ifdef __cplusplus
}
#endif

#endif
