#ifndef OJ_OKCALLS_H
#define OJ_OKCALLS_H

#include <sys/syscall.h>

namespace oj {

#define MAX_LIMIT -1

#ifdef __i386

int CPP_SYSCALL_LIM[][2] = {
    {8, MAX_LIMIT},
    {SYS_time, MAX_LIMIT},
    {SYS_read, MAX_LIMIT},
    {SYS_uname, MAX_LIMIT},
    {SYS_write, MAX_LIMIT},
    {SYS_open, MAX_LIMIT},
    {SYS_close, MAX_LIMIT},
    {SYS_execve, 1},
    {SYS_access, MAX_LIMIT},
    {SYS_brk, MAX_LIMIT},
    {SYS_munmap, MAX_LIMIT},
    {SYS_mprotect, MAX_LIMIT},
    {SYS_mmap2, MAX_LIMIT},
    {SYS_fstat64, MAX_LIMIT},
    {SYS_set_thread_area, MAX_LIMIT},
    {252, 2},
    {0, 0}
};

#else

int CPP_SYSCALL_LIM[][2] = {
    {8, MAX_LIMIT},
    {SYS_time, MAX_LIMIT},
    {SYS_read, MAX_LIMIT},
    {SYS_uname, MAX_LIMIT},
    {SYS_write, MAX_LIMIT},
    {SYS_open, MAX_LIMIT},
    {SYS_close, MAX_LIMIT},
    {SYS_execve, MAX_LIMIT},
    {SYS_access, MAX_LIMIT},
    {SYS_brk, MAX_LIMIT},
    {SYS_munmap, MAX_LIMIT}, 
    {SYS_mprotect, MAX_LIMIT},
    {SYS_mmap, MAX_LIMIT},
    {SYS_fstat, MAX_LIMIT},
    {SYS_set_thread_area, MAX_LIMIT},
    {252, 2},
    {SYS_arch_prctl, MAX_LIMIT},
    {231, MAX_LIMIT},
    {0, 0}
};

#endif // END __i386

} // END namespace oj

#endif // END OJ_OKCALLS_H
