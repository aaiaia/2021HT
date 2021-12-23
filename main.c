#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
struct strType
{
	char* name;
	char* str;
	unsigned int len;
}typedef s_strType;

s_strType* mk_strTypeCpVal(char* fieldName, char* str)
{
	s_strType* _p;
	unsigned int _len;

	_p = (s_strType*)malloc(sizeof(s_strType));
	memset(_p, 0, sizeof(s_strType));

	// set fieldName
	_p->name = fieldName;

	// str copy and 
	_len = strlen(str);
	_p->str = (char*)malloc(sizeof(char)*(_len+1));
	*(_p->str+(_len+1)) = NULL;	// set to null @ end
	memcpy(_p->str, str, sizeof(char) * (_len));
	// set length
	_p->len = _len;

	printf("[s_strType] name: \"%s\", str: \"%s\", len: %d\r\n", _p->name, _p->str, _p->len);

	return _p;
}

void rm_strType(s_strType** p)
{
	free((*p)->str);
	free(*p);
	*p = NULL;
}

void loadFile(char* path)
{
	char bf[1024];
	char* _bf;
	FILE* fp = NULL;

	char parse_strPointer[1024];
	char *parse_preStrIdx;
	char *parse_postStrIdx;
	unsigned int parse_strLen;

	memset(bf, 0, sizeof(char)*1024);
	fp = fopen((const char*)path, "r");

	// Parsing Field name

	// Parsing Data
	do
	{
		_bf = fgets(bf, 1024, fp);	// include CR, LF
		if(bf == _bf)
		{
			printf("Read Line : %s", bf);

			// Init
			parse_preStrIdx = bf;
			parse_postStrIdx = bf;

			// Set
			do
			{
				// find word
				parse_preStrIdx = parse_postStrIdx;

				if(*(parse_preStrIdx) != '"')	// not string type
				{
					parse_postStrIdx = strchr(parse_preStrIdx ,',');	// parse_postStrIdx indicates loc of ','
				}
				else
				{
					parse_postStrIdx = strchr(++parse_preStrIdx ,'"');	// move non '"', parse_postStrIdx indicates loc of
				}

				if(parse_postStrIdx == NULL) 
				{
					parse_postStrIdx = strchr(parse_preStrIdx ,'\r');
					if(parse_postStrIdx == NULL) // Not include line feed, mean that last word
					{
						char *_cp;
						for(_cp = parse_preStrIdx; *_cp != NULL; _cp++);
						parse_postStrIdx = _cp;
					}
				}
				else;

				// cal length
				if(parse_postStrIdx > parse_preStrIdx)
				{
					parse_strLen = parse_postStrIdx - parse_preStrIdx;
				}
				else
				{
					parse_strLen = parse_preStrIdx - parse_postStrIdx ;
				}

				// copy word
				memcpy(parse_strPointer, parse_preStrIdx, sizeof(char) * (parse_strLen));
				*(parse_strPointer + parse_strLen) = NULL;	// insert NULL

				// save word
				printf("___\"%s\"\r\n", parse_strPointer);

				// detect end condition, \r\n or NULL
				if(*parse_postStrIdx != NULL)
				{
					parse_postStrIdx+=1; // Move right to void ',' and '\r'

					if(*parse_postStrIdx == '\n')
					{
						parse_postStrIdx++;	// Move to NULL
						parse_preStrIdx = parse_postStrIdx;
						printf("detect line feed!\r\n");
						break;
					}
					else;
				}
				else
				{
					parse_preStrIdx = parse_postStrIdx;
					printf("detect file end!\r\n");
					break;
				}
			}
			while(parse_postStrIdx != NULL);
		}
		else;
	}
	while(_bf != NULL);
}

int main(int argc, char* argv[])
{
	loadFile("au-500.csv");

	s_strType* _tmp;
	_tmp = mk_strTypeCpVal("testField", "testString");

	rm_strType(&_tmp);
	printf("[s_strType] delete test _tmp: 0x%x\r\n", _tmp);
	

	return 0;
}
