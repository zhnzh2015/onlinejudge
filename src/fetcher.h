#ifndef OJ_FETCHER_H
#define OJ_FETCHER_H

#include <string>
#include <vector>
#include "Configure.h"
#include "log.h"

namespace oj {

class Fetcher {
public:
    Fetcher() {}

    virtual int32_t init(const bm::configure::ConfigUnit& cfg) { return 0; }

    virtual int32_t fetch_tasks(std::vector<uint32_t>& tasks) = 0;

    virtual int32_t fetch_solution(uint32_t id, std::string& solution) = 0;

    virtual int32_t fetch_solution_info(uint32_t id, uint32_t& pid, uint32_t& userid) = 0;

    virtual int32_t fetch_solution_lang(uint32_t id, uint32_t& lang) = 0;

    virtual int32_t fetch_problem_info(uint32_t pid,
                                       uint32_t& mem_lmt,
                                       uint32_t& time_lmt,
                                       bool& isspj) = 0;

    virtual bool update(uint32_t id, uint32_t result) = 0;

    virtual void clear() {}
};

} // END namespace oj

#endif // END OJ_FETCHER_H
