#pragma once
#ifndef INVENTORY_H
#define INVENTORY_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"
#include "include\metadataMng.h"

#define INVENTORYTXT_PATH "data\\inventory.txt"
#define INVENTORYMETATXT_PATH "data\\inventoryMeta.txt"

// - Loads the contents of inventory txt file to 
// a memory buffer
// - Returns a dynamically allocated InventoryItem array,
// which should be deallocated later.
// - nInvItemsOut will receive the number of entries present
// in the database.
InventoryItemType *deserializeInventoryDB(int *nInvItemsOut, DBMetadata *metadata);

// - Rewrites the file containing the inventory entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeInventoryDB(InventoryItemType *inventory, int nInvTypes, DBMetadata *metadata);

// - Returns a dynamically allocated array containing
// all the inventory entries.
// - Writes number of entries to nEntries ptr
InventoryItemType *getAllInventoryItems(int *nEntries);

// Loads the value of an inventory entry
InventoryItemType getInventoryItem(int invItemID);

// - Sets the value of an existing inventory entry
// - Returns -1 if an error ocurred
int setInventoryItem(int invItemID, InventoryItemType newInventoryData);

// - Adds a new inventory entry
// - Returns the inventory ID of the new inventory item entered
int addInventoryItem(InventoryItemType newInventoryData);

// - Removes an inventory entry
// - Returns -1 if error
int removeInventoryItem(int invItemID);

// - Uses Linear (naive) search to find an inventory entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchInvItemByID(int invItemID, int nEntries, InventoryItemType* inventory);

// - Uses Binary Search to find an inventory entry
// - Returns the index of the entry, or -1 if not found
int binarySearchInvItemByID(int invItemID, int nEntries, InventoryItemType* inventory);

// - Uses quick sort for sorting inventory items
// based on their invItemID
void quicksortInventory(InventoryItemType *inventory, int low, int high);

// This is required by quicksortInventory to define the partition index
int quicksortInventoryPartition(InventoryItemType *inventory, int low, int high);

// Used by quicksortInventory too
void swapInventoryItems(InventoryItemType* inventory, int index1, int index2);

// Utility 
void printInventoryItemData(InventoryItemType invItemData);

#endif // INVENTORY_H

