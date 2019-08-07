#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#define FOUND 1
#define MISSING 0

#define MAX_SIZE 1024 * 2

#define ONE_FILE 0
#define MANY_FILES 1

#define ASTERISK 0x2A
#define CR 0x0D
#define LF 0x0A
#define SPACE 0x20
#define TAB 0x09
#define WILDCARD ASTERISK

char searchString[MAX_SIZE];
char stringBuffer[MAX_SIZE];

char filename[256];
char expression[256];
int option;

int findThisInThat(char *thisString, char *thatString);

void processFile();

int countThisInThat(char thisChar, char *thatString);

void main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("\n");
		printf("Use: search <filename> <string>");
		printf("\n");
		exit(1);
	}
	else
	{
		strcpy_s(searchString, MAX_SIZE, argv[2]);
		strcpy_s(expression, 256, argv[1]);
		if (countThisInThat(WILDCARD, expression) == 0)
		{
			strcpy_s(filename, 256, argv[1]);
			processFile();
		}
		else
		{
			// int findrc;
			// struct _finddata_t findit;
			// long findhandle;

			printf("Wildcard function is not available at this time.");
		}
	}
}

int countThisInThat(char thisChar, char *thatString)
{
	int thatStringLen = strlen(thatString);
	int loopNum = thatStringLen - 1;
	int count = 0;
	for (int index = 0; index <= loopNum; index++)
	{
		if (thatString[index] == thisChar)
		{
			count++;
		}
	}
	return count;
}

void processFile()
{
	FILE *fptr;
	fopen_s(&fptr, filename, "r");
	if (fptr == NULL)
	{
		printf("Error opening %s\n", filename);
		exit(1);
	}
	else
	{
		printf("Searching file %s\n", filename);
		int index = 1;
		int numberFound = 0;
		while (!feof(fptr))
		{
			fgets(stringBuffer, MAX_SIZE, fptr);
			if (findThisInThat(searchString, stringBuffer) == FOUND)
			{
				numberFound++;
			}
			index++;
		}
		if (numberFound == 0)
		{
			printf("No occurrences found.");
		}
	}
}

int findThisInThat(char *thisString, char *thatString)
{
	int thatStringLen = strlen(thatString);
	int thisStringLen = strlen(thisString);
	int loopNum = thatStringLen - thisStringLen;
	if (thatString[thatStringLen - 1] == '\n')
	{
		thatString[thatStringLen - 1] = 0x00;
		thatStringLen--;
	}
	if (thatStringLen >= thisStringLen)
	{
		for (int index = 0; index <= loopNum; index++)
		{
			if (!(memcmp(&thatString[index], thisString, thisStringLen)))
			{
				// printf("Found on line %d:\n", index);
				printf("%s\n", stringBuffer);
				return FOUND;
			}
		}
	}
	return MISSING;
}