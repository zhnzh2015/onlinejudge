#include "mysql_fetcher.h"

namespace oj {

int32_t MysqlFetcher::init(const bm::configure::ConfigUnit& cfg) {
    _m_hostname = cfg["hostname"].to_cstr();
    _m_username = cfg["username"].to_cstr();
    _m_password = cfg["password"].to_cstr();
    _m_dbname = cfg["dbname"].to_cstr();
    _m_port = cfg["port"].to_uint32();

    _m_fetch_tasks_sql = cfg["fetch_tasks"].to_cstr();
    _m_fetch_solution_sql = cfg["fetch_solution"].to_cstr();
    _m_update_solution_res_sql = cfg["update_solution_res"].to_cstr();

    _m_conn = mysql_init(NULL);

    uint32_t timeout = cfg["timeout"].to_uint32();
    mysql_options(_m_conn, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);

    if (!mysql_real_connect(_m_conn,
                            _m_hostname.c_str(),
                            _m_username.c_str(),
                            _m_password.c_str(),
                            _m_dbname.c_str(),
                            _m_port, 0, 0)) {
        BM_LOG_FATAL("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    if (_execute_sql("set names utf8") < 0) {
        return -1;
    }
    return 0;
}

int32_t MysqlFetcher::fetch_tasks(std::vector<uint32_t>& tasks) {
    if (mysql_real_query(_m_conn, _m_fetch_tasks_sql.c_str(), _m_fetch_tasks_sql.size())) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(_m_conn);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res)) != NULL) {
        tasks.push_back(strtoul(row[0], NULL, 10));
    }
    mysql_free_result(res);
    return 0;
}

int32_t MysqlFetcher::fetch_solution(uint32_t id, std::string& solution) {
    char sql[256];
    snprintf(sql, sizeof(sql), _m_fetch_solution_sql.c_str(), id);

    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(_m_conn);
    MYSQL_ROW row = mysql_fetch_row(res);
    if (row == NULL) {
        BM_LOG_WARNING("Can't find solution by id %u, sql = [%s]\n", id, sql);
        return -1;
    }

    solution = row[0];
    mysql_free_result(res);
    return 0;
}

bool MysqlFetcher::update(uint32_t id, uint32_t result) {
    char sql[2048];
    snprintf(sql, sizeof(sql), _m_update_solution_res_sql.c_str(), result, id);

    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return false;
    }

    if (mysql_affected_rows(_m_conn) > 0ul) {
        return true;
    }
    return false;
}

int32_t MysqlFetcher::fetch_solution_info(uint32_t id, uint32_t& pid, uint32_t& userid) {
    char sql[128];
    sprintf(sql, "SELECT `problem_id`,`userid` FROM `oj_solution` where `id`=%u", id);

    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(_m_conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    pid = strtoul(row[0], NULL, 10);
    userid = strtoul(row[1], NULL, 10);

    mysql_free_result(res);
    return 0;
}

int32_t MysqlFetcher::fetch_solution_lang(uint32_t id, uint32_t& lang) {
    char sql[128];
    sprintf(sql, "SELECT `language` FROM `oj_solution` where `id`=%u", id);

    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(_m_conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    lang = strtol(row[0], NULL, 10);

    mysql_free_result(res);
    return 0;
}

int32_t MysqlFetcher::fetch_problem_info(uint32_t pid, uint32_t& mem_lmt, uint32_t& time_lmt,
                                                                            bool& isspj) {
    char sql[128];
    sprintf(sql, "SELECT `time_limit`,`memory_limit`,`spj` FROM `oj_problem` where `id`=%u", pid);

    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        return -1;
    }

    MYSQL_RES* res = mysql_store_result(_m_conn);
    MYSQL_ROW row = mysql_fetch_row(res);

    time_lmt = strtoul(row[0], NULL, 10);
    mem_lmt = strtoul(row[1], NULL, 10);
    isspj = (row[2][0] == '1');

    mysql_free_result(res);
    return 0;
}

void MysqlFetcher::clear() {
    _execute_sql("commit");
}

int32_t MysqlFetcher::_execute_sql(const char* sql) {
    if (mysql_real_query(_m_conn, sql, strlen(sql))) {
        BM_LOG_WARNING("[MYSQL ERROR] %s", mysql_error(_m_conn));
        sleep(20);
        _m_conn = NULL;
        return 1;
    }
    return 0;
}

/*
void _update_solution(int id, int result, int time, int memory, int sim, int sim_s_id,double pass_rate) {
    if(result==OJ_TL&&memory==0) result=OJ_ML;
    char sql[BUFFER_SIZE];
    if (oi_mode) {
        sprintf(sql, "UPDATE solution SET result=%d,time=%d,memory=%d,judgetime=NOW(),pass_rate=%f WHERE solution_id=%d LIMIT 1%c", result, time, memory, pass_rate,solution_id, 0);
    } else {
        sprintf(sql, "UPDATE solution SET result=%d,time=%d,memory=%d,judgetime=NOW() WHERE solution_id=%d LIMIT 1%c", result, time, memory, solution_id, 0);
    }
    if (mysql_real_query(conn, sql, strlen(sql))) {
        // printf("..update failed! %s\n",mysql_error(conn));
    }
    if (sim) {
        sprintf(sql, "insert into sim(s_id,sim_s_id,sim) values(%d,%d,%d) on duplicate key update  sim_s_id=%d,sim=%d", solution_id, sim_s_id, sim, sim_s_id, sim);
        // printf("sql= %s\n",sql);
        if (mysql_real_query(conn, sql, strlen(sql))) {
            // printf("..update failed! %s\n",mysql_error(conn));
        }
    }
}

void MysqlFetcher::addceinfo(uint32_t id, const std::string& info) {
    char sql[(1 << 16)], *end;
    char ceinfo[(1 << 16)], *cend;
    FILE *fp = fopen("ce.txt", "r");
    snprintf(sql, (1 << 16) - 1, "DELETE FROM compileinfo WHERE solution_id=%d", solution_id);
    mysql_real_query(conn, sql, strlen(sql));
    cend = ceinfo;
    while (fgets(cend, 1024, fp)) {
        cend += strlen(cend);
        if (cend - ceinfo > 40000) break;
    }
    cend = 0;
    end = sql;
    strcpy(end, "INSERT INTO compileinfo VALUES(");
    end += strlen(sql);
    *end++ = '\'';
    end += sprintf(end, "%d", solution_id);
    *end++ = '\'';
    *end++ = ',';
    *end++ = '\'';
    end += mysql_real_escape_string(conn, end, ceinfo, strlen(ceinfo));
    *end++ = '\'';
    *end++ = ')';
    *end = 0;
    //      printf("%s\n",ceinfo);
    if (mysql_real_query(conn, sql, end - sql)) printf("%s\n", mysql_error(conn));
    fclose(fp);
}

void _addreinfo_mysql(int solution_id,const char * filename) {
    char sql[(1 << 16)], *end;
    char reinfo[(1 << 16)], *rend;
    FILE *fp = fopen(filename, "r");
    snprintf(sql, (1 << 16) - 1,
                    "DELETE FROM runtimeinfo WHERE solution_id=%d", solution_id);
    mysql_real_query(conn, sql, strlen(sql));
    rend = reinfo;
    while (fgets(rend, 1024, fp)) {
        rend += strlen(rend);
        if (rend - reinfo > 40000) break;
    }
    rend = 0;
    end = sql;
    strcpy(end, "INSERT INTO runtimeinfo VALUES(");
    end += strlen(sql);
    *end++ = '\'';
    end += sprintf(end, "%d", solution_id);
    *end++ = '\'';
    *end++ = ',';
    *end++ = '\'';
    end += mysql_real_escape_string(conn, end, reinfo, strlen(reinfo));
    *end++ = '\'';
    *end++ = ')';
    *end = 0;
    //      printf("%s\n",ceinfo);
    if (mysql_real_query(conn, sql, end - sql)) printf("%s\n", mysql_error(conn));
    fclose(fp);
}

void MysqlFetcher::update_user(uint32_t userid) {
    char sql[BUFFER_SIZE];
    sprintf(sql, "UPDATE `users` SET `solved`=(SELECT count(DISTINCT `problem_id`) FROM `solution` WHERE `user_id`=\'%s\' AND `result`=\'4\') WHERE `user_id`=\'%s\'", user_id, user_id);
    if (mysql_real_query(conn, sql, strlen(sql))) write_log(mysql_error(conn));

    sprintf(sql, "UPDATE `users` SET `submit`=(SELECT count(*) FROM `solution` WHERE `user_id`=\'%s\') WHERE `user_id`=\'%s\'", user_id, user_id);
    if (mysql_real_query(conn, sql, strlen(sql))) write_log(mysql_error(conn));
}

void MysqlFetcher::update_problem(uint32_t pid) {
    char sql[BUFFER_SIZE];
    sprintf(sql, "UPDATE `problem` SET `accepted`=(SELECT count(*) FROM `solution` WHERE `problem_id`=\'%d\' AND `result`=\'4\') WHERE `problem_id`=\'%d\'", p_id, p_id);
    if (mysql_real_query(conn, sql, strlen(sql))) write_log(mysql_error(conn));
    sprintf(sql, "UPDATE `problem` SET `submit`=(SELECT count(*) FROM `solution` WHERE `problem_id`=\'%d\') WHERE `problem_id`=\'%d\'", p_id, p_id);
    if (mysql_real_query(conn, sql, strlen(sql))) write_log(mysql_error(conn));
}

void _get_custominput_mysql(int solution_id, char * work_dir) {
    char sql[BUFFER_SIZE], src_pth[BUFFER_SIZE];
    // get the source code
    MYSQL_RES *res;
    MYSQL_ROW row;
    sprintf(sql, "SELECT input_text FROM custominput WHERE solution_id=%d", solution_id);
    mysql_real_query(conn, sql, strlen(sql));
    res = mysql_store_result(conn);
    row = mysql_fetch_row(res);
    if(row!=NULL){
        // create the src file
        sprintf(src_pth, "data.in");
        FILE *fp_src = fopen(src_pth, "w");
        fprintf(fp_src, "%s", row[0]);
        fclose(fp_src);
    }
    mysql_free_result(res);
}
*/

} // END namespace oj
