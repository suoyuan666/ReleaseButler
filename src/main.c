#include "errmsg.h"
#include "os-detect.h"
#include "libconf.h"

#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <curl/curl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#define PACK_NAME_MAX 12
#define URL_LEN_MAX 128

#define CHECK_CURL_RES      \
    if(res != CURLE_OK){    \
        CONNECT_FAILED;     \
    }

struct WebSource {
  char *memory;
  size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);
int install(char *url, confOJ* cj_config, char* version);
void splitString(char *dst, char* str, char key, int length);
void ReverseString(char *src);

int main(int argc, char* argv[]){

    int this_option_optind = optind ? optind : 1;

    confOJ *conf = confCreate();

    char *json_str = NULL;
    int option_index = 0, c = 0;
    struct stat st = {0};
    char path[64] = {0};
    long fsize;
    char *buffer = NULL;
    char url[URL_LEN_MAX] = {0};
    static struct option long_options[] = {
        {"install", required_argument, 0,  'i' },
        {"from",    required_argument, 0,  'f' },
        {"update",  no_argument,       0,  'u' },
        {"help",    no_argument,       0,  'h' },
        {"version", no_argument,       0,  'v' },
        {0,         0,                 0,   0  }
    };

    char *home = getenv("HOME"); //get enviroment variable $HOME to edit config file.
    if(!home){
        ENV_NOT_READ
    }
    strcpy(path, home);
    strcat(path, "/.config/releasebuter/");
    if (stat(path, &st) == -1)    mkdir(path, 0700);
    strcat(path, "config.conf");
    FILE *fd_config = fopen(path, "a+");
    if (!fd_config)
    {
        FILE_NOT_OPEN
    }


    fseek(fd_config, 0L, SEEK_END);
    fsize = ftell(fd_config);
    if(fsize){
        fseek(fd_config, 0L, SEEK_SET); 
        buffer = malloc(fsize);
        fread(buffer, 1, fsize, fd_config);
        conf_Parse(conf, buffer);
    }else{
        conf_ADDconf(conf);
        conf_ADDconf(conf->next);
    }

    while (1)
    {
        c = getopt_long(argc, argv, "i:f:u::h::v::", long_options, &option_index);
        if (c == -1)
            break;
        switch (c) {
            case 'u':
                if(!fsize){
                    FILE_NOT_READ
                }
                
                for(confOJ* tmp=conf; tmp->next; tmp=tmp->next->next->next){
                    install(tmp->next->value, tmp, tmp->next->next->value);
                }
                freopen(path, "w", fd_config);
                json_str = conf2str(conf);
                fputs(json_str, fd_config);
                break;
            case 'i':
                if(fsize){
                    for(confOJ* tmp = conf;tmp->next;tmp=tmp->next->next->next){
                        if(!strcmp(tmp->value, optarg)){
                            PAK_HAS_INS
                        }
                    }
                }
                fprintf(stderr, "install package: %s", optarg);                
                conf_AddString(conf, "package", optarg);
                break;
            case 'f':
                fprintf(stderr, " from: %s", optarg);
                printf("\n");
                conf_AddString(conf->next, "url", optarg);
                install(optarg, conf, 0);
                json_str = conf2str(conf);
                fputs(json_str, fd_config);
                conf_Free(conf);
                break;
            case 'h':
                puts("ReleaseButler: version 0.1 \n");
                puts("Usage: releasebuter install <package> from <url>\t");
                puts("       releasebuter update\n");
                puts("ReleaseButler can fetch package from Internet such as GitHub and if it is a mainstream personal desktop system, it can be installed automatically.\n");
                puts("Option:\t");
                puts("-i, --install:  install software\t");
                puts("-u, --update:   update all installed software\t");
                puts("-h, --help:     print help information");
                puts("-v, --version:  print version");
                puts("\ninstall Option:\t");
                puts("\t-f, --from: download from specified URL");
                break;
            case 'v':
                puts("ReleaseButler: version 0.1");
                break;
        }
    }
    if(buffer)  free(buffer);
    fclose(fd_config);
    return 0;
}

int install(char *url, confOJ* conf, char* cur_version){
    char filename[64] = {0};
    char url_ass[URL_LEN_MAX] = {0};
    char url_download[URL_LEN_MAX] = {0};
    char url_location[URL_LEN_MAX] = {0};
    char version[16] = {0};
    char *location;
    CURLcode res;
    long response_code;
    int pid;
    int os_check;
    int i, j;
    struct WebSource webrs;
    webrs.memory = malloc(1);
    webrs.size = 0;

    printf("install begin, url: %s\n", url);

    if(url[strlen(url)-1] != '/')
        url[strlen(url)] = '/';
    strcpy(url_ass, url);
    strcpy(url_download, url);
    strcat(url, "releases/latest");
    strcat(url_ass, "releases/expanded_assets/");
    strcat(url_download, "releases/download/");

    CURL *curl = curl_easy_init();
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        res = curl_easy_perform(curl);
    }else{
        CURL_INIT_FAILED
    }
    CHECK_CURL_RES

    res = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &response_code);
    CHECK_CURL_RES
    res = curl_easy_getinfo(curl, CURLINFO_REDIRECT_URL, &location);
    CHECK_CURL_RES

    strcpy(url_location, location);
    splitString(version, url_location, '/', strlen(location));
    printf("version: %s\n", version);


    strcat(url_ass, version);
    strcat(url_download, version);
    url_download[strlen(url_download)] = '/';

    curl_easy_reset(curl);
    if(curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url_ass);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&webrs);
        res = curl_easy_perform(curl);
    }else{
        CURL_INIT_FAILED
    }

    CHECK_CURL_RES

    if(!webrs.size){
        WEBRS_NOT_READ
    }

    curl_easy_cleanup(curl);

    if((os_check = OS_detect()) == OS_NOT_FOUND){
        OS_NOT_SUPPORT
    }


    //get filename from github webpage source code.
    switch (os_check){
        case 0:
        case 1:
            for(int i=0, j=1, x=2; x < webrs.size; i+=3, j+=3, x+=3){
                if(webrs.memory[i] == 'd' && webrs.memory[j] == 'e' && webrs.memory[x] == 'b'){
                    for(int y = 0; (y < 64) && (webrs.memory[x-y] != '/') ; y++){
                        filename[y] = webrs.memory[x-y];
                    }
                    break;
                }
            }
            break;
        case 2:
            for(int i=0, j=1, x=2; x < webrs.size; i+=3, j+=3, x+=3){
                if(webrs.memory[i] == 'r' && webrs.memory[j] == 'p' && webrs.memory[x] == 'm'){
                    for(int y = 0; (y < 64) && (webrs.memory[x-y] != '/') ; y++){
                        filename[y] = webrs.memory[x-y];
                    }
                    break;
                }
            }
        default:
            OS_NOT_SUPPORT
            break;
    }

    free(webrs.memory);
    webrs.size = 0;
    ReverseString(filename);
    printf("filename: %s\n", filename);
    strcat(url_download, filename);
    printf("url_download: %s\n", url_download);

    pid = fork();
    if(pid == 0){
        execlp("wget", "wget", "-q", "--show-progress", url_download,  NULL);
    }else if(pid == -1){
        FORK_ERROR
    }

    wait(0);
    switch (OS_package[os_check].pack_install_name[0]){
        case 'd':
            pid = fork();
            if(pid == 0){
                execlp("sudo", "sudo", OS_package[os_check].pack_install_name, "-i", filename, NULL);
            }else if(pid == -1){
                FORK_ERROR
            }
            break;
        case 'r':
            pid = fork();
            if(pid == 0){
                execlp("sudo", "sudo", OS_package[os_check].pack_install_name, "install", filename, NULL);
            }else if(pid == -1){
                FORK_ERROR
            }
            break;
        default:
            OS_NOT_SUPPORT
            break;
    }
    wait(0);

    if(cur_version == 0){
        conf_AddString(conf->next->next, "version", version);
    }else  if(strcmp(cur_version, version) < 0){
        strcpy(conf->next->next->value, version);
    }

    return 0;
}

/*
Scan from the end of src until flag is found. The characters in the period are copied to dst and the order is reversed.
length = strlen(dst)
*/

void splitString(char *dst, char* str, char key, int length){
    char tmp = 0;
    char* p = dst;
    int flag = 0;
    int i = 0, j =0;

    for(i = strlen(str) - 1; (i > 0) && (flag < length); i--){
        if(str[i] == key){
            break;
        }
        *p = str[i];
        p++;  flag++;
    }
    ReverseString(dst);
}

void ReverseString(char *src){
    char tmp = 0;
    int i = 0, j =0;

    for(i = strlen(src) - 1, j = 0; i - j != 0; i--, j++){
        tmp = src[j];
        src[j] = src[i];
        src[i] = tmp;
    }
}

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
  size_t realsize = size * nmemb;
  struct WebSource *mem = (struct WebSource *)userp;
 
  char *ptr = realloc(mem->memory, mem->size + realsize + 1);
  if(ptr == NULL) {
    fprintf(stderr, "not enough memory (realloc returned NULL)\n");
    return 0;
  }
 
  mem->memory = ptr;
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
 
  return realsize;
}