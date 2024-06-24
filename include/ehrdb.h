#pragma once
#ifndef EHRDB_H
#define EHRDB_H

#include <string.h>
#include "include\dataDefs.h"
#include "include\cmdui_utils.h"
#include "include\datautils.h"
#include "include\metadataMng.h"

#define EHRTXT_PATH "data\\EHRs.txt"
#define EHRMETATXT_PATH "data\\EHRsMeta.txt"

// - Loads the contents of EHR txt file to 
// a memory buffer
// - Returns a dynamically allocated EHR array,
// which should be deallocated later.
// - nEHRsOut will receive the number of EHRs present
// in the database.
EHR *deserializeEHRDB(int *nEHRsOut, DBMetadata *metadata);

// - Rewrites the file containing the EHR entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeEHRDB(EHR *appointments, int nAppointments, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the EHR entries.
// - Writes number of entries to nEntries ptr
EHR *getAllEHRs(int *nEntries);

// Loads the value of an EHR entry
EHR getEHR(int ehrID);

// - Sets the value of an existing EHR entry
// - Returns -1 if an error ocurred
int setEHR(int ehrID, EHR newEHRData);

// - Adds a new EHR entry
// - Returns the EHR ID of the new EHR item entered
int addEHR(EHR newEHRData);

// - Removes an EHR entry
// - Returns -1 if error
int removeEHR(int ehrID);

// - Uses Linear (naive) search to find an EHR entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchEHRByID(int ehrID, int nEntries, EHR* ehrs);

// - Uses Binary Search to find an EHR entry
// - Returns the index of the entry, or -1 if not found
int binarySearchEHRByID(int ehrID, int nEntries, EHR* ehrs);

// - Uses quick sort for sorting EHR items
// based on their ehrID
void quicksortEHRs(EHR *ehrs, int low, int high);

// This is required by quicksortEHRs to define the partition index
int quicksortEHRPartition(EHR *ehrs, int low, int high);

// Used by quicksortEHRs too
void swapEHRs(EHR *ehrs, int index1, int index2);

// Utility
void printEHRData(EHR ehrData);

#endif 