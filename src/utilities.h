#ifndef OJ_UTILITIES_H
#define OJ_UTILITIES_H

#include <string>
#include <inttypes.h>

namespace oj {

int execute_cmd(const char* fmt, ...);

int get_proc_status(int pid, const char* mark);

long get_file_size(const char* fname);

std::string uint32_to_str(uint32_t num);

} // END namespace oj

#endif // END OJ_UTILITIES_H
