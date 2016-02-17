#ifndef OJ_MYSQL_FETCHER_H
#define OJ_MYSQL_FETCHER_H

#include "fetcher.h"

#include <mysql/mysql.h>
#include <string>

#include "common.h"

namespace oj {

class MysqlFetcher : public Fetcher {
public:
    MysqlFetcher() {}

    int32_t init(const bm::configure::ConfigUnit& cfg);

    int32_t fetch_tasks(std::vector<uint32_t>& tasks);

    int32_t fetch_solution(uint32_t id, std::string& solution);

    int32_t fetch_solution_info(uint32_t id, uint32_t& pid, uint32_t& userid);

    int32_t fetch_solution_lang(uint32_t id, uint32_t& lang);

    int32_t fetch_problem_info(uint32_t pid, uint32_t& mem_lmt, uint32_t& time_lmt, bool& isspj);

    bool update(uint32_t id, uint32_t result);

    void clear();

private:
    int32_t _execute_sql(const char* sql);

private:
    std::string _m_hostname;
    std::string _m_username;
    std::string _m_password;
    std::string _m_dbname;
    uint32_t _m_port;

    std::string _m_fetch_tasks_sql;
    std::string _m_fetch_solution_sql;
    std::string _m_update_solution_res_sql;

    MYSQL* _m_conn;
};

} // END namespace oj

#endif
