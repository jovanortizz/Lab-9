#include <stdio.h>
#include <stdlib.h>

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order;
};

// Fill out this structure
struct HashType
{
    struct RecordType* data;
};

// Compute the hash function
int hash(int x)
{
    return x % 10; // Simple hash function for demonstration
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i, j;

    for (i = 0; i < hashSz; ++i)
    {
        if (pHashArray[i].data != NULL)
        {
            printf("Index %d -> ", i);
            j = 0;
            while (pHashArray[i].data[j].id != -1) // Assuming -1 indicates end of records for simplicity
            {
                printf("%d %c %d -> ", pHashArray[i].data[j].id, pHashArray[i].data[j].name, pHashArray[i].data[j].order);
                ++j;
            }
            printf("\n");
        }
    }
}

int main(void)
{
    struct RecordType *pRecords;
    struct HashType hashTable[10]; // Assuming hash table size of 10 for simplicity
    int recordSz = 0;
    int i, index;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize hash table
    for (i = 0; i < 10; ++i)
    {
        hashTable[i].data = NULL;
    }

    // Insert records into hash table
    for (i = 0; i < recordSz; ++i)
    {
        index = hash(pRecords[i].id);
        if (hashTable[index].data == NULL)
        {
            hashTable[index].data = (struct RecordType*) malloc(sizeof(struct RecordType) * (recordSz + 1)); // Extra space for possible future inserts
            if (hashTable[index].data == NULL)
            {
                printf("Cannot allocate memory\n");
                exit(-1);
            }
            hashTable[index].data[0].id = -1; // Initialize end marker
        }

        // Find the next available slot
        int j;
        for (j = 0; hashTable[index].data[j].id != -1; ++j)
        {
            if (j == recordSz)
            {
                printf("Hash table is full\n");
                exit(-1);
            }
        }

        // Insert record
        hashTable[index].data[j].id = pRecords[i].id;
        hashTable[index].data[j].name = pRecords[i].name;
        hashTable[index].data[j].order = pRecords[i].order;
        hashTable[index].data[j + 1].id = -1; // Update end marker
    }

    // Display records in hash table
    displayRecordsInHash(hashTable, 10);

    // Free dynamically allocated memory
    for (i = 0; i < 10; ++i)
    {
        if (hashTable[i].data != NULL)
        {
            free(hashTable[i].data);
        }
    }
    free(pRecords);

    return 0;
}

