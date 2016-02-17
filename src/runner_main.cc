#include <iostream>
#include <string>

#include "common.h"
#include "mysql_fetcher.h"
#include "Configure.h"
#include "dispatcher.h"
#include "log.h"
#include "cpp_runner.h"

using namespace std;

void print_res(oj::RunnerRes& res) {
    std::string result_str = "";
    switch (res.result) {
        case oj::COMPILING:
            result_str = "COMPILING";
            break;
        case oj::RUNNING:
            result_str = "RUNNING";
            break;
        case oj::ACCEPTED:
            result_str = "ACCEPTED";
            break;
        case oj::PRESENTATION_ERROR:
            result_str = "PRESENTATION_ERROR";
            break;
        case oj::WRONG_ANSWER:
            result_str = "WRONG_ANSWER";
            break;
        case oj::TIME_LIMIT_EXCEED:
            result_str = "TIME_LIMIT_EXCEED";
            break;
        case oj::MEMORY_LIMIT_EXCEED:
            result_str = "MEMORY_LIMIT_EXCEED";
            break;
        case oj::OUTPUT_LIMIT_EXCEED:
            result_str = "OUTPUT_LIMIT_EXCEED";
            break;
        case oj::RUNTIME_ERROR:
            result_str = "RUNTIME_ERROR";
            break;
        case oj::COMPILE_ERROR:
            result_str = "COMPILE_ERROR";
            break;
        default:
            result_str = "Unknown result";
            break;
    }
    fprintf(stderr, "Using time [%u] ms, Memory [%u] kB, Result [%s]\n",
                                    res.time, res.memory, result_str.c_str());
}

int main(int argc, char* argv[]) {
    uint32_t solution_id = 0;
    uint32_t run_id = 0;
    string work_home;

    int ch;
    while ((ch = getopt(argc, argv, "r:s:w:")) != -1) {
        switch (ch) {
        case 'r': run_id = strtoul(optarg, NULL, 10); break;
        case 's': solution_id = strtoul(optarg, NULL, 10); break;
        case 'w': work_home = optarg; break;
        default:
            BM_LOG_WARNING("Unsupported parameter [%c]\n", ch);
            break;
        }
    }
    bm::configure::Configure cfg;
    cfg.load("../conf", "oj.conf");

    oj::MysqlFetcher fetcher;
    fetcher.init(cfg["mysql"]);

    oj::Runner* runner = new oj::CppRunner(cfg["language"]["cpp"],
                                              &fetcher,
                                              work_home,
                                              run_id,
                                              solution_id);
    oj::RunnerRes res;
    runner->judge(res);

    print_res(res);

    return 0;
}
