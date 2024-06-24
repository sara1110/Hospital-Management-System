#include "include\txtdatabase.h"

// Takes an EntryType and converts it into a formatted string,
// which can be taken by standard library functions.
// Example output: "%d %d %31s %31s %d"
int generateEntryParseString(char* targetStr, EntryAttributes eType) {
    
    if(NULL==targetStr 
    || NULL==eType.attribArr 
    || eType.nAttribs<=0) {
        return -1;
    }

    int strOffset = 0;

    for(int atr=0; atr<eType.nAttribs; atr++) {
        enum nativeTypeEnum currentAttrib = eType.attribArr[atr].datatype;
        switch(currentAttrib) {
            case INT: {
                char intFormat[] = " %d";
                strncpy(targetStr+strOffset, intFormat, sizeof(intFormat)-1);
                strOffset+=sizeof(intFormat)-1;
                break;
            }
            case CHAR: {
                char charFormat[] = " %c";
                strncpy(targetStr+strOffset, charFormat, sizeof(charFormat)-1);
                strOffset+=sizeof(charFormat)-1;
                break;
            }
            case STR: {
                char stringFormat[] = " %s";
                //sprintf(stringFormat, " %s");
                strncpy(targetStr+strOffset, stringFormat, sizeof(stringFormat)-1);
                strOffset+=sizeof(stringFormat)-1;
                break;
            }
            case FLOAT: {
                char floatFormat[] = " %f";
                strncpy(targetStr+strOffset, floatFormat, sizeof(floatFormat)-1);
                strOffset+=sizeof(floatFormat)-1;
                break;
            }
            case DOUBLE: {
                char doubleFormat[] = " %lf";
                strncpy(targetStr+strOffset, doubleFormat, sizeof(doubleFormat)-1);
                strOffset+=sizeof(doubleFormat)-1;
                break;
            }
            default: {
                break;
            }
        }
    }
}

/*
GETALL_DATABASE_DECL(Patient)
GETALL_DATABASE_DEF (Patient);

GET_FROM_DATABASE_DECL(Patient)
GET_FROM_DATABASE_DEF(Patient);

SET_TO_DATABASE_DECL(Patient)
SET_TO_DATABASE_DEF(Patient);

Patient tstPatient;
EntryAttributes attribs;

PRINT_ENTRY_DECL(Patient)
PRINT_ENTRY_DEF(tstPatient, attribs);
*/
