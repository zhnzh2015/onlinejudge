#include "dispatcher.h"

namespace oj {

int Dispatcher::run(const std::string& work_home, bool loop) {
    std::vector<uint32_t> tasks;
    size_t k = 0;
    while (loop) {
        _m_fetcher->fetch_tasks(tasks);
        pid_t pid = 0;
        for (std::vector<uint32_t>::const_iterator cit = tasks.begin(); cit != tasks.end(); ++cit) {
            if (!_match(*cit)) {
                continue;
            }

            if (_m_running_num >= _m_parallel_num) {
                pid = waitpid(-1, NULL, 0);
                for (k = 0; k < _m_running_slot.size(); ++k) {
                    if (pid == _m_running_slot[k]) {
                        _m_running_slot[k] = 0;
                        break;
                    }
                }
                if (k >= _m_running_slot.size()) {
                    BM_LOG_FATAL("waitpid got error pid, please check it!");
                    continue;
                }
                --_m_running_num;
                ++_m_process_task;
                BM_LOG_NOTICE("process task num: %d", _m_process_task);
            } else {
                for (k = 0; k < _m_running_slot.size(); ++k) {
                    if (_m_running_slot[k] == 0) {
                        break;
                    }
                }
            }

            if (_m_running_num < _m_parallel_num) {
                _m_fetcher->update(*cit, COMPILING);
                ++_m_running_num;
                _m_running_slot[k] = fork();
                if (_m_running_slot[k] == 0) {
                    _run_client(work_home, k, *cit);
                    exit(0);
                }
            }
            BM_LOG_NOTICE("Get task by id %d", *cit);
        }
        while ((pid = waitpid(-1, NULL, WNOHANG)) > 0) {
            --_m_running_num;
            ++_m_process_task;
            BM_LOG_NOTICE("process task num: %d", _m_process_task);
            for (k = 0; k < _m_running_slot.size(); ++k) {
                if (_m_running_slot[k] == pid) {
                    _m_running_slot[k] = 0;
                    break;
                }
            }
        }
        if (tasks.size() > 0) {
            tasks.clear();
        } else {
            sleep(3);
        }
    }
    return _m_process_task;
}

/*
int32_t Dispatcher::set_daemon() {
    pid_t pid = fork();

    if (pid < 0) {
        return -1;

    } else if (pid != 0) {
        exit(0); // parent exit
    }

    // child continues
    setsid(); // become session leader

    chdir(oj_home); // change working directory

    umask(0); // clear file mode creation mask

    close(0); // close stdin
    close(1); // close stdout
    close(2); // close stderr

    return 0;
}

int32_t Dispatcher::is_running() {
    int fd = open(_m_lock_fname.c_str(), O_RDWR | O_CREAT, LOCKMODE);
    if (fd < 0) {
        // FATAL LOG
        //syslog(LOG_ERR|LOG_DAEMON, "can't open %s: %s", LOCKFILE, strerror(errno));
        //exit(1);
        return 0;
    }
    if (_lockfile(fd) < 0) {
        if (errno == EACCES || errno == EAGAIN) {
            close(fd);
            return 1;
        }
        //FATAL LOG
        //syslog(LOG_ERR|LOG_DAEMON, "can't lock %s: %s", LOCKFILE, strerror(errno));
        return 1;
    }

    ftruncate(fd, 0);
    char buf[16];
    sprintf(buf, "%d", getpid());
    write(fd, buf, strlen(buf) + 1);
    close(fd);
    return 0;
}
*/

void Dispatcher::_run_client(const std::string& work_home, uint32_t run_id, uint32_t task_id) {
    struct rlimit LIM;
    LIM.rlim_max = 800;
    LIM.rlim_cur = 800;
    setrlimit(RLIMIT_CPU, &LIM);

    LIM.rlim_max = 80 * SIZE_MB;
    LIM.rlim_cur = 80 * SIZE_MB;
    setrlimit(RLIMIT_FSIZE, &LIM);

    LIM.rlim_max = SIZE_MB << 11;
    LIM.rlim_cur = SIZE_MB << 11;
    setrlimit(RLIMIT_AS, &LIM);

    // TODO 设置用户组最大的进程数量
    //LIM.rlim_max = 200;
    //LIM.rlim_cur = 200;
    //setrlimit(RLIMIT_NPROC, &LIM);

    char run_id_str[32], task_id_str[32];
    snprintf(task_id_str, sizeof(task_id_str), "%u", task_id);
    snprintf(run_id_str, sizeof(run_id_str), "%u", run_id);
    
    execl(_m_runner_bin.c_str(), _m_runner_bin.c_str(), "-s", task_id_str,
                                                        "-w", work_home.c_str(),
                                                        "-r", run_id_str,
                                                        (char*)NULL);
}

int Dispatcher::_lockfile(int fd) {
    struct flock fl;
    fl.l_type = F_WRLCK;
    fl.l_start = 0;
    fl.l_whence = SEEK_SET;
    fl.l_len = 0;
    return fcntl(fd, F_SETLK, &fl);
}

} // END namespace oj
