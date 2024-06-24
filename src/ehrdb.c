#include "include\ehrdb.h"

// - Loads the contents of EHR txt file to 
// a memory buffer
// - Returns a dynamically allocated EHR array,
// which should be deallocated later.
// - nEHRsOut will receive the number of EHRs present
// in the database.
EHR *deserializeEHRDB(int *nEHRsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(EHRTXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;
    
    deserializeMetadata(metadata, EHRMETATXT_PATH);
    nEntries = metadata->nEntries;
    *nEHRsOut=nEntries;
    nOrphanIDs = metadata->nOrphanIDs;

    EHR outpdata;
    EHR *loadedEntries = malloc(sizeof(EHR)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeEHRDB(): Allocated loaded entries\n");

    int ehrIDinput;
    int patientIDinput;
    char inMedicalHistory[100];
    char inPrescriptions[100];
    char inAllergies[100];
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %d %s %s %s",  
        &ehrIDinput, &patientIDinput, inMedicalHistory, inPrescriptions, inAllergies)
        != EOF
    ) {
        EHR iehr;
        iehr.ehrID=ehrIDinput;
        iehr.patientID=patientIDinput;
        strcpy(iehr.medicalHistory, inMedicalHistory);
        strcpy(iehr.prescriptions, inPrescriptions);
        strcpy(iehr.allergies, inAllergies);

        loadedEntries[entryCounter] = iehr;
        entryCounter++;
    }
    printf("> deserializeEHRDB(): deserialized into loadedEntries\n");

    free(metadata->orphanIDs); // dont forget to deallocate
    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the EHR entries.
// - Returns -1 if aappointmentID error occurred, otherwise returns 0.
int serializeEHRDB(EHR *ehrs, int nEHRs, DBMetadata *metadata) {
    if(NULL==ehrs || nEHRs<0) return -1;

    int metadataState = serializeMetadata(metadata, nEHRs, EHRMETATXT_PATH);

    if(-1==metadataState) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(EHRTXT_PATH, "r"); 

    if(NULL==dbFile) {
        fclose(dbFile);
        return -1;
    }

    fclose(dbFile);

    if(0!=nEHRs) {

        dbFile = fopen(EHRTXT_PATH, "w");

        // Update database entries
        for(int i=0; i<nEHRs; i++) {
            printf("");
            fprintf(dbFile, "%d %d %s %s %s\n", 
            ehrs[i].ehrID, ehrs[i].patientID,
            ehrs[i].medicalHistory, ehrs[i].prescriptions,
            ehrs[i].allergies);
        }
        fclose(dbFile);
    }
    return 0;
}

// - Returns a dynamically allocated array containing
// all the EHR entries.
// - Writes number of entries to nEntries ptr
EHR *getAllEHRs(int *nEntries) {
    DBMetadata metadata;
    int nEntriesLocal;
    EHR *loadedEntries = deserializeEHRDB(&nEntriesLocal, &metadata);
    *nEntries=nEntriesLocal;

    return loadedEntries;
}

// Loads the value of an EHR entry
EHR getEHR(int ehrID) {
    int nEntries;
    DBMetadata metadata;
    EHR* loadedEntries = deserializeEHRDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getEHR(): Loaded entries\n");
    EHR outpdata;

    if(-1==ehrID) {
        outpdata.ehrID=-1;
        return outpdata;
    }

    // Binary search the doctor data
    int foundIndex = binarySearchEHRByID((int)ehrID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.ehrID=loadedEntries[foundIndex].ehrID;
        outpdata.patientID=loadedEntries[foundIndex].patientID;
        strcpy(outpdata.medicalHistory, loadedEntries[foundIndex].medicalHistory);
        strcpy(outpdata.prescriptions, loadedEntries[foundIndex].prescriptions);
        strcpy(outpdata.allergies, loadedEntries[foundIndex].allergies);

    }
    else {
        outpdata.ehrID=-1;
    }

    free(loadedEntries);
    printf("> getEHR(): finished\n");
    return outpdata;
}

// - Sets the value of an existing EHR entry
// - Returns -1 if an error ocurred
int setEHR(int ehrID, EHR newEHRData) {

    int error = 
        (ehrID<0) || (0>newEHRData.patientID) || 
        (NULL==newEHRData.medicalHistory) || 
        (NULL==newEHRData.prescriptions) || 
        (NULL==newEHRData.allergies);

    // If previous condition is 1, toggle it
    // to -1. 0 toggles to 0, thus, no errors.
    error=-error;
    if(-1==error) return error;

    int nEntries;
    DBMetadata metadata={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    EHR* loadedEntries = deserializeEHRDB(&nEntries, &metadata);

    // Search for the patientID in the loaded database
    int foundIndex = //linearSearchInvItemByID(invItemID, nEntries, loadedEntries);
    binarySearchEHRByID(ehrID, nEntries, loadedEntries);

    // Insert the new value
    if(-1!=foundIndex) {
        loadedEntries[foundIndex] = newEHRData;
        // If insertion was successful, sort the array
        // ... (do this later in addPatient when using an orphanID)
    }
    else {
        return -1;
    }
    
    // Rewrite the array to file and modify metadata
    // Return the error/success status of serialization
    error = serializeEHRDB(loadedEntries, nEntries, &metadata);    
    free(loadedEntries);
    return error;
}

// - Adds a new EHR entry
// - Returns the EHR ID of the new EHR item entered
int addEHR(EHR newEHRData) {

    int error =  
        (0>newEHRData.ehrID) || (0>newEHRData.patientID) || 
        (NULL==newEHRData.medicalHistory) || 
        (NULL==newEHRData.prescriptions) ||
        (NULL==newEHRData.allergies); 

    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;
    EHR* loadedEntries = deserializeEHRDB(&nEntries, &metadata);

    // Increase array size
    EHR* extendedEntries = malloc(sizeof(EHR)*(nEntries+1));
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
        newEHRData.ehrID = outID;
    }
    newEHRData.ehrID=outID;
    extendedEntries[outID] =newEHRData; // Append a new entry at the bottom
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortEHRs(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeEHRDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes an EHR entry
// - Returns -1 if error
int removeEHR(int ehrID) {
    if(ehrID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    EHR* loadedEntries = deserializeEHRDB(&nEntries, &metadata);

    int foundIndex = binarySearchEHRByID(ehrID, nEntries, loadedEntries);

    if(-1==foundIndex)
        return -1;

    EHR* reducedEntries;
    
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(EHR)*(nEntries-1));
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
        newOrphanIds[0] = ehrID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs-1] = ehrID;
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

    int error = serializeEHRDB(reducedEntries, metadata.nEntries, &metadata);

    free(reducedEntries);
    free(loadedEntries);

    return error;   
}

// - Uses Linear (naive) search to find an EHR entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchEHRByID(int ehrID, int nEntries, EHR* ehrs) {
    if(NULL==ehrs || nEntries<=0 || ehrID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(ehrID==ehrs[i].ehrID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find an EHR entry
// - Returns the index of the entry, or -1 if not found
int binarySearchEHRByID(int ehrID, int nEntries, EHR* ehrs) {
    if((ehrID<0) || (nEntries <=0)) {
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

            if(ehrs[mid].ehrID==ehrID) {
                foundIndex=mid;
                break;
            }

            if(ehrs[mid].ehrID < ehrID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(ehrs[0].ehrID==ehrID) {
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

// - Uses quick sort for sorting EHR items
// based on their ehrID
void quicksortEHRs(EHR *ehrs, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortEHRPartition(ehrs, low, high);

        // Recursion on the left
        quicksortEHRs(ehrs, low, partitionIndex-1);
        // Recursion on the right 
        quicksortEHRs(ehrs, partitionIndex+1, high);
    }
}

// This is required by quicksortEHRs to define the partition index
int quicksortEHRPartition(EHR *ehrs, int low, int high) {
    int pivotID = ehrs[low].ehrID;
    int i = low;
    int j = high;

    while(i < j) {
        // find fist element greater than the pivot
        // (from start)
        while(ehrs[i].ehrID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(ehrs[j].ehrID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapEHRs(ehrs, i, j);
        }
    }

    swapEHRs(ehrs, low, j);
    return j;
}

// Used by quicksortEHRs too
void swapEHRs(EHR *ehrs, int index1, int index2) {
    EHR temp = ehrs[index1];
    ehrs[index1] = ehrs[index2];
    ehrs[index2] = temp;
}

// Utility
void printEHRData(EHR ehrData) {
    char readablebuffer[100];
    
    clearStr(readablebuffer);
    printYellow("HISTORY");
    printf("---------\n");
    strcpy(readablebuffer, ehrData.medicalHistory);
    replaceCharBy(readablebuffer, '_', ' ');
    printf("%s\n", readablebuffer);
    
    clearStr(readablebuffer);
    printYellow("PRESCRIPTIONS");
    printf("-------------\n");
    strcpy(readablebuffer, ehrData.prescriptions);
    replaceCharBy(readablebuffer, '_', ' ');
    printf("%s\n", readablebuffer);

    clearStr(readablebuffer);
    printYellow("ALLERGIES");
    printf("---------\n");
    strcpy(readablebuffer, ehrData.allergies);
    replaceCharBy(readablebuffer, '_', ' ');
    printf("%s\n", readablebuffer);
}
