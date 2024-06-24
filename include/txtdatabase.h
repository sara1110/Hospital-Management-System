#pragma once
#ifndef HOSPSYS_DATABASE_H
#define HOSPSYS_DATABASE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "include\dataDefs.h"

// Enumeration representing native datatypes
enum nativeTypeEnum {
    INT=0,
    CHAR,
    STR,
    FLOAT,
    DOUBLE
};

// Represents a particular datatype used by 
// an attribute.
// i.e.: int, char, char[], float
typedef struct dataTypeStrc {
    enum nativeTypeEnum datatype;
    int arrSize; // set to 1 for a non-array type
} DataType;

// Represents the entry/entity format
// of a particular database.
// i.e.: Patient, Doctor, StaffNurse.
typedef struct entryAttributesStrc {
    DataType* attribArr; // Buffer of attribute datatypes
    int nAttribs; // Number of attributes (size of attribArr)
} EntryAttributes;

// Metadata representing a particular database 
typedef struct dbMetadataStrc {
    int nEntries, nOrphanIDs;
    int *orphanIDs;
    char *parseString; // provided by generateEntryParseString()
    char *filePath;  
} DBmetadata;

// - Takes an EntryType and converts it into a formatted string,
// which can be taken by standard library functions.
// - Example output: "%d %d %31s %31s %d"
// - Returns -1 if errors | 0 if no errors
int generateEntryParseString(char* targetStr, EntryAttributes eType);

// - Database serialization and deserialization macros
#define SERIALIZE_DATABASE_DECL(EntryStrcType) EntryStrcType* serialize##EntryStrcType##Database()

// Database operation macros: getAll, add, set, get, delete
#define GETALL_DATABASE_DECL(EntryStrcType) EntryStrcType getAll##EntryStrcType##s(int* nEntries)
#define GET_FROM_DATABASE_DECL(EntryStrcType) EntryStrcType get##EntryStrcType(int entryID) 
#define ADD_TO_DATABASE_DECL(EntryStrcType) int add##EntryStrcType(EntryStrcType newEntry)
#define SET_TO_DATABASE_DECL(EntryStrcType) int set##EntryStrcType(int entryID, EntryStrcType new##EntryStrcType) 
#define DELETE_FROM_DATABASE_DECL(EntryStrcType) int delete##EntryStrcType(int entryID)

#define GETALL_DATABASE_DEF(EntryStrcType) \
{ } \

#define GET_FROM_DATABASE_DEF(EntryStrcType) \
{ }\

#define ADD_TO_DATABASE_DEF(EntryStrcType) \
{ } \

#define SET_TO_DATABASE_DEF(EntryStrcType) \
{ } \

#define REMOVE_FROM_DATABASE_DEF(EntryStrcType) \
{ } \

// Search / sort operation macros

// Print entry macro
#define PRINT_ENTRY_DECL(EntryStrcType) void print##EntryStrcType(EntryStrcType entryData)
#define PRINT_ENTRY_DEF(EntryStrcType, EntryAttributes)\
void print##EntryStrcType(EntryStrcType entryData)\
{\
    if(0 > entryData.pkID) {\
        printf("|< NON EXISTANT ENTRY >|\n");\
        return;\
    } \
    unsigned char* attribPointer = (unsigned char*) &entryData;     /*This points to current attribute in the struct*/ \
    for(int i=0; i < EntryAttributes.nAttribs; i++) {\
        printf("| ");\
        switch(EntryAttributes.nAttribs[i].datatype){\
        case INT: {\
            printf("");\
            atribPointer+=sizeof(int);\
            break;\
            }\
        case CHAR: {\
            atribPointer+=sizeof(char);\
            break;\
        }\
        case STR: {\
            printf("");\
            break;\
        }\
        case FLOAT: {\
            break;\
        }\
        default:\
            break;\
        }\
    }\
}\

#endif // HOSPSYS_DATABASE_H