#include <sys/types.h>

#define MAX_FILES 1024
#define MAX_PATH 1024

struct FileInfo{
    char name[256];
    off_t size;
    time_t mtime;
    int is_dir;
};

struct Panel{
    char current_path[MAX_PATH];
    struct FileInfo files[MAX_FILES];
    int file_count;
    int selected;
};