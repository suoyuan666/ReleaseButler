#define OS_NOT_SUPPORT                                                              \
    fprintf(stderr, "releasebuter: the software is not suitable for this OS\n");    \
    return 1;
#define FORK_ERROR                                                                  \
    fprintf(stderr, "releasebuter: fork syscall error\n");                          \
    return 1;
#define PAK_HAS_INS                                                                 \
    fprintf(stderr, "releasebuter: software has been installed\n");                 \
    return 1;
#define CONNECT_FAILED                                                                                                          \
    fprintf(stderr, "releasebuter: connect to link(s) failed, error message: %s\n", curl_easy_strerror(res));                   \
    return 1;
#define CURL_INIT_FAILED                                                                                 \
    fprintf(stderr, "releasebuter: curl_init call failed, so the program will crash\n");                 \
    return 1;
#define FILE_NOT_OPEN                                                                     \
    fprintf(stderr, "releasebuter: some file cannot open, so the program cann't work\n"); \
    return 1;
#define WEBRS_NOT_READ                                                                                   \
    fprintf(stderr, "releasebuter: the source code of this GitHub Relase page cannot be read\n");        \
    return 1;
#define ENV_NOT_READ                                                                 \
    fprintf(stderr, "releasebuter: cann't read $HOME variable\n");                   \
    return 1;
#define FILE_NOT_READ                                                                     \
    fprintf(stderr, "releasebuter: some file cannot read, so the program cann't work\n"); \
    return 1;
#define CONF_CANNOT_ADD                             \
    fprintf(stderr, "connot add value to there\n"); \
    exit(1);
#define CONF_HAS_VALUE                                                           \
    fprintf(stderr, "conf object has value, so cannot insert value in there\n"); \
    exit(1);
#define CONF_LEN_OVER                                                               \
    fprintf(stderr, "argument is so long that the software cann't deal with it\n"); \
    exit(1);
#define BUF_OVERFLOW                                                \
    fprintf(stderr, "not enough memory (realloc returned NULL)\n"); \
    exit(1);