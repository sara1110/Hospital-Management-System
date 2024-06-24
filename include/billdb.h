#pragma once
#ifndef BILLDB_H
#define BILLDB_H

#include "include\dataDefs.h"
#include "include\metadataMng.h"

#define BILLTXT_PATH "data\\bills.txt"
#define BILLMETATXT_PATH "data\\billsMeta.txt"

// - Loads the contents of inventory txt file to 
// a memory buffer
// - Returns a dynamically allocated Bill array,
// which should be deallocated later.
// - nBillsOut will receive the number of entries present
// in the database.
Bill *deserializeBillDB(int *nBillsOut, DBMetadata *metadata);

// - Rewrites the file containing the bill entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeBillDB(Bill *bills, int nBills, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the bill entries.
// - Writes number of entries to nEntries ptr
Bill *getAllBills(int *nEntries);

// Loads the value of a bill entry
Bill getBill(int billID);

// - Sets the value of an existing bill entry
// - Returns -1 if an error ocurred
//int setBill(int billID, Bill newBillData);

// - Adds a new bill entry
// - Returns the bill ID of the new bill entered
int addBill(Bill newBillData);

// - Removes an bill entry
// - Returns -1 if error
int removeBill(int billID);

// - Uses Linear (naive) search to find a bill entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchBillByID(int billID, int nEntries, Bill* bills);

// - Uses Binary Search to find a bill entry
// - Returns the index of the entry, or -1 if not found
int binarySearchBillByID(int billID, int nEntries, Bill* bills);

// - Uses quick sort for sorting bill items
// based on their billID
void quicksortBills(Bill *bills, int low, int high);

// This is required by quicksortInventory to define the partition index
int quicksortBillsPartition(Bill *bills, int low, int high);

// Used by quicksortInventory too
void swapBills(Bill* Bills, int index1, int index2);

// Utility 
void printBillData(Bill billData);


#endif // BILLDB_H