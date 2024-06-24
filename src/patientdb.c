#include "include\patientdb.h"

// - Loads the contents of patient txt file to 
// a memory buffer
// - Returns a dynamically allocated Patient array,
// which should be deallocated later.
// - nPatientsOut will receive the number of patients present
// in the database.
// - metadata will receive status data about the database which is later needed
// for serialization
Patient *deserializePatientDB(int *nPatientsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(PATIENTSTXT_PATH, "r");
    FILE *metaFile = fopen(PATIENTSMETATXT_PATH, "r");

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
    
	*nPatientsOut=nEntries;

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

    Patient outpdata;
    Patient *loadedEntries = malloc(sizeof(Patient)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializePatientDB(): Allocated loaded entries\n");

    int patientIDinput;
    char personalIDstr[50];
    char nameStr[50];
    char passStr[50];
    int isLocal;
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %s %s %s %d", 
        &patientIDinput, personalIDstr, nameStr, passStr, &isLocal)
        != EOF
    ) {
        Patient ipatient;
            ipatient.pkID=patientIDinput;
            strcpy(ipatient.personalID, personalIDstr);
            strcpy(ipatient.name, nameStr);
            strcpy(ipatient.password, passStr);
            ipatient.isLocal = isLocal;

        loadedEntries[entryCounter] = ipatient;
        entryCounter++;
        //printf("> iterating on dbFile read\n");
    }
    printf("> deserializePatientDB(): deserialized into loadedEntries\n");

    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the patient entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializePatientDB(Patient *patients, int nPatients, DBMetadata *metadata) {
    if(NULL==patients || nPatients<0) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(PATIENTSTXT_PATH, "r"); 
    FILE *dbMeta = fopen(PATIENTSMETATXT_PATH, "r");
    if(NULL==dbFile || NULL==dbMeta) {
        fclose(dbFile);
        fclose(dbMeta);
        return -1; 
    }
    
    fclose(dbFile);
    fclose(dbMeta);

    // nPatients == 0 means the last entry should be cleared,
    // thus this loop should not be executed
    if(0!=nPatients) {
        dbFile = fopen(PATIENTSTXT_PATH, "w");
        dbMeta = fopen(PATIENTSMETATXT_PATH, "w");
        // Update database entries
        for(int i=0; i<nPatients; i++) {
            printf("");
            fprintf(dbFile, "%d %s %s %s %d\n",
            patients[i].pkID, patients[i].personalID, patients[i].name,
            patients[i].password, patients[i].isLocal);
        }
    }

    //Update metadata, even when nPatients is 0
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
// all the student entries.
// - Writes number of entries to nEntries ptr
Patient *getAllPatients(int *nEntries) {
    DBMetadata metadata;
    Patient* loadedEntries = deserializePatientDB(nEntries, &metadata);
    
    return loadedEntries;
}

// Loads the value of a patient entry
Patient getPatient(int patientID) {
    int nEntries;
    DBMetadata metadata;
    Patient* loadedEntries = deserializePatientDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getPatient(): Loaded entries\n");
    Patient outpdata;

    if(-1==patientID) {
        outpdata.pkID=-1;
        return outpdata;
    }

    // Binary search the patient data
    int foundIndex = linearSearchPatientByID((int)patientID, nEntries, loadedEntries);//binarySearchPatient((int)patientID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.pkID = loadedEntries[foundIndex].pkID;
        strcpy(outpdata.personalID, loadedEntries[foundIndex].personalID);
        strcpy(outpdata.name, loadedEntries[foundIndex].name);
        strcpy(outpdata.password, loadedEntries[foundIndex].password);
        outpdata.isLocal = loadedEntries[foundIndex].isLocal;
    }
    else {
        outpdata.pkID=-1;
    }

    free(loadedEntries);
    printf("> getPatient(): finished\n");
    return outpdata;
}

// - Sets the value of an existing patient entry
// - Returns -1 if an error ocurred
int setPatient(int patientID, Patient newPatientData) {   

    int error = 
        (patientID<0) || (NULL==newPatientData.personalID) || 
        (NULL==newPatientData.name) || (NULL==newPatientData.password) || 
        ((1!=newPatientData.isLocal) && (0!=newPatientData.isLocal)); 

    // If previous condition is 1, toggle it
    // to -1. 0 toggles to 0, thus, no errors.
    error=-error;
    if(-1==error) return error;

    int nEntries;
    DBMetadata metadata={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    Patient* loadedEntries = deserializePatientDB(&nEntries, &metadata);

    // Search for the patientID in the loaded database
    int foundIndex = linearSearchPatientByID(patientID, nEntries, loadedEntries);//binarySearchPatient(patientID, nEntries, loadedEntries);

    // Insert the new value
    if(-1!=foundIndex) {
        loadedEntries[foundIndex] = newPatientData;
        // If insertion was successful, sort the array
        // ... (do this later in addPatient when using an orphanID)
    }
    else {
        return -1;
    }

    // Rewrite the array to file and modify metadata
    // Return the error/success status of serialization
    error = serializePatientDB(loadedEntries, nEntries, &metadata);    
    free(loadedEntries);
    return error;
}

// - Uses Linear (naive) search to find a patient entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearchPatient fails
int linearSearchPatientByID(int patientID, int nEntries, Patient* patients) {

    if(NULL==patients || nEntries<=0 || patientID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(patientID==patients[i].pkID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find a patient entry
// - Returns the index of the entry, or -1 if not found
int binarySearchPatientByID(int patientID, int nEntries, Patient* patients) {
    if((patientID<0) || (nEntries <=0)) {
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

            if(patients[mid].pkID==patientID) {
                foundIndex=mid;
                break;
            }

            if(patients[mid].pkID < patientID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(patients[0].pkID==patientID) {
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

// - Adds a new patient entry (appends it)
// - Returns the patient ID of the new patient entered.
// - Returns -1 if an error occurred
// - The patientID in newPatientData will be overriden by a new
// given ID
int addPatient(Patient newPatientData) {

    int error = 
        //(newPatientData.patientID<0) || 
        (NULL==newPatientData.personalID) || 
        (NULL==newPatientData.name) || (NULL==newPatientData.password) || 
        ((1!=newPatientData.isLocal) && (0!=newPatientData.isLocal)); 
    
    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;//={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    Patient* loadedEntries = deserializePatientDB(&nEntries, &metadata);

    // Increase array size
    Patient* extendedEntries = malloc(sizeof(Patient)*(nEntries+1));
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
        newPatientData.pkID = outID;
        //extendedEntries[outID] = newPatientData; // Append a new entry at the bottom
    }
    newPatientData.pkID=outID;
    extendedEntries[outID] = newPatientData;
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortPatients(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializePatientDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

int removePatient(int patientID) {
    if(patientID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    Patient* loadedEntries = deserializePatientDB(&nEntries, &metadata);

    int foundIndex = linearSearchPatientByID(patientID, nEntries, loadedEntries);//binarySearchPatient(patientID, nEntries, loadedEntries);
    
    if(-1==foundIndex)
        return -1;

    Patient* reducedEntries;
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(Patient)*(nEntries-1));
    }

    int prevArrCounter=0;
    int reducedArrCounter=0;
    if(NULL!=reducedEntries) {
        while((reducedArrCounter<nEntries-1) && (prevArrCounter<nEntries)) {
            if(prevArrCounter == foundIndex) {
                prevArrCounter++; // Skips the entry to eliminate
            }

            // Copy each new patient entry
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
        newOrphanIds[0] = patientID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs] = patientID;
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

    int error = serializePatientDB(reducedEntries, metadata.nEntries, &metadata);

    //free(newOrphanIds);
    free(reducedEntries);
    free(loadedEntries);
    free(newOrphanIds);

    return error;
}

// - Uses quick sort for sorting patients
// based on their patientID
void quicksortPatients(Patient *patients, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortPatientsPartition(patients, low, high);

        // Recursion on the left
        quicksortPatients(patients, low, partitionIndex-1);
        // Recursion on the right 
        quicksortPatients(patients, partitionIndex+1, high);
    }
}

int quicksortPatientsPartition(Patient *patients, int low, int high) {
    int pivotID = patients[low].pkID;
    int i = low;
    int j = high;

    while(i < j) {

        // find fist element greater than the pivot
        // (from start)
        while(patients[i].pkID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(patients[j].pkID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapPatients(patients, i, j);
        }
    }

    swapPatients(patients, low, j);
    return j;
}

void swapPatients(Patient* patientArr, int index1, int index2) {
    Patient temp = patientArr[index1];
    patientArr[index1] = patientArr[index2];
    patientArr[index2] = temp; 
}

void printPatientData(Patient patientData) {
    if(0>patientData.pkID) {
        printf("|< NON EXISTANT PATIENT >|\n");
        return;
    }

    printf("| %d | %s | %s | %s | %d |\n", 
    patientData.pkID, patientData.personalID, patientData.name, 
    patientData.password, patientData.isLocal);
}