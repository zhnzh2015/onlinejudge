#include <iostream>
#include "common.h"
#include "mysql_fetcher.h"
#include "Configure.h"
#include "dispatcher.h"

using namespace std;

int main() {
    bm::configure::Configure cfg;
    cfg.load("../conf", "oj.conf");

    oj::MysqlFetcher fetcher;
    fetcher.init(cfg["mysql"]);

    oj::Dispatcher disp(&fetcher, 4, "../runner/lock", "./runner_main");
    disp.run("../home");

    return 0;
}
