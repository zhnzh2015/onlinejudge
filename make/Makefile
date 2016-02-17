all: main runner_main

BASE_ROOT=../../..
WORK_ROOT=..

INLCUDE=-I ${BASE_ROOT}/public/configure/include -I ${BASE_ROOT}/public/log/include -I ${BASE_ROOT}/third/boost/1.59.0/include
LIBARAY=-L ${BASE_ROOT}/public/configure/lib -lbm_configure \
		-L ${BASE_ROOT}/public/log/lib -lbm_log \
		-lmysqlclient

main: ${WORK_ROOT}/src/main.cc mysql_fetcher.o dispatcher.o utilities.o
	g++ -o main $^ ${LIBARAY} ${INLCUDE}

runner_main: ${WORK_ROOT}/src/runner_main.cc mysql_fetcher.o cpp_runner.o runner.o utilities.o compare.o
	g++ -o $@ $^ ${LIBARAY} ${INLCUDE}

mysql_fetcher.o: ${WORK_ROOT}/src/mysql_fetcher.cc ${WORK_ROOT}/src/mysql_fetcher.h
	g++ -c ${WORK_ROOT}/src/mysql_fetcher.cc ${INLCUDE}

dispatcher.o: ${WORK_ROOT}/src/dispatcher.cc ${WORK_ROOT}/src/dispatcher.h
	g++ -c ${WORK_ROOT}/src/dispatcher.cc ${INLCUDE}

utilities.o: ${WORK_ROOT}/src/utilities.cc ${WORK_ROOT}/src/utilities.h
	g++ -c ${WORK_ROOT}/src/utilities.cc ${INLCUDE}

runner.o: ${WORK_ROOT}/src/runner.cc ${WORK_ROOT}/src/runner.h ${WORK_ROOT}/src/utilities.h
	g++ -c ${WORK_ROOT}/src/runner.cc ${INLCUDE}

cpp_runner.o: ${WORK_ROOT}/src/cpp_runner.cc ${WORK_ROOT}/src/cpp_runner.h ${WORK_ROOT}/src/utilities.h
	g++ -c ${WORK_ROOT}/src/cpp_runner.cc ${INLCUDE}

compare.o: ${WORK_ROOT}/src/compare.cc ${WORK_ROOT}/src/compare.h
	g++ -c ${WORK_ROOT}/src/compare.cc ${INLCUDE}

clean:
	rm -f main
	rm -f runner_main
	rm -f *.o

#all: judged.cc
#	g++ -Wall -c -I/usr/local/mysql/include/mysql -I/usr/include/mysql  judged.cc
#	g++ -Wall -o judged judged.o -L/usr/local/mysql/lib/mysql -L/usr/lib/mysql  -lmysqlclient
#

