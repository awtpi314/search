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

int findrc = 0;
struct _finddata_t findit;
long findhandle;

int fileCount = 0;

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
		/* OLD VERSION USING SEPERATE CASE FOR NON-WILDCARD FILE SEARCH PATHS
		if (countThisInThat(WILDCARD, expression) == 0)
		{
			strcpy_s(filename, 256, argv[1]);
			processFile();
		}
		else
		{
		   END OLD VERSION */
		findhandle = _findfirst(expression, &findit);
		if (findhandle > 0)
		{
			fileCount++;
			strcpy_s(filename, 256, findit.name);
			processFile();
			while (findrc == 0)
			{
				findrc = _findnext(findhandle, &findit);
				if (findrc != 0)
					break;
				fileCount++;
				strcpy_s(filename, 256, findit.name);
				processFile();
			}
			findrc = _findclose(findhandle);
		}
		else
			printf("No files found matching \"%s\"\n", expression);
		// } OLD VERSION USING SEPERATE CASE FOR NON-WILDCARD FILE SEARCH PATHS
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
		printf("\n================================================================================================\n");
		printf("Searching file %s...\n", filename);
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
		printf("\n================================================================================================\n");
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