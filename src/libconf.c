#include <stdio.h> 
#include <string.h>
#include <stdlib.h>
#include "errmsg.h"

#define NAME_LEN_MAX 64
#define VALUE_LEN_MAX 256

typedef struct confOJ
{
    struct confOJ *next;
    char name[NAME_LEN_MAX];
    char value[VALUE_LEN_MAX];
}confOJ;

#define confCreate() (confOJ*)calloc(1,sizeof(confOJ))


confOJ* conf_AddString(confOJ* conf, char* name, char* value){
    if(!conf){
        CONF_CANNOT_ADD
    }
    if(!(conf->name[0] == '\0' || conf->value[0] == '\0')){
        CONF_HAS_VALUE
    }
    if(strlen(name) > NAME_LEN_MAX || strlen(value) > VALUE_LEN_MAX){
        CONF_LEN_OVER
    }
    
    strcpy(conf->name, name);
    strcpy(conf->value, value);

    return conf;
}

confOJ* conf_ADDconf(confOJ* prev){
    confOJ* conf = confCreate();
    prev->next = conf;

    return conf;
}

void conf_Free(confOJ* conf){
    int deep = 0;
    confOJ* tar = conf->next;
    while (conf->next){
        deep++;
        conf = conf->next;
    }
    for(int i = 0; i < deep; i++){
        free(conf);
        conf = tar;
        tar = tar->next;
    }
}

void conf_Parse(confOJ* conf, char* buf){
    int i = 0, j =0, len = 0;
    char flag = 0;
    int start_len = 3;
    char tmp[VALUE_LEN_MAX] = {0};
    const int length = strlen(buf);

    confOJ* cf_tmp = NULL;
    confOJ* target = conf;

    for(i = 0; i < length; i++){
        if(buf[i] == '\'')   break;
    }
    i+=3;
    while(buf[i]){
        for(int x = 0; x < start_len; x++){
            for(j = 0; i < length; i++, j++){
                switch (buf[i]){
                    case ':':
                        if(!flag){
                            if(strlen(tmp) > NAME_LEN_MAX){
                                CONF_LEN_OVER
                            }
                            len = strlen(tmp);
                            for(int y = 0; y < len; y++)
                                target->name[y] = tmp[y];
                            for(int y = 0; y < len; y++)
                                tmp[y] = '\0';
                            j = -1;
                        }else{
                            tmp[j] = buf[i];
                        }
                        
                        break;
                    case '\n':
                        if(strlen(tmp) > VALUE_LEN_MAX){
                            CONF_LEN_OVER
                        }
                        len = strlen(tmp);
                        for(int y = 0; y < len; y++)
                            target->value[y] = tmp[y];
                        for(int y = 0; y < len; y++)
                            tmp[y] = '\0';
                        i++;
                        goto again;
                    case '\'':
                        i++;
                        goto end;
                        break;
                    case '"':
                        if(!flag)
                            j--;
                        flag = flag ? 0 : 1;
                        break;
                    default:
                        if(flag)
                            tmp[j] = buf[i];
                        break;
                }
            }
            again:
                cf_tmp = confCreate();
                target->next = cf_tmp;
                target = cf_tmp;
        }
        end:
            i += 6;
            start_len += 3;
    }
}

char* conf2str(confOJ* conf){
    confOJ *target = conf;
    int buf_len_max = 512;
    char *buf = calloc(buf_len_max,  sizeof(char));

    buf[0] = '\''; buf[1] = '\''; buf[2] = '\n';
    int i = 3; int x = 0, y = 0;

    for(; target; target = target->next){
        int nameLen = strlen(target->name);
        int vallen = strlen(target->value);
        if(!nameLen || !vallen) break;
        int offset = nameLen + vallen + 9; //the 9 char is ", ", :, ", ", \n, ', ', \n
        if(i + offset > buf_len_max){
            if(!realloc(buf, buf_len_max + offset)){
                BUF_OVERFLOW
            }
            buf_len_max += offset;
        }
        buf[i++] = '"';
        for(x = 0; x < nameLen; x++, i++){
            buf[i] = target->name[x];
        }
        buf[i++] = '"';
        buf[i++] = ':';
        buf[i++] = '"';
        for(x = 0; x < vallen; x++, i++){
            buf[i] = target->value[x];
        }
        buf[i++] = '"';
        buf[i++] = '\n';
    }
    buf[i++] = '\'';
    buf[i++] = '\'';
    buf[i] = '\n';

    return buf;
}