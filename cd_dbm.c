#define _XOPEN_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <gdbm-ndbm.h>

#include "cd_dbm.h"

#define CD_CATALOG_FILE_BASE "cd_catalog_data"
#define CD_TRACK_FILE_BASE "cd_track_data"
#define CD_CATALOG_FILE_DIR  "cd_catalog_data.dir"
#define CD_CATALOG_FILE_PAG  "cd_catalog_data.pag"
#define CD_TRACK_FILE_DIR  "cd_track_data.dir"
#define CD_TRACK_FILE_PAG  "cd_track_data.pag"
/*使用下面2个文件范围变量追踪当前的数据库*/
static DBM *cd_catalog_dbm_ptr = NULL;
static DBM *cd_track_dbm_ptr = NULL;

/*用于初始化数据库*/
int database_initialize(const int new_database)
{
	int open_mode = O_CREAT | O_RDWR;

	/*if any existing database is open ,then close it*/
	if(cd_catalog_dbm_ptr)
	{
		dbm_close(cd_catalog_dbm_ptr);
	}

	if(cd_track_dbm_ptr)
	{
		dbm_close(cd_track_dbm_ptr);
	}

	if(new_database)
	{
		/*delete the old files*/
		(void)unlink(CD_CATALOG_FILE_PAG);
		(void)unlink(CD_CATALOG_FILE_DIR);
		(void)unlink(CD_TRACK_FILE_PAG);
		(void)unlink(CD_TRACK_FILE_DIR);
	}

	/*open some new files ,creating them if required*/
	cd_catalog_dbm_ptr = dbm_open(CD_CATALOG_FILE_BASE, open_mode, 0666);//0644
	cd_track_dbm_ptr = dbm_open(CD_TRACK_FILE_BASE, open_mode, 0666);//0644

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		fprintf(stderr, "Unable to creat database\n");
		cd_catalog_dbm_ptr = cd_track_dbm_ptr = NULL;
		return (0);
	}

	return (1);
}

/*database_close函数用于关闭已打开的数据库，并将2个数据库指针设为NULL，以此表明当前没有打开的数据库*/
void database_close(void)
{
	if(cd_catalog_dbm_ptr)
	{
		dbm_close(cd_catalog_dbm_ptr);
	}

	if(cd_track_dbm_ptr)
	{
		dbm_close(cd_track_dbm_ptr);
	}

	cd_catalog_dbm_ptr = cd_track_dbm_ptr = NULL;
}

cd_catalog_entry cd_catalog_DBM_FirstItem()
{
	// fprintf(stderr, "get first key point 0\n");
	datum key_datum;
	datum data_datum;
	cd_catalog_entry entry_to_return;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	key_datum = dbm_firstkey(cd_catalog_dbm_ptr);
	// fprintf(stderr, "get first key point 1\n");
	if(key_datum.dptr != NULL)
	{
		// fprintf(stderr, "get first key point 2\n");
		data_datum = dbm_fetch(cd_catalog_dbm_ptr, key_datum);
	    if(data_datum.dptr)
	    {
	        // fprintf(stderr, "get first key point 3\n");
	        memcpy(&entry_to_return, (char *)data_datum.dptr, data_datum.dsize);
	        
	    }
	}
	// fprintf(stderr, "get first key point 4\n");
	return entry_to_return;
}

cd_catalog_entry cd_catalog_DBM_NextItem()
{
	datum key_datum;
	datum data_datum;
	cd_catalog_entry entry_to_return;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	key_datum = dbm_nextkey(cd_catalog_dbm_ptr);

	if(key_datum.dptr != NULL)
	{
		data_datum = dbm_fetch(cd_catalog_dbm_ptr, key_datum);
	    if(data_datum.dptr)
	    {
	        memcpy(&entry_to_return, (char *)data_datum.dptr, data_datum.dsize);
	        
	    }
	}
		
	return entry_to_return;
}


cd_catalog_entry get_cd_catalog_entry(const char *cd_catalog_ptr)
{
	cd_catalog_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 1];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (entry_to_return);
	}

	if(!cd_catalog_ptr)
	{
		return (entry_to_return);
	}

	if(strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	/*set up the search key, which is a composite key of catalog_entry entry and track number*/
	memset(entry_to_find, '\0', sizeof(entry_to_find));
	strcpy(entry_to_find, cd_catalog_ptr);

	local_key_datum.dptr = (void *)entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cd_catalog_dbm_ptr, local_key_datum);
	if(local_data_datum.dptr)
	{
		memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
	}

	return (entry_to_return);
}


cd_track_entry get_cd_track_entry(const char *cd_catalog_ptr, const int track_no)
{
	cd_track_entry entry_to_return;
	char entry_to_find[CAT_CAT_LEN + 10];
	datum local_data_datum;
	datum local_key_datum;

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (entry_to_return);
	}

	if(!cd_catalog_ptr)
	{
		return (entry_to_return);
	}

	if(strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	/*set up the search key, which is a composite key of catalog_entry entry and track number*/
	memset(entry_to_find, '\0', sizeof(entry_to_find));
	sprintf(entry_to_find, "%s %d", cd_catalog_ptr, track_no);
	// printf("get current track's key is : %s \n", entry_to_find);//调测使用

	local_key_datum.dptr = (void *)entry_to_find;
	local_key_datum.dsize = sizeof(entry_to_find);//保存时使用的键的大小和查询时使用的键的大小要完全相等，重要！！！

	memset(&local_data_datum, '\0', sizeof(local_data_datum));
	local_data_datum = dbm_fetch(cd_track_dbm_ptr, local_key_datum);
	// printf("current data pointer from key %s is: %lx \n", entry_to_find,(unsigned long)(local_data_datum.dptr));
	if(local_data_datum.dptr)
	{
		// fprintf(stderr, "get track succeeds\n");
		memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
		return (entry_to_return);
	}
	// fprintf(stderr, "get track failed\n");
	return (entry_to_return);
}

/*add_cdc_entry的作用是在数据库中增加一个新的目录项记录*/
int add_cd_catalog_entry(const cd_catalog_entry entry_to_add)
{
	char key_to_add[CAT_CAT_LEN + 1];
	datum local_data_datum;
	datum local_key_datum;
	int result;

	/*check database initialized and parameters valid*/
	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (0);
	}
	//这个判断条件不可能发生，因为entry_to_add.catalog最大为CAT_CAT_LEN-1
	if(strlen(entry_to_add.catalog_entry) >= CAT_CAT_LEN)
	{
		return (0);
	}

	/*ensure the search key contains only the valid string and nulls*/
	memset(&key_to_add, '\0', sizeof(key_to_add));
	strcpy(key_to_add, entry_to_add.catalog_entry);

	local_key_datum.dptr = (void *)key_to_add;
	local_key_datum.dsize = sizeof(key_to_add);

	local_data_datum.dptr = (void *)&entry_to_add;
	local_data_datum.dsize = sizeof(entry_to_add);

	result = dbm_store(cd_catalog_dbm_ptr, local_key_datum, local_data_datum, DBM_REPLACE);

	/*dbm_store() uses 0 for success*/
	if(result == 0)
	{
		return (1);
	}

	return (0);
}

int add_cd_track_entry(const cd_track_entry entry_to_add)
{
	char key_to_add[CAT_CAT_LEN + 10];
	datum local_key_datum;
	datum local_data_datum;
	int result;

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (0);
	}

	if(strlen(entry_to_add.catalog_entry) >= CAT_CAT_LEN)
	{
		return (0);
	}

	memset(&key_to_add, '\0', sizeof(key_to_add));
	sprintf(key_to_add, "%s %d", entry_to_add.catalog_entry, entry_to_add.track_no);
	// fprintf(stdout, "dbm add track's key is %s\n", key_to_add);

	local_key_datum.dptr = (void *)key_to_add;
	local_key_datum.dsize = sizeof(key_to_add);
	
	local_data_datum.dptr = (void *) &entry_to_add;
	local_data_datum.dsize = sizeof(entry_to_add);

	result = dbm_store(cd_track_dbm_ptr, local_key_datum, local_data_datum, DBM_REPLACE);

	/*dbm_store() uses 0 for success and -ve numbers for errors*/
	if(result == 0)
	{
		// fprintf(stdout, "dbm add track succeeds\n");
		return (1);
	}
	
	return (0);
}

int del_cd_catalog_entry(const char *cd_catalog_ptr)
{
	char key_to_del[CAT_CAT_LEN + 1];
	datum local_key_datum;
	int result;
	
	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (0);
	}
	
	if(!cd_catalog_ptr)
	{
		return (0);
	}
	
	if(strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (0);
	}
	
	/*set up the search key, which is a composite key of catalog_entry entry and track number*/
	memset(key_to_del, '\0', sizeof(key_to_del));
	strcpy(key_to_del, cd_catalog_ptr);
	
	local_key_datum.dptr = (void *)key_to_del;
	local_key_datum.dsize = sizeof(key_to_del);
	
	// memset(&local_data_datum, '\0', sizeof(local_data_datum));
	result = dbm_delete(cd_catalog_dbm_ptr, local_key_datum);
	if(result == 0)
	{
		return (1);
	}
	
	return (0);
}

int del_cd_track_entry(const char *cd_catalog_ptr, const int track_no)
{
	char key_to_del[CAT_CAT_LEN + 10];
	datum local_key_datum;
	int result;

	// memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (0);
	}

	if(!cd_catalog_ptr)
	{
		return (0);
	}

	if(strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (0);
	}

	/*set up the search key, which is a composite key of catalog_entry entry and track number*/
	memset(key_to_del, '\0', sizeof(key_to_del));
	sprintf(key_to_del, "%s %d", cd_catalog_ptr, track_no);

	local_key_datum.dptr = (void *)key_to_del;
	local_key_datum.dsize = sizeof(key_to_del);

	//dbm_delete() uses 0 for success
	result = dbm_delete(cd_track_dbm_ptr, local_key_datum);
	if(result == 0)
	{
		return (1);
	}

	return (0);
}
int searched_null_key_num = 0;
cd_catalog_entry search_cd_catalog_entry(const char *cd_catalog_ptr, int *first_call_ptr)
{
	static int local_first_call = 1;
	cd_catalog_entry entry_to_return;
	datum local_data_datum;
	static datum local_key_datum; /*notice this must be static*/

	memset(&entry_to_return, '\0', sizeof(entry_to_return));

	if(!cd_catalog_dbm_ptr || !cd_track_dbm_ptr)
	{
		return (entry_to_return);
	}

	if(!cd_catalog_ptr || !first_call_ptr)
	{
		return (entry_to_return);
	}

	if(strlen(cd_catalog_ptr) >= CAT_CAT_LEN)
	{
		return (entry_to_return);
	}

	/*Protect against never passing *first_call_ptr TRUE*/
	if(local_first_call)
	{
		local_first_call = 0;
		*first_call_ptr = 1;
	}

	if(*first_call_ptr) //第一次搜索走这个分支
	{
		*first_call_ptr = 0;
		local_key_datum = dbm_firstkey(cd_catalog_dbm_ptr);
	}
	else //第二次搜索走这个分支
	{
		local_key_datum = dbm_nextkey(cd_catalog_dbm_ptr);
	}

	do
	{
		if(local_key_datum.dptr != NULL)
		{
			/*an entry was found*/
			local_data_datum = dbm_fetch(cd_catalog_dbm_ptr, local_key_datum);
			if(local_data_datum.dptr)
			{
				memcpy(&entry_to_return, (char *)local_data_datum.dptr, local_data_datum.dsize);
				if(!(strstr(entry_to_return.catalog_entry, cd_catalog_ptr) || 
					 strstr(entry_to_return.title, cd_catalog_ptr) || 
					 strstr(entry_to_return.artist, cd_catalog_ptr) ||
					 strstr(entry_to_return.type, cd_catalog_ptr) ))//此处可以另写一个函数将传进来的字符串与每一个标题依次对比，并返回有一部分字符串完全匹配的节点
				{
					memset(&entry_to_return, '\0', sizeof(entry_to_return));
					local_key_datum = dbm_nextkey(cd_catalog_dbm_ptr);
				}
			}
		}
		/*else
		{
			printf("key is null %d time\n", ++searched_null_key_num);
		}*/
	}while(local_key_datum.dptr && local_data_datum.dptr && (entry_to_return.catalog_entry[0]=='\0'));

	return (entry_to_return);
}