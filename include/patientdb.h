#pragma once
#ifndef PATIENT_DB
#define PATIENT_DB

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"
#include "include\metadataMng.h"

#define PATIENTSTXT_PATH "data\\patients.txt"
#define PATIENTSMETATXT_PATH "data\\patientsMeta.txt"

// - Loads the contents of patient txt file to 
// a memory buffer
// - Returns a dynamically allocated Patient array,
// which should be deallocated later.
// - nPatientsOut will receive the number of patients present
// in the database.
Patient *deserializePatientDB(int *nPatientsOut, DBMetadata *metadata);

// - Rewrites the file containing the patient entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializePatientDB(Patient *patients, int nPatients, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the student entries.
// - Writes number of entries to nEntries ptr
Patient *getAllPatients(int *nEntries);

// Loads the value of a patient entry
Patient getPatient(int patientID);

// - Sets the value of an existing patient entry
// - Returns -1 if an error ocurred
int setPatient(int patientID, Patient newPatientData);

// - Adds a new patient entry
// - Returns the patient ID of the new patient entered
int addPatient(Patient newPatientData);

// - Removes a patient entry
// - Returns -1 if error
int removePatient(int patientID);

// - Uses Linear (naive) search to find a patient entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchPatientByID(int patientID, int nEntries, Patient* patients);

// - Uses Binary Search to find a patient entry
// - Returns the index of the entry, or -1 if not found
int binarySearchPatientByID(int patientID, int nEntries, Patient* patients);

// - Uses quick sort for sorting patients
// based on their patientID
void quicksortPatients(Patient *patients, int low, int high);

// This is required by quicksortPatients to define the partition index
int quicksortPatientsPartition(Patient *patients, int low, int high);

// Used by quicksortPatients too
void swapPatients(Patient* patientArr, int index1, int index2);

// Utility 
void printPatientData(Patient patientData);

#endif