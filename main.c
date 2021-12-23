#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void loadFile(char* path)
{
	char* bf[1024];
	FILE* fp = NULL;

	memset(bf, 0, sizeof(char)*1024);
	fp = fopen((const char*)path, "r");

	while(fp!=NULL)
	{
		fscanf(fp, "%s\r\n", bf);
		printf("%s", bf);
	}
}

int main(int argc, char* argv[])
{
	loadFile("au-500.csv");

	return 0;
}
