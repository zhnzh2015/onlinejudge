#include "cpp_runner.h"
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>

#include "utilities.h"
#include "okcalls.h"

namespace oj {

const char* CppRunner::compile_cmd[11] = {"g++", "Main.cc", "-o", "Main","-fno-asm", "-Wall",
                                          "-lm", "-std=c++0x", "-DONLINE_JUDGE", NULL};

CppRunner::CppRunner(const bm::configure::ConfigUnit& cfg,
                     Fetcher* fetcher,
                     const std::string& work_home,
                     uint32_t runner_id,
                     uint32_t solution_id) {
    Runner::init(cfg, fetcher, work_home, runner_id, solution_id, false);

    _m_lang_ext = ".cc";
}

void CppRunner::_exec_compile() {
    execvp(compile_cmd[0], (char * const *) compile_cmd);
}

void CppRunner::_exec_solution() {
    execl("./Main", "./Main", (char *)NULL);
}

void CppRunner::_init_syscalls_limits() {
    memset(_m_syscall_counter, 0, sizeof(_m_syscall_counter));
    if (_m_record_syscall) {
        for (int i = 0; i < SYSCALL_NUM; ++i) {
            _m_syscall_counter[i] = 0;
        }
    } else {
        for (int i = 0; CPP_SYSCALL_LIM[i][1] != 0; ++i) {
            _m_syscall_counter[CPP_SYSCALL_LIM[i][0]] = CPP_SYSCALL_LIM[i][1];
        }
    }
}

} // END namespace oj
