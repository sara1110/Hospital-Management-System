#include "include\metadataMng.h"

// - Reads the content of an specific metadata file
// - Stores the data into a DBmetadata struct
void deserializeMetadata(DBMetadata *targetMetadata, const char *metadataFilePath) {

    FILE *metaFile = fopen(metadataFilePath, "r");

    // Read metadata first
    int nEntries, nOrphanIDs;

    char lineBuff[100];
    char line1Buff[100];
    char line2Buff[100];
    int lineCounter=0;
    
    // Parse each inventoryMeta.txt line
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

    targetMetadata->nEntries=nEntries;
    targetMetadata->nOrphanIDs=nOrphanIDs;
	
	// avoid allocating '0 bytes'
    targetMetadata->orphanIDs = (nOrphanIDs > 0) ? 
		malloc(sizeof(int) * nOrphanIDs) : (NULL);
    
	//*nInvItemsOut=nEntries;

    if(targetMetadata->nOrphanIDs>0) {
        // Parse the orphanIDs array
        // (Load the orphan IDs as an array)
        char wordBuff[10];
        // Setup the wordBuff to an empty string
        for(int cc=0;cc<sizeof(wordBuff);cc++) 
            wordBuff[cc]=' ';

        // do this just in case
        wordBuff[9]='\0';

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
                    targetMetadata->orphanIDs[idCounter]=newOrphanID;
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
        printf("%d, ", targetMetadata->orphanIDs[i]);
    }
    printf(" }\n");
    
    //free(targetMetadata->orphanIDs);
    fclose(metaFile); 
}

// - Saves the contents of a DBMetadata struct
// - Returns -1 if errors, 0 if no errors 
int serializeMetadata(DBMetadata *targetMetadata, int nInvTypes, const char *metadataFilePath) {

    FILE *dbMeta = fopen(metadataFilePath, "r");
    if(NULL==dbMeta) {
        fclose(dbMeta);
        return -1;
    }

    fclose(dbMeta);

    if(nInvTypes != 0) {
        dbMeta = fopen(metadataFilePath, "w");

        //Update metadata, even when nInvTypes is 0
        fprintf(dbMeta, "%d %d\n", targetMetadata->nEntries, targetMetadata->nOrphanIDs);

        // Serialize orphanIDs 
        if(targetMetadata->nOrphanIDs>0){
            // Use the preexisting orphanIDs         
            if(NULL!=targetMetadata->orphanIDs) {
                for(int i=0; i<targetMetadata->nOrphanIDs;i++) {
                    int iOrphan = targetMetadata->orphanIDs[i];
                    fprintf(dbMeta, "%d ", iOrphan);
                }
                fprintf(dbMeta, "\n");
            }
        }    
        fclose(dbMeta);
    }

    return 0;
}
