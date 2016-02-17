#ifndef OJ_COMMON_H
#define OJ_COMMON_H

#include <inttypes.h>

#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <unistd.h>
#include <time.h>
#include <stdarg.h>
#include <ctype.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <mysql/mysql.h>
#include <assert.h>

#include <string>

#define OJ_WT0 0
#define OJ_WT1 1
#define OJ_CO 12
#define OJ_TR 13

#ifdef __i386
#define REG_SYSCALL orig_eax
#define REG_RET eax
#define REG_ARG0 ebx
#define REG_ARG1 ecx
#else
#define REG_SYSCALL orig_rax
#define REG_RET rax
#define REG_ARG0 rdi
#define REG_ARG1 rsi
#endif

#define SIZE_MB 1048576
#define SIZE_KB 1024
#define STD_T_LIM 2
#define FSIZE_LIM (SIZE_MB << 5)
#define STD_M_LIM (SIZE_MB << 7)
#define BUFFER_SIZE 512

namespace oj {

enum JudgeRes {
    COMPILING = 2,
    RUNNING = 3,
    ACCEPTED = 4,
    PRESENTATION_ERROR = 5,
    WRONG_ANSWER = 6,
    TIME_LIMIT_EXCEED = 7,
    MEMORY_LIMIT_EXCEED = 8,
    OUTPUT_LIMIT_EXCEED = 9,
    RUNTIME_ERROR = 10,
    COMPILE_ERROR = 11,
};

struct RunnerInfo {
    uint32_t id;
    uint32_t solution_id;
    uint32_t problem_id;
    uint32_t userid;
    uint32_t time_lmt;
    uint32_t memory_lmt;
    uint32_t language;
    bool isspj;

    std::string root;
    std::string compile_info_fname;
    std::string input_fname;
    std::string output_fname;
    std::string user_fname;
    std::string error_fname;
    std::string diff_fname;
};

struct RunnerRes {
    int memory;
    int time;
    JudgeRes result;
};

} // END namespace oj

#endif // END OJ_COMMON_H
