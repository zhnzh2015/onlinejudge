#include "runner.h"
#include "utilities.h"
#include "compare.h"

namespace oj {

int32_t Runner::init(const bm::configure::ConfigUnit& cfg,
                     Fetcher* fetcher,
                     const std::string& work_home,
                     uint32_t runner_id,
                     uint32_t solution_id,
                     bool record_syscall) {
    _m_home = work_home;

    _m_runner_info.id = runner_id;
    _m_runner_info.solution_id = solution_id;
    _m_runner_info.problem_id = -1;
    _m_runner_info.userid = -1;
    _m_runner_info.time_lmt = -1;
    _m_runner_info.memory_lmt = -1;
    _m_runner_info.language = -1;
    _m_runner_info.isspj = false;

    _m_runner_info.root = _m_home + "/runner/" + uint32_to_str(runner_id);
    _m_runner_info.compile_info_fname = cfg["compile_info_fname"].to_cstr();
    _m_runner_info.input_fname= cfg["input_fname"].to_cstr();
    _m_runner_info.output_fname = cfg["output_fname"].to_cstr();
    _m_runner_info.user_fname = cfg["user_fname"].to_cstr();
    _m_runner_info.error_fname = cfg["error_fname"].to_cstr();
    _m_runner_info.diff_fname = cfg["diff_fname"].to_cstr();

    _m_fetcher = fetcher;
    _m_time = 0;
    _m_memory = 0;
    _m_result = ACCEPTED;
    _m_record_syscall = record_syscall;

    _m_is_init = true;
    return 0;
}

void Runner::judge(RunnerRes& res) {
    if (compile() == 0 && _m_result == ACCEPTED) {
        run();
    }

    res.memory = _m_memory;
    res.time = _m_time;
    res.result = _m_result;
}

int32_t Runner::compile() {
    if (!_m_is_init) {
        BM_LOG_WARNING("Runner not initialized!");
        return -1;
    }

    _clean_runner_home();

    int ret = _fetch_solution(_m_runner_info.root.c_str(), _m_runner_info.solution_id);
    if (ret < 0) {
        return -1;
    }

    int pid = fork();
    if (pid == 0) {
        chdir(_m_runner_info.root.c_str());
        struct rlimit LIM;
        LIM.rlim_max = 60;
        LIM.rlim_cur = 60;
        setrlimit(RLIMIT_CPU, &LIM);
        alarm(60);

        LIM.rlim_max = 900 * SIZE_MB;
        LIM.rlim_cur = 900 * SIZE_MB;
        setrlimit(RLIMIT_FSIZE, &LIM);

        LIM.rlim_max = SIZE_MB << 11;
        LIM.rlim_cur = SIZE_MB << 11;
        setrlimit(RLIMIT_AS, &LIM);

        freopen(_m_runner_info.compile_info_fname.c_str(), "w", stderr);
        _exec_compile();
        exit(0);
    }

    int status = 0;
    waitpid(pid, &status, 0);
    if (status != 0) {
        _m_result = COMPILE_ERROR;
        return -1;
    }
    return 0;
}

int32_t Runner::run() {
    int ret = _m_fetcher->fetch_solution_info(_m_runner_info.solution_id,
                                              _m_runner_info.problem_id,
                                              _m_runner_info.userid);
    if (ret < 0) {
        BM_LOG_WARNING("Solution [%u]'s target problem [%u] not exist!",
                                _m_runner_info.solution_id, _m_runner_info.problem_id);
        return -1;
    }

    char data_path[1024];
    snprintf(data_path, sizeof(data_path), "%s/data/%u/", _m_home.c_str(),
                                                         _m_runner_info.problem_id);

    DIR *dp;
    dirent *dirp;
    if (_m_runner_info.problem_id > 0 && (dp = opendir(data_path)) == NULL) {
        BM_LOG_FATAL("No such directory: [%s]", data_path);
        exit(-1);
    }

    ret = _m_fetcher->fetch_problem_info(_m_runner_info.problem_id, 
                                         _m_runner_info.memory_lmt,
                                         _m_runner_info.time_lmt,
                                         _m_runner_info.isspj);
    std::string fname;
    std::string prefix;
    while ((dirp = readdir(dp)) != NULL) {
        fname = dirp->d_name;
        if (fname.size() <= 3 || fname.substr(fname.size() - 3) != ".in") {
            continue;
        }
        prefix = fname.substr(0, fname.size() - 3);

        execute_cmd("cp %s%s.in %s/%s", data_path, prefix.c_str(),
                                _m_runner_info.root.c_str(), _m_runner_info.input_fname.c_str());

        _init_syscalls_limits();
        pid_t pid = fork();
        if (pid == 0) {
            chdir(_m_runner_info.root.c_str());
            _run_solution();
            exit(0);
        } else {
            _watch_solution(pid);
            if (_m_result != ACCEPTED && _m_result != PRESENTATION_ERROR) {
                break;
            }

            if (_m_time > _m_runner_info.time_lmt) {
                _m_result = TIME_LIMIT_EXCEED;
                break;
            }

            if (_m_memory > _m_runner_info.memory_lmt) {
                _m_result = MEMORY_LIMIT_EXCEED;
                break;
            }

            if (_m_runner_info.isspj) {
                // TODO special judge 
            } else {
                _m_result = Compare::compare((data_path + prefix + ".out").c_str(),
                                 (_m_runner_info.root + "/" + _m_runner_info.output_fname).c_str());
            }
            if (_m_result != ACCEPTED && _m_result != PRESENTATION_ERROR) {
                break;
            }
        }
    }

    return 0;
}

int32_t Runner::_fetch_solution(const std::string& dir, uint32_t id) {
    std::string solution;
    int32_t ret = _m_fetcher->fetch_solution(id, solution);
    if (ret != 0) {
        return ret;
    }

    FILE* fp_src = fopen((dir + "/Main" + _m_lang_ext).c_str(), "w");
    fprintf(fp_src, "%s", solution.c_str());
    fclose(fp_src);

    return 0;
}

void Runner::_run_solution() {
    nice(19);

    freopen(_m_runner_info.input_fname.c_str(), "r", stdin);
    freopen(_m_runner_info.output_fname.c_str(), "w", stdout);
    freopen(_m_runner_info.error_fname.c_str(), "a+", stderr);

    // trace me
    ptrace(PTRACE_TRACEME, 0, NULL, NULL);

    //while (setgid(1536) != 0) sleep(1);
    //while (setuid(1536) != 0) sleep(1);
    //while (setresuid(1536, 1536, 1536) != 0) sleep(1);
    struct rlimit LIM;

    LIM.rlim_cur = (_m_runner_info.time_lmt - _m_time) / 1000 + 1;
    LIM.rlim_max = LIM.rlim_cur;
    setrlimit(RLIMIT_CPU, &LIM);
    alarm(0);
    alarm(_m_runner_info.time_lmt * 10);

    LIM.rlim_max = FSIZE_LIM + SIZE_MB;
    LIM.rlim_cur = FSIZE_LIM;
    setrlimit(RLIMIT_FSIZE, &LIM);

    LIM.rlim_cur = LIM.rlim_max = 1;
    setrlimit(RLIMIT_NPROC, &LIM);

    LIM.rlim_cur = SIZE_MB << 6;
    LIM.rlim_max = SIZE_MB << 6;
    setrlimit(RLIMIT_STACK, &LIM);

    LIM.rlim_cur = SIZE_KB * _m_runner_info.memory_lmt / 2 * 3;
    LIM.rlim_max = SIZE_KB * _m_runner_info.memory_lmt * 2;
    setrlimit(RLIMIT_AS, &LIM);

    _exec_solution();
    exit(0);
}

void Runner::_watch_solution(pid_t pid) {
    FILE *ferr = fopen((_m_runner_info.root + "/" + _m_runner_info.error_fname).c_str(), "a+");
    struct user_regs_struct reg;
    struct rusage ruse;
    int status;
    bool entry = true;
    while (true) {
        wait4(pid, &status, 0, &ruse);
        _m_memory = std::max(_m_memory, static_cast<uint32_t>(get_proc_status(pid, "VmPeak:")));
        if (_m_memory > _m_runner_info.memory_lmt) {
            if (_m_result == ACCEPTED || _m_result == PRESENTATION_ERROR) {
                _m_result = MEMORY_LIMIT_EXCEED;
            }
            ptrace(PTRACE_KILL, pid, NULL, NULL);
            break;
        }

        if (WIFEXITED(status)) {
            break;
        }

        if (get_file_size((_m_runner_info.root + "/" + _m_runner_info.error_fname).c_str()) > 0) {
            _m_result = RUNTIME_ERROR;
            ptrace(PTRACE_KILL, pid, NULL, NULL);
            break;
        }

        if (!_m_runner_info.isspj) {
            int32_t user_file_size = get_file_size(_m_runner_info.user_fname.c_str());
            int32_t output_file_size = get_file_size(_m_runner_info.output_fname.c_str());
            if (user_file_size > output_file_size * 2 + 1024) {
                _m_result = OUTPUT_LIMIT_EXCEED;
                ptrace(PTRACE_KILL, pid, NULL, NULL);
                break;
            }
        }

        int exit_code = WEXITSTATUS(status);
        if (exit_code != 0x05 && exit_code != 0) {
            if (_m_result == ACCEPTED || _m_result == PRESENTATION_ERROR) {
                switch (exit_code) {
                    case SIGCHLD: 
                    case SIGALRM: alarm(0);
                    case SIGKILL: 
                    case SIGXCPU: _m_result = TIME_LIMIT_EXCEED; break;
                    case SIGXFSZ: _m_result = OUTPUT_LIMIT_EXCEED; break;
                    default: _m_result = RUNTIME_ERROR; break;
                }
                fprintf(ferr, "Runtime Error: %s\n", strsignal(exit_code));
            }
            ptrace(PTRACE_KILL, pid, NULL, NULL);
            break;
        }

        if (WIFSIGNALED(status)) {
            int sig_num = WTERMSIG(status);
            if (_m_result == ACCEPTED || _m_result == PRESENTATION_ERROR) {
                switch (sig_num) {
                    case SIGCHLD:
                    case SIGALRM: alarm(0);
                    case SIGKILL:
                    case SIGXCPU: _m_result = TIME_LIMIT_EXCEED; break;
                    case SIGXFSZ: _m_result = OUTPUT_LIMIT_EXCEED; break;
                    default: _m_result = RUNTIME_ERROR; break;
                }
                fprintf(ferr, "Runtime Error: %s\n", strsignal(sig_num));
            }
            break;
        }

        ptrace(PTRACE_GETREGS, pid, NULL, &reg);
        if (!_m_record_syscall && _m_syscall_counter[reg.REG_SYSCALL] == 0) {
            _m_result = RUNTIME_ERROR;
            BM_LOG_WARNING("[ERROR] Not allowed system call [%llu], solution_id [%d]\n",
                                reg.REG_SYSCALL, _m_runner_info.id);
            fprintf(ferr, "[ERROR] Not allowed system call [%llu]\n", reg.REG_SYSCALL);
            ptrace(PTRACE_KILL, pid, NULL, NULL);

        } else if (_m_record_syscall) {
            _m_syscall_counter[reg.REG_SYSCALL] = 1;

        } else {
            if (entry && _m_syscall_counter[reg.REG_SYSCALL] > 0) {
                --_m_syscall_counter[reg.REG_SYSCALL];
            }
        }
        entry = !entry;
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    }
    fclose(ferr);

    _m_time += (ruse.ru_utime.tv_sec * 1000 + ruse.ru_utime.tv_usec / 1000);
    _m_time += (ruse.ru_stime.tv_sec * 1000 + ruse.ru_stime.tv_usec / 1000);
}

void Runner::_clean_runner_home() {
    execute_cmd("rm -Rf %s/*", _m_runner_info.root.c_str());
}

} // END namespace oj
