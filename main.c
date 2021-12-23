#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
struct strType
{
	char* fieldName;
	char* strVal;
	unsigned int len;
}typedef s_strType;

s_strType* mk_strType()
{
	s_strType* _p;

	_p = (s_strType*)malloc(sizeof(s_strType));
	memset(_p, 0, sizeof(s_strType));
	return _p;
}

void set_strInStrType_uMA(s_strType* dst_p, char* src_str)
{
	unsigned int debug = 0;
	char* _str;
	unsigned int _strLen;

	_strLen = strlen(src_str);

	_str = (char*)malloc(sizeof(char)*(_strLen+1));
	memcpy(_str, src_str, sizeof(char)*(_strLen));
	*(_str+_strLen) = NULL;

	dst_p->strVal = _str;
	dst_p->len = _strLen;
}

s_strType* mk_strTypeCpVal(char* fieldName, char* str)
{
	s_strType* _p;
	unsigned int _len;

	_p = (s_strType*)malloc(sizeof(s_strType));
	memset(_p, 0, sizeof(s_strType));

	// set fieldName
	_p->fieldName = fieldName;

	// str copy and 
	_len = strlen(str);
	_p->strVal = (char*)malloc(sizeof(char)*(_len+1));
	*(_p->strVal+(_len+1)) = NULL;	// set to null @ end
	memcpy(_p->strVal, str, sizeof(char) * (_len));
	// set length
	_p->len = _len;

	printf("[s_strType] fieldName: \"%s\", strVal: \"%s\", len: %d\r\n", _p->fieldName, _p->strVal, _p->len);

	return _p;
}

void rm_strTypeCpVal(s_strType** p)
{
	free((*p)->strVal);
	free(*p);
	*p = NULL;
}

void print_strType(s_strType* p)
{
	printf("fieldName: \"%s\"", (p->fieldName!=NULL)?p->fieldName:"");

	printf(", strVal: \"%s\"", (p->strVal!=NULL)?p->strVal:"");

	printf(", p->len= %d\r\n", p->len);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

struct dataSet
{
	s_strType*** data;
	unsigned int row;
	unsigned int col;
}typedef s_dataSet;

s_dataSet* mk_dataSet(unsigned int row, unsigned int col)
{
	s_dataSet* _p = (s_dataSet*)malloc(sizeof(s_dataSet));
	_p->data = (s_strType***)malloc(sizeof(s_strType**)*row);
	for(s_strType*** _pRow = _p->data; _pRow < (_p->data+row); _pRow++)
	{
		(*_pRow) = (s_strType**)malloc(sizeof(s_strType*)*col);
		for(s_strType** _pCol = (*_pRow); _pCol < (*_pRow + col); _pCol++)
		{
			(*_pCol) = mk_strType();
		}
	}

	_p->row = row;
	_p->col = col;
	return _p;
}

void rm_dataSet(s_dataSet** p)
{
	return;
}

void print_dataSet(s_dataSet* p)
{
	printf("[print_dataSet] p->row: %d, p->col: %d\r\n", p->row, p->col);
	for(unsigned int _row = 0; _row < p->row; _row++)
	{
		for(unsigned int _col = 0; _col < p->col; _col++)
		{
			printf("%s,", (p->data[_row][_col])->strVal);
		}
		printf("\r\n");
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

s_dataSet* loadFile(char* path)
{
	char bf[1024];
	char* _bf;
	FILE* fp = NULL;

	unsigned int _row = 0;
	unsigned int _col = 0;

	unsigned int row = 0;
	unsigned int col = 0;

	s_dataSet* _dataSet = NULL;

	char parse_strPointer[1024];
	char *parse_preStrIdx;
	char *parse_postStrIdx;
	unsigned int parse_strLen;

	memset(bf, 0, sizeof(char)*1024);
	fp = fopen((const char*)path, "r");

	_bf = fgets(bf, 1024, fp);	// include CR, LF
	printf("Read Line : %s", bf);
	_col = 0;
	for(unsigned int i=0; bf[i]!='\n'; i++)
	{
		if		(bf[i]==',')	_col++;
		else if	(bf[i]=='\r')	_col++;
	}
	printf("_col lenght is '%d'\r\n", _col);
	//set col
	col = _col;
	fseek(fp, 0, SEEK_SET);

	// Make data structure
	_dataSet = mk_dataSet(501, col);

	// Parsing Data, first row is fieldName
	_row = 0;
	_col = 0;
	do
	{
		_bf = fgets(bf, 1024, fp);	// include CR, LF
		if(bf == _bf)
		{
#if 0
			printf("Read Line : %s", bf);
#endif

			// Init
			parse_preStrIdx = bf;
			parse_postStrIdx = bf;

			// Set
			do
			{
				// find word
				parse_preStrIdx = parse_postStrIdx;

				if(*(parse_preStrIdx) == '"')	// not string type
				{
					parse_preStrIdx++;	// Move right @ '"'
					parse_postStrIdx = strchr(parse_preStrIdx, '"');	// parse_postStrIdx indicates loc of '"'
				}
				else
				{
					parse_postStrIdx = strchr(parse_preStrIdx, ',');	// parse_postStrIdx indicates loc of ','
				}

				if(parse_postStrIdx == NULL)	// not found loc ','
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
				// move if detect '"'
				if(*parse_postStrIdx == '"')	parse_postStrIdx++;
				else;

				// save word
#if 0
				printf("\"%s\", _row= %d, _col=%d\r\n", parse_strPointer, _row, _col);	// _row, _col are indexa
#endif
				set_strInStrType_uMA(_dataSet->data[_row][_col], parse_strPointer);
#if 0
				print_strType(_dataSet->data[_row][_col]);
#endif

				// increse _col
				_col++;

				// detect end condition, \r\n or NULL
				if(*parse_postStrIdx != NULL)
				{
					parse_postStrIdx+=1; // Move right to void ',' and '\r'

					if(*parse_postStrIdx == '\n')
					{
						parse_postStrIdx++;	// Move to NULL
						parse_preStrIdx = parse_postStrIdx;
						if(_col != col)	printf("[FILE PARSING ERROR]!!!\r\n");
						else;
						_col = 0;
						_row++;
#if 0
						printf("detect line feed!\r\n");
#endif
						break;
					}
					else;
				}
				else
				{
					parse_preStrIdx = parse_postStrIdx;
					printf("detect file end!\r\n");
					_row++;
					break;
				}
			}
			while(parse_postStrIdx != NULL);
		}
		else;
	}
	while(_bf != NULL);
	printf("_row= %d, _col=%d\r\n", _row, _col);	// _row, _col are length
	return _dataSet;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{
	s_dataSet* dSet_au500 = NULL;
	dSet_au500 = loadFile("au-500.csv");
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
	print_dataSet(dSet_au500);
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
	printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");

	s_strType* _tmp;
	_tmp = mk_strTypeCpVal("testField", "testString");

	rm_strTypeCpVal(&_tmp);
	printf("[s_strType] delete test _tmp: 0x%x\r\n", _tmp);
	

	return 0;
}
