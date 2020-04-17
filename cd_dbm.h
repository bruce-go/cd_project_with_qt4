#ifndef CD_DBM_H
#define CD_DBM_H

#ifdef __cplusplus
extern "C"{
#endif
/*catalog_entry table*/
#define CAT_CAT_LEN      30
#define CAT_TITLE_LEN    70
#define CAT_TYPE_LEN     30
#define CAT_ARTIST_LEN   70

typedef struct 
{
	char catalog_entry[CAT_CAT_LEN + 1];
	char title[CAT_TITLE_LEN + 1];
	char type[CAT_TYPE_LEN + 1];
	char artist[CAT_ARTIST_LEN + 1];
} cd_catalog_entry;

/*The track table, one entry per track*/
#define TRACK_CAT_LEN    CAT_CAT_LEN
#define TRACK_TTEXT_LEN  70

typedef struct 
{
	char catalog_entry[TRACK_CAT_LEN + 1];
	int  track_no;
	char track_txt[TRACK_TTEXT_LEN + 1];
} cd_track_entry;

/*initialization and termination functions*/
int database_initialize(const int new_databse);
void database_close(void);

/*two for go through the whole cd_catalog database*/
cd_catalog_entry cd_catalog_DBM_FirstItem();
cd_catalog_entry cd_catalog_DBM_NextItem();

/*two for simple data retrieval*/
cd_catalog_entry get_cd_catalog_entry(const char *cd_catalog_ptr);
cd_track_entry get_cd_track_entry(const char *cd_catalog_ptr, const int track_no);

/*two for data addition*/
int add_cd_catalog_entry(const cd_catalog_entry entry_to_add);
int add_cd_track_entry(const cd_track_entry entry_to_add);

/*two for data deletion*/
int del_cd_catalog_entry(const char *cd_catalog_ptr);
int del_cd_track_entry(const char *cd_catalog_ptr, const int track_no);

/*one search function*/
cd_catalog_entry search_cd_catalog_entry(const char *cd_catalog_ptr, int *first_call_ptr);


#ifdef __cplusplus
}
#endif


#endif