#include "include\billdb.h"

// - Loads the contents of inventory txt file to 
// a memory buffer
// - Returns a dynamically allocated Bill array,
// which should be deallocated later.
// - nBillsOut will receive the number of entries present
// in the database.
Bill *deserializeBillDB(int *nBillsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(BILLTXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;
    
    deserializeMetadata(metadata, BILLMETATXT_PATH);
    nEntries = metadata->nEntries;
    *nBillsOut=nEntries;
    nOrphanIDs = metadata->nOrphanIDs;

    Bill outpdata;
    Bill *loadedEntries = malloc(sizeof(Bill)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeBillDB(): Allocated loaded entries\n");

    int inBillID;
    int inPatientID;
    char inInvoiceDateStr[10];
    char inServiceProvider[32];
    float inAmountDue;
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %d %s %s %f",  
        &inBillID, &inPatientID, inInvoiceDateStr, 
        inServiceProvider, &inAmountDue)
        != EOF
    ) {
        Bill ibill;
        ibill.billID=inBillID;
        ibill.patientID=inPatientID;
        
        // Convert string to Date
        strncpy(ibill.invoiceDate.dateInChars.dayDigits, &(inInvoiceDateStr[0]), 2);        
        strncpy(ibill.invoiceDate.dateInChars.monthDigits, &(inInvoiceDateStr[2]), 2);
        strncpy(ibill.invoiceDate.dateInChars.yearDigits, &(inInvoiceDateStr[4]), 4);

        strcpy(ibill.serviceProvider, inServiceProvider);
        ibill.amountDue = inAmountDue;

        loadedEntries[entryCounter] = ibill;
        entryCounter++;
    }
    printf("> deserializeBillDB(): deserialized into loadedEntries\n");

    free(metadata->orphanIDs); // dont forget to deallocate
    fclose(dbFile);
    return loadedEntries;   
}

// - Rewrites the file containing the bill entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeBillDB(Bill *bills, int nBills, DBMetadata *metadata) {
    if(NULL==bills || nBills<0) return -1;

    int metadataState = serializeMetadata(metadata, nBills, BILLMETATXT_PATH);

    if(-1==metadataState) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(BILLTXT_PATH, "r"); 

    if(NULL==dbFile) {
        fclose(dbFile);
        return -1;
    }

    fclose(dbFile);

    if(0!=nBills) {

        dbFile = fopen(BILLTXT_PATH, "w");

        // Update database entries
        char invoiceDateStr[9];
        for(int i=0; i<nBills; i++) {
            
            // Convert Date to string
            strncpy(&(invoiceDateStr[0]), bills[i].invoiceDate.dateInChars.dayDigits, 2);
            strncpy(&(invoiceDateStr[2]), bills[i].invoiceDate.dateInChars.monthDigits, 2);
            strncpy(&(invoiceDateStr[4]), bills[i].invoiceDate.dateInChars.yearDigits, 4);

            fprintf(dbFile, "%d %d %s %s %f\n", 
            bills[i].billID, bills[i].patientID, 
            invoiceDateStr, bills[i].serviceProvider, 
            bills[i].amountDue);
        }
        fclose(dbFile);
    }
    return 0;
}

// - Returns a dynamically allocated array containing
// all the bill entries.
// - Writes number of entries to nEntries ptr
Bill *getAllBills(int *nEntries) {
    DBMetadata metadata;
    int nEntriesLocal;
    Bill *loadedEntries = deserializeBillDB(&nEntriesLocal, &metadata);
    *nEntries=nEntriesLocal;

    return loadedEntries;
}

// Loads the value of a bill entry
Bill getBill(int billID) {
    int nEntries;
    DBMetadata metadata;
    Bill* loadedEntries = deserializeBillDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getBill(): Loaded entries\n");
    Bill outpdata;

    if(-1==billID) {
        outpdata.billID=-1;
        return outpdata;
    }

    // Binary search the doctor data
    int foundIndex = binarySearchBillByID((int)billID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.billID = loadedEntries[foundIndex].billID;
        outpdata.patientID = loadedEntries[foundIndex].patientID;

        // Copy Date
        strncpy(outpdata.invoiceDate.dateInChars.dayDigits, 
        loadedEntries[foundIndex].invoiceDate.dateInChars.dayDigits, 2);
        strncpy(outpdata.invoiceDate.dateInChars.monthDigits,
        loadedEntries[foundIndex].invoiceDate.dateInChars.monthDigits, 2);
        strncpy(outpdata.invoiceDate.dateInChars.yearDigits,
        loadedEntries[foundIndex].invoiceDate.dateInChars.yearDigits, 4);

        strcpy(outpdata.serviceProvider, loadedEntries[foundIndex].serviceProvider);
        outpdata.amountDue = loadedEntries[foundIndex].amountDue;
    
    }
    else {
        outpdata.billID=-1;
    }

    free(loadedEntries);
    printf("> getBill(): finished\n");
    return outpdata;
}

// - Sets the value of an existing bill entry
// - Returns -1 if an error ocurred
//int setBill(int billID, Bill newBillData);

// - Adds a new bill entry
// - Returns the bill ID of the new bill entered
int addBill(Bill newBillData) {

    int error =  
        (0>newBillData.patientID) || 
        (NULL==newBillData.invoiceDate.dateInChars.dayDigits) || 
        (NULL==newBillData.invoiceDate.dateInChars.monthDigits) ||
        (NULL==newBillData.invoiceDate.dateInChars.yearDigits) ||
        (NULL==newBillData.serviceProvider) ||
        (0>newBillData.amountDue); 

    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;
    Bill* loadedEntries = deserializeBillDB(&nEntries, &metadata);

    // Increase array size
    Bill* extendedEntries = malloc(sizeof(Bill)*(nEntries+1));
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
    }
    else {
        outID = nEntries; // this equals to (nEntries+1) -1
        newBillData.billID = outID;
    }
    newBillData.billID=outID;
    extendedEntries[outID] =newBillData; // Append a new entry at the bottom
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortBills(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeBillDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes an bill entry
// - Returns -1 if error
int removeBill(int billID) {
    if(billID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    Bill* loadedEntries = deserializeBillDB(&nEntries, &metadata);

    int foundIndex = binarySearchBillByID(billID, nEntries, loadedEntries);

    if(-1==foundIndex)
        return -1;

    Bill* reducedEntries;
    
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(Bill)*(nEntries-1));
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
        newOrphanIds[0] = billID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs-1] = billID;
        printf("Appended orphanID=%d\n", newOrphanIds[metadata.nOrphanIDs-1]);
    }

    if(NULL!=metadata.orphanIDs) {
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

    int error = serializeBillDB(reducedEntries, metadata.nEntries, &metadata);

    free(reducedEntries);
    free(loadedEntries);

    return error;
}

// - Uses Linear (naive) search to find a bill entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchBillByID(int billID, int nEntries, Bill* bills) {
    if(NULL==bills || nEntries<=0 || billID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(billID==bills[i].billID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find a bill entry
// - Returns the index of the entry, or -1 if not found
int binarySearchBillByID(int billID, int nEntries, Bill* bills) {
    if((billID<0) || (nEntries <=0)) {
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

            if(bills[mid].billID==billID) {
                foundIndex=mid;
                break;
            }

            if(bills[mid].billID <billID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(bills[0].billID==billID) {
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

// - Uses quick sort for sorting bill items
// based on their billID
void quicksortBills(Bill *bills, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortBillsPartition(bills, low, high);

        // Recursion on the left
        quicksortBills(bills, low, partitionIndex-1);
        // Recursion on the right 
        quicksortBills(bills, partitionIndex+1, high);
    }
}

// This is required by quicksortInventory to define the partition index
int quicksortBillsPartition(Bill *bills, int low, int high) {
    int pivotID = bills[low].billID;
    int i = low;
    int j = high;

    while(i < j) {
        // find fist element greater than the pivot
        // (from start)
        while(bills[i].billID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(bills[j].billID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapBills(bills, i, j);
        }
    }

    swapBills(bills, low, j);
    return j;
}

// Used by quicksortInventory too
void swapBills(Bill* Bills, int index1, int index2) {
    Bill temp = Bills[index1];
    Bills[index1] = Bills[index2];
    Bills[index2] = temp;
}

// Utility 
void printBillData(Bill billData) {
    if(0>billData.billID) {
        printf("|< NON EXISTANT BILL >|\n");
        return;
    }

    printf("| %d | %d | %2s/%2s/%4s | %s | %f |\n",
    billData.billID, billData.patientID, 
    billData.invoiceDate.dateInChars.dayDigits,
    billData.invoiceDate.dateInChars.monthDigits,
    billData.invoiceDate.dateInChars.yearDigits,
    billData.serviceProvider, billData.amountDue);
}