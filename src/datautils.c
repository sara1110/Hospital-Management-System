#include "include\datautils.h"

int isAlphaOnlyString(char *str) {
    int ci=0;
    int nonAlphaFound = 0;
    while('\0'!=str[ci]) {
        if(
        !( (str[ci] >= 'a' && str[ci] <= 'z')
        || (str[ci] >= 'A' && str[ci] <= 'Z')
        || str[ci] == ' ')
        ) {
            nonAlphaFound=1;
            break;
        }  
        ci++;
    }    

    return !nonAlphaFound;
}

int isNumberOnlyString(char *str) {
    int ci=0;
    int nonNumberFound = 0;
    while('\0'!=str[ci]) {
        if(!(
            (str[ci] >= '0' && str[ci] <= '9') ||
            ' ' == str[ci]
            )) {
            nonNumberFound=1;
            break;
        }
        ci++;
    }

    return !nonNumberFound;
}

int isAlphanumericOnlyString(char *str) {
    int ci=0;
    int nonAlphanumericFound=0;
    while('\0'!=str[ci]) {
        if(!(
            (str[ci] >= 'a' && str[ci] <= 'z') ||
            (str[ci] >= 'A' && str[ci] <= 'Z') ||
            (str[ci] >= '0' && str[ci] <= '9') ||
            ' ' == str[ci]
        )) {
            nonAlphanumericFound=1;
            break;
        }
        ci++;
    }

    return !nonAlphanumericFound;
}

int isBoolNum(int i) {
    return (0==i || 1==i);
}

// Replaces chars equal to 'oldChar' by 'newChar' in 'str'
// Warning: this function manipulates the string data provided
void replaceCharBy(char *str, char oldChar, char newChar) {
    if(NULL==str) return;
    
    int i = 0;
    char ci = str[0];
    while('\0'!=ci) {        
        if(str[i] == oldChar) {
            str[i]=newChar;
        }

        i++;
        ci=str[i];
    }
}

// Sets all the characters of the string
// to white space
void clearStr(char *str) {
    int i=0;
    while('\0' != str[i]) {
        str[i]=' ';
        i++;
    }
}

// Returns 0 if both dates are equal, 1
// if they differ
int datesAreEqual(Date a, Date b) {
    
    // Since this returns 1 when dates are equal, toggle
    return !(
        // for each strcmp, equal strings returns 0, thus we need toggle as well
        !strcmp(a.dateInChars.dayDigits, b.dateInChars.dayDigits) &&
        !strcmp(a.dateInChars.monthDigits, b.dateInChars.monthDigits) &&
        !strcmp(a.dateInChars.yearDigits, b.dateInChars.yearDigits)
    );
}

// Copies the contents of *src into *dst 
void dateCopy(Date *dst, Date *src) {
    strcpy(dst->dateInChars.dayDigits, src->dateInChars.dayDigits);
    strcpy(dst->dateInChars.monthDigits, src->dateInChars.monthDigits);
    strcpy(dst->dateInChars.yearDigits, src->dateInChars.yearDigits);
}

// Date to string conversion
void dateToStr(char *dst, Date *src) {
    strncpy(&(dst[0]), src->dateInChars.dayDigits, 2);
    strncpy(&(dst[2]), src->dateInChars.monthDigits, 2);
    strncpy(&(dst[4]), src->dateInChars.yearDigits, 4);
}

// String to Date conversion
void strToDate(Date *dst, char *src) {
    strncpy(dst->dateInChars.dayDigits, &(src[0]), 2);
    strncpy(dst->dateInChars.monthDigits, &(src[2]), 2);
    strncpy(dst->dateInChars.yearDigits, &(src[4]), 4);
}

// Returns 0 if both day times are equal, 1
// if they differ
int dayTimesAreEqual(DayTime a, DayTime b) {
    return !(
        !strcmp(a.dayTimeInChars.hours, b.dayTimeInChars.hours) &&
        !strcmp(a.dayTimeInChars.minutes, b.dayTimeInChars.minutes) &&
        !strcmp(a.dayTimeInChars.seconds, b.dayTimeInChars.seconds)
    );
}

// Copies the contents of *src into *dst 
void dayTimeCopy(DayTime *dst, DayTime *src) {
    strcpy(dst->dayTimeInChars.hours, src->dayTimeInChars.hours);
    strcpy(dst->dayTimeInChars.minutes, src->dayTimeInChars.minutes);
    strcpy(dst->dayTimeInChars.seconds, src->dayTimeInChars.seconds);
}

// DayTime to string conversion 
void dayTimeToStr(char *dst, DayTime *src) {
    strncpy(&(dst[0]), src->dayTimeInChars.hours, 2);
    strncpy(&(dst[2]), src->dayTimeInChars.minutes, 2);
    strncpy(&(dst[4]), src->dayTimeInChars.seconds, 2);
}

// String to DayTime conversion
void strToDayTime(DayTime *dst, char *src) {
    strncpy(dst->dayTimeInChars.hours, &(src[0]), 2);
    strncpy(dst->dayTimeInChars.minutes, &(src[2]), 2);
    strncpy(dst->dayTimeInChars.seconds, &(src[4]), 2);
}
