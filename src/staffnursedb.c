#include "include\staffnursedb.h"

// - Loads the contents of staffNurses.txt file to 
// a memory buffer
// - Returns a dynamically allocated StaffNurse array,
// which should be deallocated later.
// - nStaffNursesOut will receive the number of staff nurses present
// in the database.
StaffNurse *deserializeStaffNurseDB(int *nStaffNursesOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(STAFFNURSESTXT_PATH, "r");
    FILE *metaFile = fopen(STAFFNURSESMETATXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;
    
    char lineBuff[100];
    char line1Buff[100];
    char line2Buff[100];
    int lineCounter=0;
    
    // Parse each patientsMeta.txt line
    while(fgets(lineBuff, sizeof(lineBuff), metaFile) != NULL) {
        // First line
        if(0==lineCounter) {
            strcpy(line1Buff, lineBuff);
            sscanf(line1Buff, "%d %d", &nEntries, &nOrphanIDs);
        }
        // Second line
        else if (1==lineCounter) {
            strcpy(line2Buff, lineBuff);
        }
        lineCounter++;
        if(lineCounter==2) break;
    }

    metadata->nEntries=nEntries;
    metadata->nOrphanIDs=nOrphanIDs;
	
	// avoid allocating '0 bytes'
    metadata->orphanIDs = (nOrphanIDs > 0) ? 
		malloc(sizeof(int) * nOrphanIDs) : (NULL);
    
	*nStaffNursesOut=nEntries;

    //printf("LINE1BUFF==%s\n", line1Buff); // TESTING ONLY
    //printf("LINE2BUFF==%s\n", line2Buff); // TESTING ONLY

    if(metadata->nOrphanIDs>0) {
        // Parse the orphanIDs array
        // (Load the orphan IDs as an array)
        char wordBuff[10];
        // Setup the wordBuff to an empty string
        for(int cc=0;cc<sizeof(wordBuff);cc++) 
            wordBuff[cc]=' ';

        wordBuff[sizeof(wordBuff)-1]='\0';

        int lastWordBuffi = 0;
        int idCounter = 0;
        for(int c=0; c<sizeof(line2Buff);c++) {
            //printf("    | wordBuff==%s\n", wordBuff);
            if(line2Buff[c] == ' ' 
            || line2Buff[c] == '\n'
            || line2Buff[c] == '\0') {
                // Convert the wordbuff into an integer, and push it to orphanIDs
                if(lastWordBuffi!=0) {
                    int newOrphanID;
                    sscanf(wordBuff, "%d", &newOrphanID);
                    metadata->orphanIDs[idCounter]=newOrphanID;
                    idCounter++;
                    // clear the wordBuff
                    for(int cc=0;cc<sizeof(wordBuff);cc++) 
                        wordBuff[cc]=' ';
                    wordBuff[sizeof(wordBuff)-1]='\0';
                    lastWordBuffi=0;
                }
                if(line2Buff[c] == '\n'
                || line2Buff[c] == '\0') {
                    break;
                }
            }
            else {
                if(lastWordBuffi<=9) {
                    wordBuff[lastWordBuffi] = line2Buff[c];
                    lastWordBuffi++;
                }
                else {
                    break;
                }
            }
        }
    }
        
    // TESTING: Print the parsed orphanIDs array
    printf("> orphanIDs = { ");
    for(int i=0;i<nOrphanIDs;i++) {
        printf("%d, ", metadata->orphanIDs[i]);
    }
    printf(" }\n");

    //free(metadata->orphanIDs);
    fclose(metaFile); 

    StaffNurse outpdata;
    StaffNurse *loadedEntries = malloc(sizeof(StaffNurse)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeStaffNurseDB(): Allocated loaded entries\n");

    int staffNurseIDinput;
    char personalIDstr[50];
    char nameStr[50];
    char passStr[50];
    char functionalUnitStr[50];
    //int isLocal;
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %s %s %s %s", 
        &staffNurseIDinput, personalIDstr, nameStr, passStr, functionalUnitStr)
        != EOF
    ) {
        StaffNurse istaffnurse;
            istaffnurse.staffNurseID=staffNurseIDinput;
            strcpy(istaffnurse.personalID, personalIDstr);
            strcpy(istaffnurse.name, nameStr);
            strcpy(istaffnurse.password, passStr);
            strcpy(istaffnurse.functionalUnitStr, functionalUnitStr);
            //ipatient.isLocal = isLocal;

        loadedEntries[entryCounter] = istaffnurse;
        entryCounter++;
        //printf("> iterating on dbFile read\n");
    }
    printf("> deserializeStaffNurseDB(): deserialized into loadedEntries\n");

    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the staff nurse entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeStaffNurseDB(StaffNurse *staffNurses, int nStaffNurses, DBMetadata *metadata) {
   if(NULL==staffNurses || nStaffNurses<0) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(STAFFNURSESTXT_PATH, "r"); 
    FILE *dbMeta = fopen(STAFFNURSESMETATXT_PATH, "r");
    if(NULL==dbFile || NULL==dbMeta) {
        fclose(dbFile);
        fclose(dbMeta);
        return -1; 
    }
    
    fclose(dbFile);
    fclose(dbMeta);

    // nStaffNurses == 0 means the last entry should be cleared,
    // thus this loop should not be executed
    if(0!=nStaffNurses) {
        dbFile = fopen(STAFFNURSESTXT_PATH, "w");
        dbMeta = fopen(STAFFNURSESMETATXT_PATH, "w");
        // Update database entries
        for(int i=0; i<nStaffNurses; i++) {
            printf("");
            fprintf(dbFile, "%d %s %s %s %s\n",
            staffNurses[i].staffNurseID, staffNurses[i].personalID, staffNurses[i].name,
            staffNurses[i].password, staffNurses[i].functionalUnitStr);
        }
    }

    //Update metadata, even when nStaffNurses is 0
    fprintf(dbMeta, "%d %d\n", metadata->nEntries, metadata->nOrphanIDs);

    // Serialize orphanIDs 
    if(metadata->nOrphanIDs>0){
        // Use the preexisting orphanIDs         
        if(NULL!=metadata->orphanIDs) {
            for(int i=0; i<metadata->nOrphanIDs;i++) {
                int iOrphan = metadata->orphanIDs[i];
                fprintf(dbMeta, "%d ", iOrphan);
            }
            fprintf(dbMeta, "\n");
        }
        // Create a temporal orphanIDs array with the 
        // removed ID and serialize it (?)
        else {
            
        }
    }

    fclose(dbFile);
    fclose(dbMeta);

    return 0;
}

// - Returns a dynamically allocated array containing
// all the staff nurse entries.
// - Writes number of entries to nEntries ptr
StaffNurse *getAllStaffNurses(int *nEntries) {
    DBMetadata metadata;
    StaffNurse* loadedEntries = deserializeStaffNurseDB(nEntries, &metadata);
    
    return loadedEntries;
}

// Loads the value of a staff nurse entry
StaffNurse getStaffNurse(int staffNurseID) {
    int nEntries;
    DBMetadata metadata;
    StaffNurse* loadedEntries = deserializeStaffNurseDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getStaffNurse(): Loaded entries\n");
    StaffNurse outpdata;

    if(-1==staffNurseID) {
        outpdata.staffNurseID=-1;
        return outpdata;
    }

    // Binary search the staff nurse data
    int foundIndex = linearSearchStaffNurseByID((int)staffNurseID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.staffNurseID = loadedEntries[foundIndex].staffNurseID;
        strcpy(outpdata.personalID, loadedEntries[foundIndex].personalID);
        strcpy(outpdata.name, loadedEntries[foundIndex].name);
        strcpy(outpdata.password, loadedEntries[foundIndex].password);
        //outpdata.isLocal = loadedEntries[foundIndex].isLocal;
        strcpy(outpdata.functionalUnitStr, loadedEntries[foundIndex].functionalUnitStr);
    }
    else {
        outpdata.staffNurseID=-1;
    }

    free(loadedEntries);
    printf("> getStaffNurse(): finished\n");
    return outpdata;
}

// - Sets the value of an existing staff nurse entry
// - Returns -1 if an error ocurred
int setStaffNurse(int staffNurseID, StaffNurse newStaffNurseData) {
    int error = 
        (staffNurseID<0) || (NULL==newStaffNurseData.personalID) || 
        (NULL==newStaffNurseData.name) || (NULL==newStaffNurseData.password) || 
        (NULL==newStaffNurseData.functionalUnitStr);
        //((1!=newStaffNurseData.isLocal) && (0!=newStaffNurseData.isLocal)); 

    // If previous condition is 1, toggle it
    // to -1. 0 toggles to 0, thus, no errors.
    error=-error;
    if(-1==error) return error;

    int nEntries;
    DBMetadata metadata={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    StaffNurse* loadedEntries = deserializeStaffNurseDB(&nEntries, &metadata);

    // Search for the patientID in the loaded database
    int foundIndex = linearSearchStaffNurseByID(staffNurseID, nEntries, loadedEntries);

    // Insert the new value
    if(-1!=foundIndex) {
        loadedEntries[foundIndex] = newStaffNurseData;
        // If insertion was successful, sort the array
        // ... (do this later in addStaffNurse when using an orphanID)
    }
    else {
        return -1;
    }

    // Rewrite the array to file and modify metadata
    // Return the error/success status of serialization
    error = serializeStaffNurseDB(loadedEntries, nEntries, &metadata);    
    free(loadedEntries);
    return error;
}

// - Adds a new staff nurse entry
// - Returns the staff nurse ID of the new staff nurse entered
int addStaffNurse(StaffNurse newStaffNurseData) {
    int error = 
        //(newPatientData.patientID<0) || 
        (NULL==newStaffNurseData.personalID) || 
        (NULL==newStaffNurseData.name) || (NULL==newStaffNurseData.password) || 
        (NULL==newStaffNurseData.functionalUnitStr); 
        //((1!=newStaffNurseData.isLocal) && (0!=newStaffNurseData.isLocal)); 
    
    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;//={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    StaffNurse* loadedEntries = deserializeStaffNurseDB(&nEntries, &metadata);

    // Increase array size
    StaffNurse* extendedEntries = malloc(sizeof(StaffNurse)*(nEntries+1));
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
        //metadata.nEntries+=1;
        outID = nEntries; // this equals to (nEntries+1) -1
        newStaffNurseData.staffNurseID = outID;
        //extendedEntries[outID] = newPatientData; // Append a new entry at the bottom
    }
    newStaffNurseData.staffNurseID=outID;
    extendedEntries[outID] = newStaffNurseData;
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortStaffNurses(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeStaffNurseDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes a staff nurse entry
// - Returns -1 if error
int removeStaffNurse(int staffNurseID) {
    if(staffNurseID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    StaffNurse* loadedEntries = deserializeStaffNurseDB(&nEntries, &metadata);

    int foundIndex = linearSearchStaffNurseByID(staffNurseID, nEntries, loadedEntries);//binarySearchPatient(patientID, nEntries, loadedEntries);
    
    if(-1==foundIndex)
        return -1;

    StaffNurse* reducedEntries;
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(StaffNurse)*(nEntries-1));
    }

    int prevArrCounter=0;
    int reducedArrCounter=0;
    if(NULL!=reducedEntries) {
        while((reducedArrCounter<nEntries-1) && (prevArrCounter<nEntries)) {
            if(prevArrCounter == foundIndex) {
                prevArrCounter++; // Skips the entry to eliminate
            }

            // Copy each new staff nurse entry
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
        newOrphanIds[0] = staffNurseID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs] = staffNurseID;
        //newOrphanIds[0] = patientID;
        
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

    int error = serializeStaffNurseDB(reducedEntries, metadata.nEntries, &metadata);

    //free(newOrphanIds);
    free(reducedEntries);
    free(loadedEntries);
    free(newOrphanIds);

    return error;
}

// - Uses Linear (naive) search to find a staff nurse entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchStaffNurseByID(int staffNurseID, int nEntries, StaffNurse* staffNurses) {

    if(NULL==staffNurses || nEntries<=0 || staffNurseID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(staffNurseID==staffNurses[i].staffNurseID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find a staff nurse entry
// - Returns the index of the entry, or -1 if not found
int binarySearchStaffNurseByID(int staffNurseID, int nEntries, StaffNurse* staffNurses) {
    if((staffNurseID<0) || (nEntries <=0)) {
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

            if(staffNurses[mid].staffNurseID==staffNurseID) {
                foundIndex=mid;
                break;
            }

            if(staffNurses[mid].staffNurseID < staffNurseID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(staffNurses[0].staffNurseID==staffNurseID) {
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

// - Uses quick sort for sorting staff nurses
// based on their staff nurse ID
void quicksortStaffNurses(StaffNurse *staffNurses, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortStaffNursesPartition(staffNurses, low, high);

        // Recursion on the left
        quicksortStaffNurses(staffNurses, low, partitionIndex-1);
        // Recursion on the right 
        quicksortStaffNurses(staffNurses, partitionIndex+1, high);
    }
}

// This is required by quicksortStaffNurses to define the partition index
int quicksortStaffNursesPartition(StaffNurse *staffNurses, int low, int high) {
    int pivotID = staffNurses[low].staffNurseID;
    int i = low;
    int j = high;

    while(i < j) {

        // find fist element greater than the pivot
        // (from start)
        while(staffNurses[i].staffNurseID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(staffNurses[j].staffNurseID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapStaffNurses(staffNurses, i, j);
        }
    }

    swapStaffNurses(staffNurses, low, j);
    return j;
}

// Used by quicksortStaffNurses too
void swapStaffNurses(StaffNurse* staffNurseArr, int index1, int index2) {
    StaffNurse temp = staffNurseArr[index1];
    staffNurseArr[index1] = staffNurseArr[index2];
    staffNurseArr[index2] = temp; 
}

// Utility 
void printStaffNurseData(StaffNurse staffNurseData) {
    if(0>staffNurseData.staffNurseID) {
        printf("|< NON EXISTANT STAFF NURSE >|\n");
        return;
    }

    printf("| %d | %s | %s | %s | %s |\n", 
    staffNurseData.staffNurseID, staffNurseData.personalID, staffNurseData.name, 
    staffNurseData.password, staffNurseData.functionalUnitStr);
}