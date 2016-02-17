set names utf8; 
/* create database online_judge; */
use online_judge;

DROP TABLE IF EXISTS `oj_solution`;
CREATE TABLE `oj_solution` (
  `id` int(11) unsigned NOT NULL AUTO_INCREMENT,
  `userid` int(11) unsigned NOT NULL COMMENT '用户ID号',
  `problem_id` int(11) unsigned NOT NULL COMMENT '题目ID号',
  `time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '耗时(MS)',
  `memory` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '内存使用量(KB)',
  `language` smallint(3) unsigned NOT NULL DEFAULT '0' COMMENT '程序提交语言',
  `ip` bigint(20) NOT NULL DEFAULT '0' COMMENT '提交IP',
  `result` tinyint(3) unsigned NOT NULL DEFAULT '0' COMMENT '程序运行结果',
  `pass_rate` DECIMAL(2,2) unsigned NOT NULL DEFAULT 0,
  `source` text NOT NULL,
  `code_length` int(11) unsigned NOT NULL DEFAULT '0',
  `runtime_info` text COMMENT "runtime error info",
  `compile_info` text COMMENT "compile error info",
  `contest_id` int(11) unsigned DEFAULT NULL COMMENT '属于哪次比赛提交的解决方案',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `judge_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '最后更细时间',

  PRIMARY KEY (`id`),
  KEY `userid` (`userid`),
  KEY `problem_id` (`problem_id`),
  KEY `result` (`result`),
  KEY `contest_id` (`contest_id`)
) ENGINE=MyISAM AUTO_INCREMENT=1001 DEFAULT CHARSET=utf8;

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1001, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    unsigned long long a, b;
    while (cin >> a >> b) {
        cout << a + b << endl;
    }
    int cnt = 0;
    for (int i = 0; i < 220000000; ++i) {
        cnt += 1;
    }
    return 0;
}", 1454156041);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1002, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    int m[100000];
    int a, b;
    cin >> a >> b;
    cout << a+b << endl;
    int cnt = 0;
    vector<int> vect;
    for (int i = 0; i < 10000000; ++i) {
        cnt += 1;
        vect.push_back(cnt);
    }
    return 0;
}", 1454156042);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1003, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    int m[1000000];
    int a, b;
    cin >> a >> b;
    cout << a+b << endl;
    int cnt = 0;
    vector<int> vect;
    for (int i = 0; i < 100000000; ++i) {
        cnt += 1;
        vect.push_back(cnt);
    }
    return 0;
}", 1454156043);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1004, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    cout << a+b << endl;
    int cnt = 0;
    vector<int> vect;
    for (long long i = 0; i < 1000000000000ll; ++i) {
        cnt += 1;
        vect.push_back(cnt);
    }
    int test_array[1024 * 1024 * 1024];
    test_array[0] = 1000000;
    test_array[100000] = 9999;
    return 0;
}", 1454156044);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1005, 1, 1000, 1, "#include <iostream>
using namespace std;
int main() {
    //sleep(2);
    int a, b;
    cin >> a >> b
    cout << a+b << endl;
    return 0;
}", 1454156051);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1006, 1, 1000, 1, "#include <iostream>
using namespace std;
int main() {
    int a, b;
    scanf(\"%d%d\", &a, &b);
    fprintf(stdout, \"%d\\n\", a + b);
    return 0;
}", 1454156251);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1007, 1, 1000, 1, "#include <iostream>
using namespace std;
int main() {
    int a, b;
    cin >> a >> b;
    cout << a + b << endl;
    return 0;
}", 1454156293);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1008, 1, 1000, 1, "#include <iostream>
using namespace std;
int main() {
    unsigned long long a, b;
    while (cin >> a >> b) {
        cout << a + b << endl;
    }
    return 0;
}", 1454159293);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1009, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    unsigned long long a, b;
    while (cin >> a >> b) {
        cout << a + b << endl;
    }
    int cnt = 0;
    for (int i = 0; i < 200000000; ++i) {
        cnt += 1;
    }
    return 0;
}", 1454196041);

INSERT INTO `oj_solution`(`id`,`userid`,`problem_id`,`language`,`source`,`create_time`) VALUES(1010, 1, 1000, 1, "#include <iostream>
#include <vector>
using namespace std;
int main() {
    unsigned long long a, b;
    while (cin >> a >> b) {
        cout << ' ' << a + b << endl;
    }
    int cnt = 0;
    for (int i = 0; i < 200000000; ++i) {
        cnt += 1;
    }
    return 0;
}", 1454196341);

DROP TABLE IF EXISTS `oj_problem`;
CREATE TABLE `oj_problem` (
  `id` int(10) unsigned NOT NULL AUTO_INCREMENT,
  `title` varchar(200) NOT NULL,
  `description` text,
  `sample_input` text,
  `sample_output` text,
  `input` text,
  `output` text,
  `hint` text,
  `source` varchar(100) DEFAULT NULL,
  `spj` tinyint(3) NOT NULL DEFAULT '0',
  `time_limit` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '时间限制(MS)',
  `memory_limit` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '内存限制(KB)',
  `defunct` char(1) NOT NULL DEFAULT 'N',
  `accepted` int(11) DEFAULT '0' COMMENT 'accepted总数量',
  `submit` int(10) DEFAULT '0' COMMENT '提交总量',
  `solved` int(10) DEFAULT '0' COMMENT '解决此题目的总人数',
  `create_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '创建时间',
  `update_time` int(11) unsigned NOT NULL DEFAULT '0' COMMENT '最后更细时间',

  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=1000 DEFAULT CHARSET=utf8;

INSERT INTO `oj_problem`(`id`,`title`,`description`,`sample_input`,`sample_output`,`input`,`output`,`spj`,`time_limit`,`memory_limit`,`create_time`) VALUES("1000", "A+B Problem", "Calculate A+B", "1 2", "3", "-2 1
2147483647 2", "-1
2147483649", 0, 1000, 64000, 1454245436);

DROP TABLE IF EXISTS `oj_user`;
CREATE TABLE `oj_user` (
  `user_id` varchar(48) NOT NULL DEFAULT '',
  `email` varchar(100) DEFAULT NULL,
  `submit` int(11) DEFAULT '0',
  `solved` int(11) DEFAULT '0',
  `defunct` char(1) NOT NULL DEFAULT 'N',
  `ip` varchar(20) NOT NULL DEFAULT '',
  `accesstime` datetime DEFAULT NULL,
  `volume` int(11) NOT NULL DEFAULT '1',
  `language` int(11) NOT NULL DEFAULT '1',
  `password` varchar(32) DEFAULT NULL,
  `reg_time` datetime DEFAULT NULL,
  `nick` varchar(100) NOT NULL DEFAULT '',
  `school` varchar(100) NOT NULL DEFAULT '',
  PRIMARY KEY (`user_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `oj_sim`;
CREATE TABLE `oj_sim` (
  `s_id` int(11) NOT NULL,
  `sim_s_id` int(11) DEFAULT NULL,
  `sim` int(11) DEFAULT NULL,
  PRIMARY KEY (`s_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

DROP TABLE IF EXISTS `oj_custominput`;
CREATE TABLE `oj_custominput` (
  `solution_id` int(11) NOT NULL DEFAULT '0',
  `input_text` text,
  PRIMARY KEY (`solution_id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

