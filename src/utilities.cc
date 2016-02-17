#include "utilities.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BUFFER_SIZE 1024

namespace oj {

int execute_cmd(const char* fmt, ...) {
    char cmd[BUFFER_SIZE];

    int ret = 0;
    va_list ap;

    va_start(ap, fmt);
    vsprintf(cmd, fmt, ap);
    ret = system(cmd);
    va_end(ap);
    return ret;
}

int get_proc_status(int pid, const char* mark) {
    FILE* pf;
    char fn[BUFFER_SIZE], buf[BUFFER_SIZE];
    int ret = 0;
    sprintf(fn, "/proc/%d/status", pid);
    pf = fopen(fn, "r");
    int m = strlen(mark);
    while (pf && fgets(buf, BUFFER_SIZE - 1, pf)) {
        buf[strlen(buf) - 1] = 0;
        if (strncmp(buf, mark, m) == 0) {
            sscanf(buf + m + 1, "%d", &ret);
        }
    }
    if (pf) fclose(pf);
    return ret;
}

long get_file_size(const char* fname) {
    struct stat f_stat;
    if (stat(fname, &f_stat) == -1) {
        return 0;
    }
    return static_cast<long>(f_stat.st_size);
}

std::string uint32_to_str(uint32_t num) {
    char num_str[16];
    if (snprintf(num_str, sizeof(num_str), "%u", num) < 0) {
        return "";
    }
    return num_str;
}

} // END namespace oj
