#pragma once
#ifndef APPOINTNMENTDB_H
#define APPOINTNMENTDB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"
#include "include\metadataMng.h"

#define APPOINTMENTTXT_PATH "data\\appointments.txt"
#define APPPOINTMENTMETATXT_PATH "data\\appointmentsMeta.txt"

// - Loads the contents of appoitnments txt file to 
// a memory buffer
// - Returns a dynamically allocated Appointment array,
// which should be deallocated later.
// - nAppointnments will receive the number of appointments present
// in the database.
Appointment *deserializeAppointmentDB(int *nAppointmentsOut, DBMetadata *metadata);

// - Rewrites the file containing the appointment entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeAppointmentDB(Appointment *appointments, int nAppointments, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the appointment entries.
// - Writes number of entries to nEntries ptr
Appointment *getAllAppointments(int *nEntries);

// Loads the value of an appointment entry
Appointment getAppointment(int appointmentID);

// - Sets the value of an existing appointment entry
// - Returns -1 if an error ocurred
//int setAppointment(int appointmentID, Appointment newAppointmentData);

// - Adds a new appointment entry
// - Returns the appointment ID of the new appointment item entered
int addAppointment(Appointment newAppointmentData);

// - Removes an appointment entry
// - Returns -1 if error
int removeAppointment(int appointmentID);

// - Uses Linear (naive) search to find an appointment entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchAppointmentByID(int appointmentID, int nEntries, Appointment* appointments);

// - Uses Binary Search to find an appointment entry
// - Returns the index of the entry, or -1 if not found
int binarySearchAppointmentByID(int appointmentID, int nEntries, Appointment* appointments);

// - Uses quick sort for sorting appointment items
// based on their appointmentID
void quicksortAppointments(Appointment *appointments, int low, int high);

// This is required by quicksortAppointment to define the partition index
int quicksortAppointmentPartition(Appointment *appointments, int low, int high);

// Used by quicksortAppointment too
void swapAppointments(Appointment *appointments, int index1, int index2);

// Utility 
void printAppointmentData(Appointment appointmentData);

#endif // APPOINTNMENTDB_H