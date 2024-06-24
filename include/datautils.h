#pragma once
#ifndef DATAUTILS_H
#define DATAUTILS_H

#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"

//##############################//
//###### STRING UTILITIES ######//
//##############################//

int isAlphaOnlyString(char *str);
int isNumberOnlyString(char *str);
int isAlphanumericOnlyString(char *str);
int strHasSpecialChars(char *str);
int isBoolNum(int i);

// Replaces chars equal to 'oldChar' by 'newChar' in 'str'
// Warning: this function manipulates the string data provided
void replaceCharBy(char *str, char oldChar, char newChar);

// Sets all the characters of the string
// to white space
void clearStr(char *str);

//############################//
//###### DATE UTILITIES ######//
//############################//

// Returns 0 if both dates are equal, 1
// if they differ
int datesAreEqual(Date a, Date b);

// Copies the contents of *src into *dst 
void dateCopy(Date *dst, Date *src);

// Date to string conversion
void dateToStr(char *dst, Date *src);

// String to Date conversion
void strToDate(Date *dst, char *src);

//#################################//
//###### DAY TIME UTILITIES #######//
//#################################//

// Returns 0 if both day times are equal, 1
// if they differ
int dayTimesAreEqual(DayTime a, DayTime b);

// Copies the contents of *src into *dst 
void dayTimeCopy(DayTime *dst, DayTime *src);

// DayTime to string conversion 
void dayTimeToStr(char *dst, DayTime *src);

// String to DayTime conversion
void strToDayTime(DayTime *dst, char *src);

#endif // DATAUTILS_H