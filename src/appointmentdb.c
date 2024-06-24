#include "include\appointmentdb.h"

// - Loads the contents of appoitnments txt file to 
// a memory buffer
// - Returns a dynamically allocated Appointment array, 
// which should be deallocated later.
// - nAppointnments will receive the number of appointments present
// in the database.
Appointment *deserializeAppointmentDB(int *nAppointmentsOut, DBMetadata *metadata) {
    FILE *dbFile = fopen(APPOINTMENTTXT_PATH, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;
    
    deserializeMetadata(metadata, APPPOINTMENTMETATXT_PATH);
    nEntries = metadata->nEntries;
    *nAppointmentsOut=nEntries;
    nOrphanIDs = metadata->nOrphanIDs;

    Appointment outpdata;
    Appointment *loadedEntries = malloc(sizeof(Appointment)*nEntries);
    printf("> nEntries = %d\n", nEntries);
    printf("> deserializeAppointmentDB(): Allocated loaded entries\n");

    int appointmentID;
    int patientID;
    int doctorID;
    Date appointmentDay;
    char appointmentDayStr[9];
    DayTime startTime;
    char startTimeStr[7];

    // Deserialize the contents into loadedEntries Array
    int entryCounter = 0;
    while(
        fscanf(dbFile ,"%d %d %d %s %s",
        &appointmentID, &patientID, &doctorID, appointmentDayStr, startTimeStr)
        != EOF
    ) {
        Appointment iappointment;
        iappointment.appointmentID = appointmentID;
        iappointment.patientID = patientID;
        iappointment.doctorID = doctorID;

        // Convert from string to Date
        appointmentDayStr[8] ='\0';
        strncpy((char*)appointmentDay.dateInChars.dayDigits, (char*)appointmentDayStr, 2);
        strncpy((char*)appointmentDay.dateInChars.monthDigits, (char*)(appointmentDayStr+2), 2);
        strncpy((char*)appointmentDay.dateInChars.yearDigits, (char*)(appointmentDayStr+4), 4);
        iappointment.appointmentDay = appointmentDay;

        // Convert from string to DayTime
        startTimeStr[6]='\0';
        strncpy((char*)startTime.dayTimeInChars.hours, (char*)startTimeStr, 2);
        strncpy((char*)startTime.dayTimeInChars.minutes, (char*)(startTimeStr+2), 2);
        strncpy((char*)startTime.dayTimeInChars.seconds, (char*)(startTimeStr+4), 2);
        iappointment.startTime = startTime;

        loadedEntries[entryCounter] = iappointment;
        entryCounter++;
    }
    printf("> deserializeAppointmentDB(): deserialized into loadedEntries\n");

    free(metadata->orphanIDs); // dont forget to deallocate
    fclose(dbFile);
    return loadedEntries;
}

// - Rewrites the file containing the appointment entries.
// - Returns -1 if an error occurred, otherwise returns 0.
int serializeAppointmentDB(Appointment *appointments, int nAppointments, DBMetadata *metadata) {
    if(NULL==appointments || nAppointments<0) return -1;

    int metadataState = serializeMetadata(metadata, nAppointments, APPPOINTMENTMETATXT_PATH);

    if(-1==metadataState) return -1;

    // check that the file exists first
    FILE *dbFile = fopen(APPOINTMENTTXT_PATH, "r"); 

    if(NULL==dbFile) {
        fclose(dbFile);
        return -1;
    }

    fclose(dbFile);

    if(0!=nAppointments) {

        dbFile = fopen(APPOINTMENTTXT_PATH, "w");

        // Update database entries
        for(int i=0; i<nAppointments; i++) {
            printf("");

            // Convert Date to string
            char dateStr[9];
            dateStr[8]='\0';
            strncpy((char*)dateStr, appointments[i].appointmentDay.dateInChars.dayDigits, 2);
            strncpy((char*)(dateStr+2), appointments[i].appointmentDay.dateInChars.monthDigits, 2);
            strncpy((char*)(dateStr+4), appointments[i].appointmentDay.dateInChars.yearDigits, 4);

            // Convert DayTime to string
            char startTimeStr[7];
            startTimeStr[6]='\0';
            strncpy((char*)startTimeStr, appointments[i].startTime.dayTimeInChars.hours, 2);
            strncpy((char*)(startTimeStr+2), appointments[i].startTime.dayTimeInChars.minutes, 2);
            strncpy((char*)(startTimeStr+4), appointments[i].startTime.dayTimeInChars.seconds, 2);

            fprintf(dbFile, "%d %d %d %s %s\n",
                appointments[i].appointmentID, appointments[i].patientID,
                appointments[i].doctorID, dateStr, startTimeStr
            );
        }
        fclose(dbFile);
    }

    return 0;
}

// - Returns a dynamically allocated array containing
// all the appointment entries.
// - Writes number of entries to nEntries ptr
Appointment *getAllAppointments(int *nEntries) {
    DBMetadata metadata;
    int nEntriesLocal;
    Appointment *loadedEntries = deserializeAppointmentDB(&nEntriesLocal, &metadata);
    *nEntries=nEntriesLocal;

    return loadedEntries;
}

// Loads the value of an appointment entry
Appointment getAppointment(int appointmentID) {
    int nEntries;
    DBMetadata metadata;
    Appointment* loadedEntries = deserializeAppointmentDB(&nEntries, &metadata);
    //nEntries=metadata.nEntries;
    printf("> getAppointmentItem(): Loaded entries\n");
    Appointment outpdata;

    if(-1==appointmentID) {
        outpdata.appointmentID=-1;
        return outpdata;
    }

    // Binary search the doctor data
    int foundIndex = 
    //binarySearchAppointmentByID((int)appointmentID, nEntries, loadedEntries);
    linearSearchAppointmentByID((int)appointmentID, nEntries, loadedEntries);

    if(-1!=foundIndex) {
        outpdata.appointmentID = loadedEntries[foundIndex].appointmentID;
        outpdata.patientID = loadedEntries[foundIndex].patientID;
        outpdata.doctorID= loadedEntries[foundIndex].doctorID;
        outpdata.appointmentDay = loadedEntries[foundIndex].appointmentDay;
        outpdata.startTime = loadedEntries[foundIndex].startTime;
    }
    else {
        outpdata.appointmentID=-1;
    }

    free(loadedEntries);
    printf("> getAppointment(): finished\n");
    return outpdata;
}

// - Sets the value of an existing appointment entry
// - Returns -1 if an error ocurred
/*
int setAppointment(int appointmentID, Appointment newAppointmentData) {

}
*/

// - Adds a new appointment entry
// - Returns the appointment ID of the new appointment item entered
int addAppointment(Appointment newAppointmentData) {

    int error =  
        (0>newAppointmentData.patientID) || 
        (0>newAppointmentData.doctorID); 
        //|| 
        //(NULL==newAppointmentData.appointmentDay.dateInChars) ||
        //(NULL==newAppointmentData.startTime);

    error=-error;
    if(-1==error) return error;

    // output
    int outID;

    int nEntries;
    DBMetadata metadata;
    Appointment* loadedEntries = deserializeAppointmentDB(&nEntries, &metadata);

    // Increase array size
    Appointment* extendedEntries = malloc(sizeof(Appointment)*(nEntries+1));
    
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
        newAppointmentData.appointmentID = outID;
    }
    newAppointmentData.appointmentID=outID;
    extendedEntries[outID] =newAppointmentData; // Append a new entry at the bottom
    metadata.nEntries+=1;

    // Sort the extended array
    quicksortAppointments(extendedEntries, 0, nEntries);

    // Finally serialize the extended array,
    // and return its error status
    error = serializeAppointmentDB(extendedEntries, nEntries+1, &metadata);
    free(loadedEntries);
    free(extendedEntries);

    if(-1==error) return error;

    return outID;
}

// - Removes an appointment entry
// - Returns -1 if error
int removeAppointment(int appointmentID) {
    if(appointmentID < 0) {
        return -1;
    }

    int nEntries;
    DBMetadata metadata;
    Appointment* loadedEntries = deserializeAppointmentDB(&nEntries, &metadata);

    int foundIndex =  linearSearchAppointmentByID(appointmentID, nEntries, loadedEntries);
    //binarySearchAppointmentByID(invItemID, nEntries, loadedEntries);

    if(-1==foundIndex)
        return -1;

    Appointment* reducedEntries;
    
    // if only one entry left to eliminate, which is
    // the target
    if(0==foundIndex && 0>=nEntries) {
        reducedEntries = NULL;
    }
    else {
        reducedEntries = malloc(sizeof(Appointment)*(nEntries-1));
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
        newOrphanIds[0] = appointmentID;
    } 
    else {
        newOrphanIds = malloc(sizeof(int)*(metadata.nOrphanIDs+1));
        newOrphanIds[metadata.nOrphanIDs-1] = appointmentID;
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

    int error = serializeAppointmentDB(reducedEntries, metadata.nEntries, &metadata);

    free(reducedEntries);
    free(loadedEntries);

    return error;
}

// - Uses Linear (naive) search to find an appointment entry
// - Returns the index of the entry, or -1 if not found
// - Should be used only in case binarySearch fails
int linearSearchAppointmentByID(int appointmentID, int nEntries, Appointment* appointments) {
    if(NULL==appointments || nEntries<=0 || appointmentID<0) {
        return -1;
    }

    int wasFound = 0;
    int foundIndex = 0;
    for(int i=0; i<nEntries; i++) {
        if(appointmentID==appointments[i].appointmentID) {
            wasFound=1;
            foundIndex = i;
            break;
        }
    }

    return (1==wasFound) ? foundIndex : -1;
}

// - Uses Binary Search to find an appointment entry
// - Returns the index of the entry, or -1 if not found
int binarySearchAppointmentByID(int appointmentID, int nEntries, Appointment* appointments) {
    if((appointmentID<0) || (nEntries <=0)) {
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

            if(appointments[mid].appointmentID==appointmentID) {
                foundIndex=mid;
                break;
            }

            if(appointments[mid].appointmentID < appointmentID) {
                lowp=mid+1; // Go to the right side
            }
            else {
                highp=mid-1;
            }
        }
        
        return foundIndex;
    }
    else if(1==nEntries) {
        if(appointments[0].appointmentID==appointmentID) {
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

// - Uses quick sort for sorting appointment items
// based on their appointmentID
void quicksortAppointments(Appointment *appointments, int low, int high) {
    if(low < high) {
        
        // call partition fucntion to find partition index
        int partitionIndex = quicksortAppointmentPartition(appointments, low, high);

        // Recursion on the left
        quicksortAppointments(appointments, low, partitionIndex-1);
        // Recursion on the right 
        quicksortAppointments(appointments, partitionIndex+1, high);
    }
}

// This is required by quicksortAppointment to define the partition index
int quicksortAppointmentPartition(Appointment *appointments, int low, int high) {
    int pivotID = appointments[low].appointmentID;
    int i = low;
    int j = high;

    while(i < j) {
        // find fist element greater than the pivot
        // (from start)
        while(appointments[i].appointmentID <= pivotID 
        && i <= high -1) {
            i++;
        }
    
        // find the first element smaller than the pivot
        // (from last)
        while(appointments[j].appointmentID > pivotID 
        && j >= low + 1) {
            j--;
        }

        if(i < j) {
            swapAppointments(appointments, i, j);
        }
    }

    swapAppointments(appointments, low, j);
    return j;
}

// Used by quicksortAppointment too
void swapAppointments(Appointment *appointments, int index1, int index2) {
    Appointment temp = appointments[index1];
    appointments[index1] = appointments[index2];
    appointments[index2] = temp;
}

// Utility 
void printAppointmentData(Appointment appointmentData) {
     if(0>appointmentData.appointmentID) {
        printf("|< NON EXISTANT APPOINTMENT >|\n");
        return;
    }

    char dateStr[9];
    dateStr[8]='\0';
    strncpy((char*)dateStr, appointmentData.appointmentDay.dateInChars.dayDigits, 2);
    strncpy((char*)(dateStr+2), appointmentData.appointmentDay.dateInChars.monthDigits, 2);
    strncpy((char*)(dateStr+4), appointmentData.appointmentDay.dateInChars.yearDigits, 4);

    char startTimeStr[7];
    startTimeStr[6]='\0';
    strncpy((char*)startTimeStr, appointmentData.startTime.dayTimeInChars.hours, 2);
    strncpy((char*)(startTimeStr+2), appointmentData.startTime.dayTimeInChars.minutes, 2);
    strncpy((char*)(startTimeStr+4), appointmentData.startTime.dayTimeInChars.seconds, 2);

    printf("| %d | %d | %d | %s | %s |\n", 
    appointmentData.appointmentID, appointmentData.patientID,
    appointmentData.doctorID, dateStr, startTimeStr);   
}