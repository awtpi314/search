#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <io.h>

#define FOUND 1
#define MISSING 0

#define MAX_SIZE 1024 * 2

#define CASE_INSENSITIVE 1
#define EXCLUDE 1

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
int caseInsensitive = 0;
int exclude = 0;

int findrc = 0;
struct _finddata_t findit;
long findhandle;

int fileCount = 0;

int findThisInThat(char *thisString, char *thatString);

void processFile();

int countThisInThat(char thisChar, char *thatString);

void printUsage();

void main(int argc, char *argv[])
{
	int errors = 0;

	if (argc < 3)
	{
		printUsage();
		exit(1);
	}
	else if (argc >= 3)
	{
		strcpy_s(searchString, MAX_SIZE, argv[2]);
		strcpy_s(expression, 256, argv[1]);
		for (int index = 3; index < argc; ++index)
		{
			if (!(memcmp(argv[index], "-i", 2)))
			{
				caseInsensitive = CASE_INSENSITIVE;
			}
			else if (!(memcmp(argv[index], "-x", 2)))
			{
				exclude = EXCLUDE;
			}
			else
			{
				printf("Error: Incorrect arguments.\n");
				printUsage();
				exit(1);
			}
		}
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
		perror("Error");
		exit(1);
	}
	else
	{
		printf("\n================================================================================================\n");
		printf("Searching file %s...\n", filename);
		int index = 1;
		int count = 0;
		int result;
		while (!feof(fptr))
		{
			fgets(stringBuffer, MAX_SIZE, fptr);
			result = findThisInThat(searchString, stringBuffer);
			if (result == FOUND)
			{
				count++;
				if (!exclude)
				{
					printf("%s\n", stringBuffer);
				}
			}
			else
			{
				if (exclude)
				{
					printf("%s\n", stringBuffer);
				}
			}
			index++;
		}
		if (count == 0)
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
				return FOUND;
			}
		}
	}
	return MISSING;
}

void printUsage()
{
	printf("\n");
	printf("Use: search <filename> <string> [-x] [-c]\n");
	printf("\n");
	printf("\tWhere -i is case insensitivity;\n");
	printf("\t      -x is all not containing <string>;\n");
	printf("\t      -c prints the number of occurences.\n");
}