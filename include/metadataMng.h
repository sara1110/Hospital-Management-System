#pragma once
#ifndef METADATA_MNG_H
#define METADATA_MNG_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include\dataDefs.h"

// This structure represents the status of an 
// specific database
typedef struct DBmetaStrc {
    int nEntries;
    int *orphanIDs;
    int nOrphanIDs;
} DBMetadata;

// - Reads the content of an specific metadata file
// - Stores the data into a DBmetadata struct
void deserializeMetadata(DBMetadata *targetMetadata, const char *metadataFilePath);

/// - Saves the contents of a DBMetadata struct
int serializeMetadata(DBMetadata *targetMetadata, int nInvTypes, const char *metadataFilePath);

#endif // METADATA_MNG_H