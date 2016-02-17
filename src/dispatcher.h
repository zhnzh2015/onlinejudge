#ifndef OJ_DISPATCHER_H
#define OJ_DISPATCHER_H

#include "common.h"
#include "fetcher.h"
#include <vector>
#include <fcntl.h>

namespace oj {

class Dispatcher {
public:
    explicit Dispatcher(Fetcher* fetcher,
                        uint32_t parallel_num,
                        const std::string& lock_fname,
                        const std::string& runner_bin) : 
                                        _m_fetcher(fetcher),
                                        _m_parallel_num(parallel_num),
                                        _m_process_task(0),
                                        _m_running_num(0),
                                        _m_running_slot(_m_parallel_num, 0),
                                        _m_lock_fname(lock_fname),
                                        _m_runner_bin(runner_bin) {}

    int32_t run(const std::string& work_home, bool loop = true);

    int32_t is_running() { return 0; }

    int32_t set_daemon() { return 0; }

private:
    void _run_client(const std::string& work_home, uint32_t run_id, uint32_t task_id);

    bool _match(uint32_t task_id) {
        // if (task_id % oj_tot != oj_mod) return false;
        return true;
    }

    int _lockfile(int fd);

private:
    Fetcher* _m_fetcher;
    uint32_t _m_parallel_num;
    uint32_t _m_process_task;
    uint32_t _m_running_num;
    std::vector<pid_t> _m_running_slot;
    std::string _m_lock_fname;
    std::string _m_runner_bin;

}; // END class Dispatcher

}; // END namespace oj

#endif // END OJ_DISPATCHER_H
