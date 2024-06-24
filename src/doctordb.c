#include "include\doctordb.h"

// - Loads the contents of doctor txt file to 
// a memory buffer
// - Returns a dynamically allocated Doctor array,
// which should be deallocated later.
// - nDoctorsOut will receive the number of doctors present
// in the database.
Doctor *deserializeDoctorDB(int *nDoctorsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(DOCTORSSTXT_PATH, "r");
    FILE *metaFile = fopen(DOCTORSMETATXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;

    char lineBuff[100];
    char line1Buff[100];
    char line2Buff[100];
    int lineCounter=0;
    
    // Parse each doctorsMeta.txt line
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
    
	*nDoctorsOut=nEntries;

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

    Doctor outpdata;
    Doctor *loadedEntries = malloc(sizeof(Doctor)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeDoctorDB(): Allocated loaded entries\n");

    int doctorIDinput;
    char personalIDstr[50];
    char nameStr[50];
    char passStr[50];
    char specializationsStr[256];
    //int isLocal;
    
    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %s %s %s %s", //%d", 
        &doctorIDinput, personalIDstr, nameStr, passStr, specializationsStr) //, &isLocal)
        != EOF
    ) {
        Doctor idoctor;
        idoctor.doctorID=doctorIDinput;
        strcpy(idoctor.personalID, personalIDstr);
        strcpy(idoctor.name, nameStr);
        strcpy(idoctor.password, passStr);
        strcpy(idoctor.specializationsStr, specializationsStr);
        /*
            idoctor.isLocal = isLocal;
        */

        loadedEntries[entryCounter] = idoctor;
        entryCounter++;
        //printf("> iterating on dbFile read\n");
    }
    printf("> deserializeDoctorDB(): deserialized into loadedEntries\n");

    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the doctor entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeDoctorDB(Doctor *doctors, int nDoctors, DBMetadata *metadata) {
  if(NULL==doctors || nDoctors<0) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(DOCTORSSTXT_PATH, "r"); 
    FILE *dbMeta = fopen(DOCTORSMETATXT_PATH, "r");
    if(NULL==dbFile || NULL==dbMeta) {
        fclose(dbFile);
        fclose(dbMeta);
        return -1; 
    }
    
    fclose(dbFile);
    fclose(dbMeta);

    // nPatients == 0 means the last entry should be cleared,
    // thus this loop should not be executed
    if(0!=nDoctors) {
        dbFile = fopen(DOCTORSSTXT_PATH, "w");
        dbMeta = fopen(DOCTORSMETATXT_PATH, "w");
        // Update database entries
        for(int i=0; i<nDoctors; i++) {
            printf("");
            fprintf(dbFile, "%d %s %s %s %s\n", //%d\n",
            doctors[i].doctorID, doctors[i].personalID, doctors[i].name,
            doctors[i].password, doctors[i].specializationsStr); // doctors[i].isLocal);
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
// all the doctor entries.
// - Writes number of entries to nEntries ptr
Doctor *getAllDoctors(int *nEntries) {
    DBMetadata metadata;
    Doctor* loadedEntries = deserializeDoctorDB(nEntries, &metadata);
    
    return loadedEntries;   
}

// Loads the value of a doctor entry
Doctor getDoctor(int doctorID) {
 int nEntries;
    DBMetadata metadata;
    Doctor* loadedEntries = deserializeDoctorDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getDoctor(): Loaded entries\n");
    Doctor outpdata;

    if(-1==doctorID) {
        outpdata.doctorID=-1;
        return outpdata;
    }

    // Binary search the doctor data
    int foundIndex = //linearSearchDoctorByID((int)doctorID, nEntries, loadedEntries);//binarySearchPatient((int)patientID, nEntries, loadedEntries);
    binarySearchDoctorByID((int)doctorID, nEntries, loadedEntries);
    
    if(-1!=foundIndex) {
        outpdata.doctorID = loadedEntries[foundIndex].doctorID;
        strcpy(outpdata.personalID, loadedEntries[foundIndex].personalID);
        strcpy(outpdata.name, loadedEntries[foundIndex].name);
        strcpy(outpdata.password, loadedEntries[foundIndex].password);
        strcpy(outpdata.specializationsStr, loadedEntries[foundIndex].specializationsStr);
        //outpdata.isLocal = loadedEntries[foundIndex].isLocal;
    }
    else {
        outpdata.doctorID=-1;
    }

    free(loadedEntries);
    printf("> getDoctor(): finished\n");
    return outpdata;
}

// - Sets the value of an existing doctor entry
// - Returns -1 if an error ocurred
int setDoctor(int doctorID, Doctor newDoctorData) {

    int error = 
        (doctorID<0) || (NULL==newDoctorData.personalID) || 
        (NULL==newDoctorData.name) || (NULL==newDoctorData.password); 
        //|| ((1!=newDoctorData.isLocal) && (0!=newPatientData.isLocal)); 

    // If previous condition is 1, toggle it
    // to -1. 0 toggles to 0, thus, no errors.
    error=-error;
    if(-1==error) return error;

    int nEntries;
    DBMetadata metadata={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    Doctor* loadedEntries = deserializeDoctorDB(&nEntries, &metadata);

    // Search for the patientID in the loaded database
    int foundIndex = linearSearchDoctorByID(doctorID, nEntries, loadedEntries);//binarySearchPatient(patientID, nEntries, loadedEntries);

    // Insert the new value
    if(-1!=foundIndex) {
        loadedEntries[foundIndex] = newDoctorData;
        // If insertion was successful, sort the array
        // ... (do this later in addPatient when using an orphanID)
    }
    else {
        return -1;
    }

    // Rewrite the array to file and modify metadata
    // Return the error/success status of serialization
    error = serializeDoctorDB(loadedEntries, nEntries, &metadata);    
    free(loadedEntries);
    return error;
}

// - Adds a new doctor entry
// - Returns the doctor ID of the new doctor entered
int addDoctor(Doctor newDoctorData) {

    int error = 
        //(newPatientData.patientID<0) || 
        (NULL==newDoctorData.personalID) || 
        (NULL==newDoctorData.name) || (NULL==newDoctorData.password); 
        //|| ((1!=newPatientData.isLocal) && (0!=newPatientData.isLocal)); 
    
    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;//={.nEntries=0,.nOrphanIDs=0,.orphanIDs=0};
    Doctor* loadedEntries = deserializeDoctorDB(&nEntries, &metadata);

    // Increase array size
    Doctor* extendedEntries = malloc(sizeof(Doctor)*(nEntries+1));
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
        newDoctorData.doctorID = outID;
        //extendedEntries[outID] = newPatientData; // Append a new entry at the bottom
    }
    newDoctorData.doctorID=outID;
    extendedEntries[outID] = newDoctorData;
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortDoctors(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeDoctorDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes a doctor entry
// - Returns -1 if error
int removeDoctor(int doctorID) {
    if(doctorID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    Doctor* loadedEntries = deserializeDoctorDB(&nEntries, &metadata);

    int foundIndex = linearSearchDoctorByID(doctorID, nEntries, loadedEntries);//binarySearchPatient(patientID, nEntries, loadedEntries);
    
    if(-1==foundIndex)
        return -1;

    Doctor* reducedEntries;
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(Doctor)*(nEntries-1));
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
        newOrphanIds[0] = doctorID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs-1] = doctorID;
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

    int error = serializeDoctorDB(reducedEntries, metadata.nEntries, &metadata);

    //free(newOrphanIds);
    free(reducedEntries);
    free(loadedEntries);

    return error;   
}

// - Uses Linear (naive) search to find a doctor entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchDoctorByID(int doctorID, int nEntries, Doctor* doctors) {
    if(NULL==doctors || nEntries<=0 || doctorID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(doctorID==doctors[i].doctorID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find a doctor entry
// - Returns the index of the entry, or -1 if not found
int binarySearchDoctorByID(int doctorID, int nEntries, Doctor* doctors) {
    if((doctorID<0) || (nEntries <=0)) {
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

            if(doctors[mid].doctorID==doctorID) {
                foundIndex=mid;
                break;
            }

            if(doctors[mid].doctorID < doctorID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(doctors[0].doctorID==doctorID) {
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

// - Uses quick sort for sorting doctors
// based on their doctorID
void quicksortDoctors(Doctor *doctors, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortDoctorsPartition(doctors, low, high);

        // Recursion on the left
        quicksortDoctors(doctors, low, partitionIndex-1);
        // Recursion on the right 
        quicksortDoctors(doctors, partitionIndex+1, high);
    }
}

// This is required by quicksortDoctors to define the partition index
int quicksortDoctorsPartition(Doctor *doctors, int low, int high) {
    int pivotID = doctors[low].doctorID;
    int i = low;
    int j = high;

    while(i < j) {

        // find fist element greater than the pivot
        // (from start)
        while(doctors[i].doctorID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(doctors[j].doctorID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapDoctors(doctors, i, j);
        }
    }

    swapDoctors(doctors, low, j);
    return j;
}

// Used by quicksortDoctors too
void swapDoctors(Doctor* doctorArr, int index1, int index2) {
    Doctor temp = doctorArr[index1];
    doctorArr[index1] = doctorArr[index2];
    doctorArr[index2] = temp;  
}

// Utility 
void printDoctorData(Doctor doctorData) {
    if(0>doctorData.doctorID) {
        printf("|< NON EXISTANT DOCTOR >|\n");
        return;
    }

    printf("| %d | %s | %s | %s | %s |\n", 
    doctorData.doctorID, doctorData.personalID, doctorData.name, 
    doctorData.password,
    doctorData.specializationsStr
    );// doctorData.isLocal);
}