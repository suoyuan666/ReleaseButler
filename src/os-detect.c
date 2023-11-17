#include "errmsg.h"
#include <stdio.h>
#include <string.h>

#define PACK_NAME_MAX 12
#define OS_NAME_MAX 12
#define OS_NOT_FOUND 255

typedef struct packinfo
{
    char pack_name[PACK_NAME_MAX];
    char pack_install_name[PACK_NAME_MAX];
    char os_kind[OS_NAME_MAX];
} packinfo;

static packinfo OS_package[] = {
    {"deb", "dpkg", "debian"},
    {"deb", "dpkg", "ubuntu"},
    {"rpm", "dnf", "fedora"},
};

int OS_detect(){
    int rs = OS_NOT_FOUND;
    char buf[256] = {0};
    char name[OS_NAME_MAX] = {0};

    FILE *fd = fopen("/etc/os-release", "r");
    if(!fd){
        fd = fopen("/usr/lib/os-release", "r");
        if(!fd){
            FILE_NOT_OPEN
        }
    }
    while (fgets(buf, sizeof(buf), fd))
    {
        if (buf[0] == 'I' && buf[1] == 'D')
        {
            int length = strlen(buf);
            for (int i = 3, j = 0; i < length - 1; i++, j++)//ex: ID=debian
                name[j] = buf[i];
        }
    }

    for(rs = 0; rs < sizeof(OS_package)/sizeof(packinfo); rs++){
        if(!strcmp(OS_package[rs].os_kind, name)){
            break;
        }
    }
    return rs;
}
