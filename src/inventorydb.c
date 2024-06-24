#include "include\inventorydb.h"

// - Loads the contents of inventory txt file to
// a memory buffer
// - Returns a dynamically allocated InventoryItem array,
// which should be deallocated later.
// - nInvItemsOut will receive the number of inventory present
// in the database.
InventoryItemType *deserializeInventoryDB(int *nInvItemsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(INVENTORYTXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;
    
    deserializeMetadata(metadata, INVENTORYMETATXT_PATH);
    nEntries = metadata->nEntries;
    *nInvItemsOut=nEntries;
    nOrphanIDs = metadata->nOrphanIDs;

    InventoryItemType outpdata;
    InventoryItemType *loadedEntries = malloc(sizeof(InventoryItemType)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeInventoryDB(): Allocated loaded entries\n");

    int invItemIDinput;
    char name[64];
    char description[256];
    char category[64];
    int quantity;
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %s %s %s %d",  
        &invItemIDinput, name, description, category, &quantity)
        != EOF
    ) {
        InventoryItemType iinvitem;
        iinvitem.invItemID=invItemIDinput;
        strcpy(iinvitem.name, name);
        strcpy(iinvitem.description, description);
        strcpy(iinvitem.category, category);
        iinvitem.quantity = quantity;

        loadedEntries[entryCounter] = iinvitem;
        entryCounter++;
    }
    printf("> deserializeInventoryDB(): deserialized into loadedEntries\n");

    free(metadata->orphanIDs); // dont forget to deallocate
    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the inventory entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeInventoryDB(InventoryItemType *inventory, int nInvTypes, DBMetadata *metadata) {
    if(NULL==inventory || nInvTypes<0) return -1;

    int metadataState = serializeMetadata(metadata, nInvTypes, INVENTORYMETATXT_PATH);

    if(-1==metadataState) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(INVENTORYTXT_PATH, "r"); 

    if(NULL==dbFile) {
        fclose(dbFile);
        return -1;
    }

    fclose(dbFile);

    if(0!=nInvTypes) {

        dbFile = fopen(INVENTORYTXT_PATH, "w");

        // Update database entries
        for(int i=0; i<nInvTypes; i++) {
            
            fprintf(dbFile, "%d %s %s %s %d\n", 
            inventory[i].invItemID, inventory[i].name, 
            inventory[i].description, inventory[i].category,
            inventory[i].quantity);
        }
        fclose(dbFile);
    }
    return 0;
}

// - Returns a dynamically allocated array containing
// all the inventory entries.
// - Writes number of entries to nEntries ptr
InventoryItemType *getAllInventoryItems(int *nEntries) {
    DBMetadata metadata;
    int nEntriesLocal;
    InventoryItemType *loadedEntries = deserializeInventoryDB(&nEntriesLocal, &metadata);
    *nEntries=nEntriesLocal;

    return loadedEntries;
}

// Loads the value of an inventory entry
InventoryItemType getInventoryItem(int invItemID) {
    int nEntries;
    DBMetadata metadata;
    InventoryItemType* loadedEntries = deserializeInventoryDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getInventoryItem(): Loaded entries\n");
    InventoryItemType outpdata;

    if(-1==invItemID) {
        outpdata.invItemID=-1;
        return outpdata;
    }

    // Binary search the doctor data
    int foundIndex = linearSearchInvItemByID((int)invItemID, nEntries, loadedEntries);//binarySearchPatient((int)patientID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.invItemID = loadedEntries[foundIndex].invItemID;
        strcpy(outpdata.name, loadedEntries[foundIndex].name);
        strcpy(outpdata.description, loadedEntries[foundIndex].description);
        outpdata.quantity = loadedEntries[foundIndex].quantity;
    }
    else {
        outpdata.invItemID=-1;
    }

    free(loadedEntries);
    printf("> getInventoryItem(): finished\n");
    return outpdata;
}

// - Sets the value of an existing inventory entry
// - Returns -1 if an error ocurred
int setInventoryItem(int invItemID, InventoryItemType newInventoryData) {

    int error = 
        (invItemID<0) || (NULL==newInventoryData.name) || 
        (NULL==newInventoryData.description) || (0>newInventoryData.quantity);

    // If previous condition is 1, toggle it
    // to -1. 0 toggles to 0, thus, no errors.
    error=-error;
    if(-1==error) return error;

    int nEntries;
    DBMetadata metadata={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    InventoryItemType* loadedEntries = deserializeInventoryDB(&nEntries, &metadata);

    // Search for the patientID in the loaded database
    int foundIndex = //linearSearchInvItemByID(invItemID, nEntries, loadedEntries);
    binarySearchInvItemByID(invItemID, nEntries, loadedEntries);

    // Insert the new value
    if(-1!=foundIndex) {
        loadedEntries[foundIndex] = newInventoryData;
        // If insertion was successful, sort the array
        // ... (do this later in addPatient when using an orphanID)
    }
    else {
        return -1;
    }
    
    // Rewrite the array to file and modify metadata
    // Return the error/success status of serialization
    error = serializeInventoryDB(loadedEntries, nEntries, &metadata);    
    free(loadedEntries);
    return error;
}

// - Adds a new inventory entry
// - Returns the inventory ID of the new inventory item entered
int addInventoryItem(InventoryItemType newInventoryData) {

    int error =  
        (NULL==newInventoryData.name) || 
        (NULL==newInventoryData.description) || (0>newInventoryData.quantity); 

    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;
    InventoryItemType* loadedEntries = deserializeInventoryDB(&nEntries, &metadata);

    // Increase array size
    InventoryItemType* extendedEntries = malloc(sizeof(InventoryItemType)*(nEntries+1));
    // Copy the previous entries

    for(int i=0;i<nEntries;i++) {
		extendedEntries[i]=loadedEntries[i];
	}
	
    if(metadata.nOrphanIDs>0) {
        // Pop the last orphanID
        int newNOrphans=metadata.nOrphanIDs-1;
        printf("nOrphans-1==%d\n", newNOrphans);
        outID=metadata.orphanIDs[newNOrphans];
        printf("orphan ID re-added == %d\n", outID);
        metadata.nOrphanIDs=newNOrphans;

        int* newOrphanIDs;
        // Update the orphan IDs
        if(newNOrphans>0) {
            
            int *newOrphanIDs = malloc(sizeof(int)*newNOrphans);
            for(int i=0;i<newNOrphans;i++) {
                newOrphanIDs[i]=metadata.orphanIDs[i];
            }

            // Replace old pointer
            free(metadata.orphanIDs);
            metadata.orphanIDs=newOrphanIDs;
        }
        else {
            free(metadata.orphanIDs);
            metadata.orphanIDs=NULL;
        }
        //extendedEntries[outID] = newPatientData;
    }
    else {
        outID = nEntries; // this equals to (nEntries+1) -1
        newInventoryData.invItemID = outID;
    }
    newInventoryData.invItemID=outID;
    extendedEntries[outID] =newInventoryData; // Append a new entry at the bottom
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortInventory(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeInventoryDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes an inventory entry
// - Returns -1 if error
int removeInventoryItem(int invItemID) {
    if(invItemID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    InventoryItemType* loadedEntries = deserializeInventoryDB(&nEntries, &metadata);

    //int foundIndex = linearSearchInvItemByID(invItemID, nEntries, loadedEntries);
    int foundIndex = binarySearchInvItemByID(invItemID, nEntries, loadedEntries);

    if(-1==foundIndex)
        return -1;

    InventoryItemType* reducedEntries;
    
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(InventoryItemType)*(nEntries-1));
    }

    int prevArrCounter=0;
    int reducedArrCounter=0;
    if(NULL!=reducedEntries) {
        while((reducedArrCounter<nEntries-1) && (prevArrCounter<nEntries)) {
            if(prevArrCounter == foundIndex) {
                prevArrCounter++; // Skips the entry to eliminate
            }

            // Copy each new entry
            reducedEntries[reducedArrCounter] = loadedEntries[prevArrCounter];

            prevArrCounter++;
            reducedArrCounter++;
        }
    }

    // Update orphan IDs
    int *newOrphanIds;
    printf("nOrphanIDs=%d\n", metadata.nOrphanIDs);
    if(metadata.nOrphanIDs<=0) {
        newOrphanIds = malloc(sizeof(int)*1);
        newOrphanIds[0] = invItemID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs-1] = invItemID;
        printf("Appended orphanID=%d\n", newOrphanIds[metadata.nOrphanIDs-1]);
    }

    if(NULL!=metadata.orphanIDs) {
        //memcpy(newOrphanIds, metadata.orphanIDs, metadata.nOrphanIDs);
        // copy the previous orphanIDs
        for(int i=0; 
        i<metadata.nOrphanIDs;
        i++){
            newOrphanIds[i]=metadata.orphanIDs[i];
        }
        free(metadata.orphanIDs);
    }    

    // Update main metadata
    metadata.nEntries--;
    metadata.nOrphanIDs++;

    metadata.orphanIDs = newOrphanIds;
    
    //Debug
    printf("NEW ORPHAN IDs: ");
    for(int i=0; i<metadata.nOrphanIDs;i++) {
        printf("%d=%d ", i, newOrphanIds[i]);
    }
    printf("\n");

    printf("METADATA ORPHAN IDs: ");
    for(int i=0; i<metadata.nOrphanIDs;i++) {
        printf("%d=%d ", i, metadata.orphanIDs[i]);
    }
    printf("\n");

    int error = serializeInventoryDB(reducedEntries, metadata.nEntries, &metadata);

    free(reducedEntries);
    free(loadedEntries);

    return error;   
}

// - Uses Linear (naive) search to find an inventory entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchInvItemByID(int invItemID, int nEntries, InventoryItemType* inventory) {
    if(NULL==inventory || nEntries<=0 || invItemID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(invItemID==inventory[i].invItemID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find an inventory entry
// - Returns the index of the entry, or -1 if not found
int binarySearchInvItemByID(int invItemID, int nEntries, InventoryItemType* inventory) {
    if((invItemID<0) || (nEntries <=0)) {
        return -1;
    }
    
    int foundIndex=0;
    if(nEntries > 1) {
        // High and low indexes
        int lowp=0, highp=nEntries-1;
        //foundIndex = -1;

        // Binary search (loadedEntries must be sorted by patientID)
        while(lowp <= highp) {
            int mid = lowp + (highp-lowp)/2;

            if(inventory[mid].invItemID==invItemID) {
                foundIndex=mid;
                break;
            }

            if(inventory[mid].invItemID < invItemID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(inventory[0].invItemID==invItemID) {
            return 0;
        }
        else {
            return -1;
        }
    }
    else {
        return -1;
    }
}

// - Uses quick sort for sorting inventory items
// based on their invItemID
void quicksortInventory(InventoryItemType *inventory, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortInventoryPartition(inventory, low, high);

        // Recursion on the left
        quicksortInventory(inventory, low, partitionIndex-1);
        // Recursion on the right 
        quicksortInventory(inventory, partitionIndex+1, high);
    }
}

// This is required by quicksortInventory to define the partition index
int quicksortInventoryPartition(InventoryItemType *inventory, int low, int high) {
    int pivotID = inventory[low].invItemID;
    int i = low;
    int j = high;

    while(i < j) {
        // find fist element greater than the pivot
        // (from start)
        while(inventory[i].invItemID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(inventory[j].invItemID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapInventoryItems(inventory, i, j);
        }
    }

    swapInventoryItems(inventory, low, j);
    return j;
}

// Used by quicksortInventory too
void swapInventoryItems(InventoryItemType* inventory, int index1, int index2) {
    InventoryItemType temp = inventory[index1];
    inventory[index1] = inventory[index2];
    inventory[index2] = temp;
}

// Utility 
void printInventoryItemData(InventoryItemType invItemData) {
    if(0>invItemData.invItemID) {
        printf("|< NON EXISTANT INVENTORY ITEM >|\n");
        return;
    }

    printf("| %d | %s | %s | %s | %d |\n", 
    invItemData.invItemID, invItemData.name, 
    invItemData.description, invItemData.category,
    invItemData.quantity);
}
