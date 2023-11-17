#define NAME_LEN_MAX 64
#define VALUE_LEN_MAX 256
#define confCreate() (confOJ*)calloc(1,sizeof(confOJ))

typedef struct confOJ
{
    struct confOJ *next;
    char name[NAME_LEN_MAX];
    char value[VALUE_LEN_MAX];
}confOJ;

confOJ* conf_AddString(confOJ* conf, char* name, char* value);
confOJ* conf_ADDconf(confOJ* prev);
void conf_Free(confOJ* conf);
void conf_Parse(confOJ* conf, char* buf);
char* conf2str(confOJ* conf);

