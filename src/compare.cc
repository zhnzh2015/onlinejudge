#include "compare.h"

namespace oj {

JudgeRes Compare::special(const char* spj_bin,
                          const char* in_fname,
                          const char* out_fname,
                          const char* user_fname) {
   pid_t pid = fork();
   if (pid == 0) {
        //while (setgid(1536) != 0) sleep(1);
        //while (setuid(1536) != 0) sleep(1);
        //while (setresuid(1536, 1536, 1536) != 0) sleep(1);

        struct rlimit LIM;
       
        LIM.rlim_max = LIM.rlim_cur = 5;
        setrlimit(RLIMIT_CPU, &LIM);
        alarm(0);
        alarm(10);
  
        LIM.rlim_cur = FSIZE_LIM;
        LIM.rlim_max = FSIZE_LIM + SIZE_MB;
        setrlimit(RLIMIT_FSIZE, &LIM);

        execl(spj_bin, spj_bin, "-i", in_fname, "-o", out_fname, "-u", user_fname, (char*)NULL);
        exit(0);
   } else {
        int status;
        waitpid(pid, &status, 0);
        if (status != 0) {
            return WRONG_ANSWER;
        }
   }
   return ACCEPTED;
}

JudgeRes Compare::compare(const char* expected_fname, const char* actual_fname) {
    FILE* fexpected = fopen(expected_fname, "r");
    FILE* factual = fopen(actual_fname, "r");
    if (fexpected == NULL || factual == NULL) {
        if (fexpected != NULL) fclose(fexpected);
        if (factual != NULL) fclose(factual);
        return RUNTIME_ERROR;
    }

    JudgeRes result = ACCEPTED;
    int expected, actual;
    bool pe = false;

    do {
        expected = fgetc(fexpected);
        actual = fgetc(factual);

        pe = _find_next_nonspace(expected, actual, fexpected, factual) || pe;
        if (expected == EOF && actual == EOF) break;

        while ((!isspace(expected) && expected) || (!isspace(actual) && actual)) {
            if (expected == EOF || actual == EOF) break;
            if (expected != actual) {
                result = WRONG_ANSWER;
                break;
            }
            expected = fgetc(fexpected);
            actual = fgetc(factual);
        }
        if (result == WRONG_ANSWER) {
            break;
        }

        pe = _find_next_nonspace(expected, actual, fexpected, factual) || pe;
        if (expected == EOF && actual == EOF) break;
        if (expected == EOF || actual == EOF) {
            result = WRONG_ANSWER;
            break;
        }
    } while (true);

    if (result == ACCEPTED && pe) {
        result = PRESENTATION_ERROR;
    }

    fclose(fexpected);
    fclose(factual);
    return result;
}

bool Compare::_find_next_nonspace(int& expected, int& actual, FILE* fexpected, FILE* factual) {
    bool pe = false;
    while (isspace(expected) || isspace(actual)) {
        if (expected != actual) {
            if (expected == '\r' && actual == '\n') {
                expected = fgetc(fexpected);

            } else if (actual == '\r' && expected == '\n') {
                actual = fgetc(factual);

            } else if (actual == EOF) {
                pe = true;
                do { expected = fgetc(fexpected); } while (isspace(expected));
                break;

            } else if (expected == EOF) {
                pe = true;
                do { actual = fgetc(factual); } while (isspace(actual));
                break;

            } else {
                pe = true;
            }
        }
        if (isspace(expected)) { expected = fgetc(fexpected); }
        if (isspace(actual)) { actual = fgetc(factual); }
    }
    return pe;
}

} // END namespace oj
