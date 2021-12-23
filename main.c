#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <time.h>

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* llist.h */
enum linked_list_direction
{
	LLIST_DIR_DEFAULT,
	LLIST_DIR_PREV,
	LLIST_DIR_NEXT
};
typedef enum linked_list_direction e_llist_dir;
#define LLIST_DIR_LEFT	LLIST_DIR_PREV
#define LLIST_DIR_RIGHT	LLIST_DIR_NEXT

enum linked_list_type
{
	LLIST_TYPE_TOGETHER,
	LLIST_TYPE_SEPERATE
};
typedef enum linked_list_type e_llist_type;

struct linked_list
{
	struct linked_list*		prev;
	struct linked_list*		next;
	enum linked_list_type	type;
	void*					object;
};
typedef struct linked_list s_llist;


s_llist* open_llist(void);
s_llist* close_llist(s_llist* p_llist);
s_llist* mk_llist(s_llist* p, e_llist_dir dir);
s_llist* rm_llist(s_llist* p);
s_llist* find_llist_end(s_llist* p, e_llist_dir dir);
void* get_llist_object(s_llist* p);
int set_llist_object(s_llist* p, void* object, e_llist_type type);
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
/* llist.c */
s_llist* open_llist(void)
{
	s_llist* p = (s_llist*)calloc(1, sizeof(s_llist));
	memset(p, 0u, sizeof(s_llist));
	p->prev = NULL;
	p->next = NULL;
	p->object = NULL;
	return p;
}

s_llist* close_llist(s_llist* p)
{
	s_llist* _p_tg;
	s_llist* _p_tmp;

	if(p != NULL)
	{
		/* Remove prev side llist */
		_p_tg = p->prev;
		while(_p_tg != NULL)
		{
			_p_tmp = _p_tg->prev;
			free(_p_tg);
			_p_tg = _p_tmp;
		}

		/* Remove next side llist */
		_p_tg = p->next;
		while(_p_tg != NULL)
		{
			_p_tmp = _p_tg->next;
			free(_p_tg);
			_p_tg = _p_tmp;
		}
	}
	else
	{
		/* Do nothing */
	}

	return NULL;
}

s_llist* mk_llist(s_llist* p, e_llist_dir dir)
{
	s_llist* _p_new;
	s_llist* _p_tmp;
	if(p == NULL)
	{
		return NULL;
	}
	else
	{
		switch(dir)
		{
			case LLIST_DIR_PREV:
			{
				_p_new = open_llist();

				_p_tmp = p->prev;

				//link new and cur
				p->prev = _p_new;
				_p_new->next = p;

				if(_p_tmp != NULL)	
				{
					_p_tmp->next=_p_new;
					_p_new->prev=_p_tmp;
				}
				return _p_new;
			}

			case LLIST_DIR_NEXT:
			{
				_p_new = open_llist();

				_p_tmp = p->next;

				//link new and cur
				p->next = _p_new;
				_p_new->prev = p;

				if(_p_tmp != NULL)	
				{
					_p_tmp->prev = _p_new;
					_p_new->next = _p_tmp;
				}
				return _p_new;
			}

			default:
			{
				return p;
			}
		}
	}
}

s_llist* rm_llist(s_llist* p)
{
	s_llist* _next;
	s_llist* _prev;
	
	if(p != NULL)
	{
		_next = p->next;
		_prev = p->prev;

		if	(_next != NULL)		_next->prev=_prev;
		if	(_prev != NULL)		_prev->next=_next;

		if	(p->object != NULL)
		{
			if(p->type == LLIST_TYPE_TOGETHER)	free(p->object);
		}

		free(p);
		if		(_next != NULL)	return _next;
		else if	(_prev != NULL)	return _prev;
		else					return NULL;
	}
	else
	{
		return NULL;
	}	

}

s_llist* find_llist_end(s_llist* p, e_llist_dir dir)
{
	if(p==NULL)
	{
		return NULL;
	}
	else
	{
		s_llist* _tmp;
		switch(dir)
		{
			case LLIST_DIR_PREV:
			{
				for(_tmp=p; _tmp->prev!=NULL; _tmp=_tmp->prev);
			}
			break;

			case LLIST_DIR_NEXT:
			{
				for(_tmp=p; _tmp->next!=NULL; _tmp=_tmp->next);
			}
			break;

			default:
			{
				_tmp = p;
			}
			break;
		}
		return _tmp;
	}
}

void* get_llist_object(s_llist* p)
{
    if(p==NULL)
    {
        return NULL;
    }
    return p->object;
}

int set_llist_object(s_llist* p, void* object, e_llist_type type)
{
    if(p==NULL)
    {
        return -1;
    }
    p->object = object;
	p->type = type;
    return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
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

void print_dataSetCol(s_strType** p, unsigned int colLen)
{
	unsigned int _col;
	for(_col = 0; _col < colLen-1; _col++)
	{
		printf("%s,",p[_col]->strVal);
	}
	printf("%s\r\n",p[_col++]->strVal);
}

void print_dataSet(s_dataSet* p)
{
	printf("[print_dataSet] p->row: %d, p->col: %d\r\n", p->row, p->col);
	for(unsigned int _row = 0; _row < p->row; _row++)
	{
		printf("Addr: 0x%x, ", p->data[_row]);
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

unsigned int find_linerAlgo_keyWord(s_llist* pList, s_dataSet* pDat, char* keyWord)
{
	s_llist* _newList = pList;
	unsigned int _hitCnt = 0;
	for(unsigned int _row = 0; _row < pDat->row; _row++)
	{
		for(unsigned int _col = 0; _col < pDat->col; _col++)
		{
			char* _loc;
#if 0
			printf("ri: %d, ci: %d, str: \"%s\"", _row, _col, (pDat->data[_row][_col])->strVal);
#endif
			_loc = strstr((pDat->data[_row][_col])->strVal, keyWord);
#if 0
			printf(" ok, _loc: \"0x%x\"\r\n", _loc);
#endif
			if(_loc != NULL)
			{
				if(_newList->object != NULL)	_newList = mk_llist(_newList, LLIST_DIR_NEXT);
				else;

				if(set_llist_object(_newList , (void*)pDat->data[_row], LLIST_TYPE_SEPERATE))	printf("[find_linerAlgo_keyWord] error, set_llist_object()\r\n");
				else;
#if 0
				printf("p: 0x%x, _: 0x%x, n: 0x%x, obj: 0x%x, pDat->data[%d] = 0x%x, (void*) = 0x%x\r\n", \
						_newList->prev, _newList, _newList->next, _newList->object, _row, pDat->data[_row], pDat->data[_row]);
#endif
#if 0
				printf("Found List-> ");
				print_dataSetCol(pDat->data[_row], pDat->col);
#endif
				
				_hitCnt++;
				break;
			}
			else;
		}
	}
#if 0
	printf("p: 0x%x, _: 0x%x, n: 0x%x, obj: 0x%x\r\n", \
			pList->prev, pList, pList->next, pList->object);
#endif
	return _hitCnt;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////
#define timeMes printf("[TIME] ) ")
void printExcutingTime(struct timeval *start, struct timeval *end)
{
	struct timeval diff;
	gettimeofday( end, NULL );

	diff.tv_sec = end->tv_sec - start->tv_sec;
	diff.tv_usec = end->tv_usec - start->tv_usec;
	if(diff.tv_usec<0)
	{
		diff.tv_usec+=1000000;
		diff.tv_sec-=1;
		timeMes;	printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);
	}
	else
	{
		timeMes;	printf("%ld [sec], %ld [usec]\n", diff.tv_sec, diff.tv_usec);
	}
}

int main(int argc, char* argv[])
{
	// to measuring time
	struct timeval time_s;
	struct timeval time_e;

	// to keyIn
	char _keyInWord[1024] = {0};

	unsigned int findHitCnt;
	s_llist* findHitList;

	s_dataSet* dSet_au500 = NULL;
	s_dataSet* dSet_ca500 = NULL;
	s_dataSet* dSet_uk500 = NULL;
	s_dataSet* dSet_us500 = NULL;
	s_llist* dSetList;
	s_llist* _addNew_dSetList;

	unsigned int printCnt;

	/* init llist */
	findHitList = open_llist();

	/* init dataset llist */
	dSetList = open_llist();
	_addNew_dSetList = dSetList;

	while(1)
	{
		printf("Insert Keyword: ");
		scanf("%1023s", _keyInWord);
		switch(_keyInWord[0])
		{
			case '/':
			{
				printf("Insert fineName: ");
				scanf("%1023s", _keyInWord);
				
				void* _tmpAddrChk;
				if(_addNew_dSetList->object != NULL)	_addNew_dSetList = mk_llist(_addNew_dSetList, LLIST_DIR_NEXT);
				else;

				if(set_llist_object(_addNew_dSetList , _tmpAddrChk = (void*)loadFile(_keyInWord), LLIST_TYPE_TOGETHER))	printf("[find_linerAlgo_keyWord] error, set_llist_object()\r\n");
				else;

				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
				print_dataSet((s_dataSet*)get_llist_object(_addNew_dSetList));
				printf("add data set addr: 0x%08x\r\n", _tmpAddrChk);
				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");
				printf("////////////////////////////////////////////////////////////////////////////////////////////////////\r\n");

			}
			break;

			default:
			{
				findHitCnt = 0;
				printCnt = 0;
				gettimeofday(&time_s, NULL);
				for(s_llist* _search_dSetList = dSetList; (_search_dSetList != NULL) && (_search_dSetList->object != NULL); _search_dSetList = _search_dSetList->next)
				{
					printf("_search_dSetList addr: 0x%08x\r\n", _search_dSetList);
					findHitCnt += find_linerAlgo_keyWord(findHitList , (s_dataSet*)get_llist_object(_search_dSetList), _keyInWord);
				}
				gettimeofday(&time_e, NULL);
				printf("Hit count: %d, print cnt: %d\r\n", findHitCnt, printCnt);
				printExcutingTime(&time_s, &time_e);
//				{
//						for(s_llist* _hitList = findHitList; (_hitList != NULL) && (_hitList->object != NULL); _hitList = _hitList->next)
//						{
//							printf("p: 0x%08x,_: 0x%08x,n: 0x%08x,", _hitList->prev, _hitList, _hitList->next);
//							print_dataSetCol((s_strType**)get_llist_object(_hitList), dSet_au500->col);
//							printCnt++;
//						}
//					printf("Hit count: %d, print cnt: %d\r\n", findHitCnt, printCnt);
//					printExcutingTime(&time_s, &time_e);
//				}
				// clear llist
				findHitList = close_llist(findHitList );
				findHitList = open_llist();
			}
		}

	}

	s_strType* _tmp;
	_tmp = mk_strTypeCpVal("testField", "testString");

	rm_strTypeCpVal(&_tmp);
	printf("[s_strType] delete test _tmp: 0x%x\r\n", _tmp);

	return 0;
}
