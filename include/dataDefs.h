#pragma once
#ifndef DATADEFS_H
#define DATADEFS_H

// The maximum amount of entries displayed at once on screen
// when showing the full content of a database
#define MAXENTRIESDISPLAY 10
#define DEFAULTSEPARATORLENGTH 32

// Error message macros 
#define INVALIDCHOICEMSG "Invalid choice, please try again..."
#define INVALIDINPUTMSG "Invalid input, please try again..."

// --- User representation data ---

typedef struct patientStrc {
    int pkID;
    //PersonalID ID;
    char personalID[50];
    char name[50];
    char password[50];
    int isLocal;
} Patient;

typedef struct doctorStrc
{
    int doctorID;
    char personalID[50];
    char name[50];
    char password[50];
    // Later use this for storing the area(s) in which this doctor operates
    char specializationsStr[256];
} Doctor;

typedef struct staffNurseStrc
{
    //PersonalID ID;
    int staffNurseID;
    char personalID[50];
    char name[50];
    char password[50];
    char functionalUnitStr[256];
} StaffNurse;

// This allows compatibility to store
// patients, doctors and staff nurses in a
// single User variable / array
typedef union userUnion
{
    Patient patient;
    Doctor doctor;
    StaffNurse staffNurse;
} User;

//------------------------------------

// --- Schedule / Appointment data --- 

// Date represented using characters
// 8 bytes of depth (without padding) 
typedef struct charDateStrc {
    char dayDigits[2]; // 01 to 31
    char monthDigits[2]; // 01 to 12
    char yearDigits[4]; // 1 to 9999
} CharDate;

// Date represented using unsigned integers
// ? bytes of depth (without padding)
typedef struct uintDateStrc {
    unsigned short day; // uint? for days
    unsigned short month; // uint? for months
    unsigned int year; // uint? for years
} UintDate;

// Final date union
typedef union dateUnion {
    CharDate dateInChars;
    UintDate dateInUints;
} Date;

typedef struct charDayTimeStrc {
    char seconds[2]; // 00 - 59
    char minutes[2]; // 00 - 59
    char hours[2]; // 00 - 23
} CharDayTime;

typedef struct uintDayTimeStrc {
    unsigned char seconds; // uint8 for seconds
    unsigned char minutes; // uint8 for minutes
    unsigned char hours; // uint 8 for hours
} UintDayTime;

typedef union dayTimeUnion {
    CharDayTime dayTimeInChars;
    UintDayTime dayTimeInUints;
} DayTime;

typedef struct appointmentStrc {
    int appointmentID; // primary key
    int patientID; // foreign key to Patients DB
    int doctorID; // foreign key to Doctors DB
    Date appointmentDay;
    DayTime startTime; 
} Appointment;

//------------------------------------

//-------- EHR Data --------//
typedef struct EHRStrc {
    int ehrID;
    int patientID;
    char medicalHistory[100];
    char prescriptions[150];
    //char billingInformation[100];
    char allergies[100];
} EHR;
//------------------------------------

//------- Bill Data --------//
typedef struct BillStrc {
    int billID;
    int patientID;
    //char invoiceDate[10];
    Date invoiceDate;
    char serviceProvider[32];
    float amountDue;
    //float amountBreakdown;
} Bill;

//------------------------------------

// --- Inventory data ---
// Represents a kind of Item and its quantity
typedef struct invItemStrc
{
    int invItemID;
    char name[64];
    char description[256];
    char category[64];
    int quantity;
} InventoryItemType;

//------------------------------------

// This struct stores references to all
// state variables/data 
typedef struct sysStateStrc
{
    User currentlyLoggedUser;
    char adminPassw[64];
} SystemState;

// ############################

#endif // DATADEFS_H