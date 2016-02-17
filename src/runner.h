#ifndef OJ_RUNNER_H
#define OJ_RUNNER_H

#include <string>

#include "fetcher.h"
#include "common.h"

namespace oj {

static const int SYSCALL_NUM = 512;

class Runner {
public:
    Runner() : _m_is_init(false) {}

    ~Runner() {}

    virtual int32_t init(const bm::configure::ConfigUnit& cfg,
                         Fetcher* fetcher,
                         const std::string& work_home,
                         uint32_t run_id,
                         uint32_t solution_id,
                         bool record_syscall);

    void judge(RunnerRes& res);

    virtual int32_t compile();

    virtual int32_t run();

protected:
    virtual int32_t _fetch_solution(const std::string& dir, uint32_t id);

    virtual void _run_solution();

    virtual void _watch_solution(pid_t pid);

    virtual void _clean_runner_home();

    virtual void _exec_compile() = 0;

    virtual void _exec_solution() = 0;

    virtual void _init_syscalls_limits() = 0;

protected:
    std::string _m_home;
    std::string _m_lang_ext;

    Fetcher* _m_fetcher;
    RunnerInfo _m_runner_info;

    JudgeRes _m_result;

    uint32_t _m_time;
    uint32_t _m_memory;

    bool _m_is_init;
    bool _m_record_syscall;
    int _m_syscall_counter[SYSCALL_NUM];
};

} // END namespace oj

#endif // END OJ_RUNNER_H
