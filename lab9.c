#include <stdio.h>
#include <stdlib.h>

#define SIZE 100

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

// Fill out this structure
struct HashType
{
	struct HashType *next;
	struct RecordType* records;
	int size;
};

// Compute the hash function
int hash(int x)
{
	return x%23;
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
		// Implement parse data block
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
	int i;

	for (i=0;i<hashSz;++i)
	{
		// if index is occupied with any records, print all
		if(pHashArray[i].records!=NULL && pHashArray[i].records->id != -1){
			printf("index %d ", i);
			struct HashType *pTemp = &pHashArray[i];
			while(pTemp != NULL && pTemp->records != NULL && pTemp->records->id !=-1){
				printf("-> %d, %c, %d ", pTemp->records->id, pTemp->records->name, pTemp->records->order);
				pTemp = pTemp->next;
			}
		}
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation

	//declare and initialize hashtable array
	struct HashType *pHashArray = (struct HashType*)malloc(sizeof(struct HashType)*SIZE);
	for(int i = 0; i < SIZE; i++){
		pHashArray[i].records = (struct RecordType*)malloc(sizeof(struct RecordType));
		pHashArray[i].records->id = -1;
		pHashArray[i].records->name = '\0';
		pHashArray[i].records->order = -1;
		pHashArray[i].size = 0;
		pHashArray[i].next = NULL;
	}

	//iteratre through records and transfer to hash table array
	for(int i = 0; i < recordSz; i++){
		int index = hash(pRecords[i].id);
		struct HashType * pCurrent = &pHashArray[index];
		
		//if the current index is empty, add the records to be the first in its order
		if(pCurrent->records->id == -1){
			pCurrent->records = &pRecords[i];
		}
		//otherwise, iterate through records until the end is reached and insert records into next empty slot
		else{
			while(pCurrent->next!= NULL)
				pCurrent = pCurrent ->next;

			pCurrent->next = (struct HashType*)malloc(sizeof(struct HashType));
			pCurrent->next->records = &pRecords[i];
			pCurrent->next->size=1;
			pCurrent->next->next=NULL;
		}
	}
	//display the records in hash table array
	displayRecordsInHash(pHashArray, recordSz);

	return 0;
}