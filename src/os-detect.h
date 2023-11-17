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

int OS_detect();