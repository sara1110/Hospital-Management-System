#pragma once
#ifndef DOCTOR_DB
#define DOCTOR_DB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"
#include "include\metadataMng.h"

#define DOCTORSSTXT_PATH "data\\doctors.txt"
#define DOCTORSMETATXT_PATH "data\\doctorsMeta.txt"

// - Loads the contents of doctor txt file to 
// a memory buffer
// - Returns a dynamically allocated Doctor array,
// which should be deallocated later.
// - nDoctorsOut will receive the number of doctors present
// in the database.
Doctor *deserializeDoctorDB(int *nDoctorsOut, DBMetadata *metadata);

// - Rewrites the file containing the doctor entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeDoctorDB(Doctor *doctors, int nDoctors, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the doctor entries.
// - Writes number of entries to nEntries ptr
Doctor *getAllDoctors(int *nEntries);

// Loads the value of a doctor entry
Doctor getDoctor(int doctorID);

// - Sets the value of an existing doctor entry
// - Returns -1 if an error ocurred
int setDoctor(int doctorID, Doctor newDoctorData);

// - Adds a new doctor entry
// - Returns the doctor ID of the new doctor entered
int addDoctor(Doctor newDoctorData);

// - Removes a doctor entry
// - Returns -1 if error
int removeDoctor(int doctorID);

// - Uses Linear (naive) search to find a doctor entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchDoctorByID(int doctorID, int nEntries, Doctor* doctors);

// - Uses Binary Search to find a doctor entry
// - Returns the index of the entry, or -1 if not found
int binarySearchDoctorByID(int doctorID, int nEntries, Doctor* doctors);

// - Uses quick sort for sorting doctors
// based on their doctorID
void quicksortDoctors(Doctor *doctors, int low, int high);

// This is required by quicksortDoctors to define the partition index
int quicksortDoctorsPartition(Doctor *doctors, int low, int high);

// Used by quicksortDoctors too
void swapDoctors(Doctor* doctorArr, int index1, int index2);

// Utility 
void printDoctorData(Doctor doctorData);

#endif // DOCTOR_DB