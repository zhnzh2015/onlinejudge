#ifndef OJ_CPP_RUNNER_H
#define OJ_CPP_RUNNER_H

#include "common.h"
#include "fetcher.h"
//#include "okcalls.h"
#include "runner.h"

namespace oj {

class CppRunner : public Runner {
public:
    static const char* compile_cmd[11];

public:
    CppRunner(const bm::configure::ConfigUnit& cfg,
              Fetcher* fetcher,
              const std::string& work_home,
              uint32_t run_id,
              uint32_t solution_id);

protected:
    void _exec_compile();

    void _exec_solution();

    void _init_syscalls_limits();
};

} // END NAMESPACE oj

#endif // END OJ_CPP_RUNNER_H
