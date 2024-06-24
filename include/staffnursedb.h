#pragma once
#ifndef STAFFNURSE_DB
#define STAFFNURSE_DB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\metadataMng.h"

#define STAFFNURSESTXT_PATH "data\\staffNurses.txt"
#define STAFFNURSESMETATXT_PATH "data\\staffNursesMeta.txt"

// - Loads the contents of staffNurses.txt file to 
// a memory buffer
// - Returns a dynamically allocated StaffNurse array,
// which should be deallocated later.
// - nStaffNursesOut will receive the number of staff nurses present
// in the database.
StaffNurse *deserializeStaffNurseDB(int *nStaffNursesOut, DBMetadata *metadata);

// - Rewrites the file containing the staff nurse entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeStaffNurseDB(StaffNurse *staffNurses, int nStaffNurses, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the staff nurse entries.
// - Writes number of entries to nEntries ptr
StaffNurse *getAllStaffNurses(int *nEntries);

// Loads the value of a staff nurse entry
StaffNurse getStaffNurse(int staffNurseID);

// - Sets the value of an existing staff nurse entry
// - Returns -1 if an error ocurred
int setStaffNurse(int staffNurseID, StaffNurse newStaffNurseData);

// - Adds a new staff nurse entry
// - Returns the staff nurse ID of the new staff nurse entered
int addStaffNurse(StaffNurse newStaffNurseData);

// - Removes a staff nurse entry
// - Returns -1 if error
int removeStaffNurse(int staffNurseID);

// - Uses Linear (naive) search to find a staff nurse entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchStaffNurseByID(int staffNurseID, int nEntries, StaffNurse* staffNurses);

// - Uses Binary Search to find a staff nurse entry
// - Returns the index of the entry, or -1 if not found
int binarySearchStaffNurseByID(int staffNurseID, int nEntries, StaffNurse* staffNurses);

// - Uses quick sort for sorting staff nurses
// based on their staff nurse ID
void quicksortStaffNurses(StaffNurse *staffNurses, int low, int high);

// This is required by quicksortStaffNurses to define the partition index
int quicksortStaffNursesPartition(StaffNurse *staffNurses, int low, int high);

// Used by quicksortStaffNurses too
void swapStaffNurses(StaffNurse* staffNurseArr, int index1, int index2);

// Utility 
void printStaffNurseData(StaffNurse staffNurseData);

#endif // STAFFNURSE_DB