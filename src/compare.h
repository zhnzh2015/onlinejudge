#ifndef OJ_COMPARE_H
#define OJ_COMPARE_H

#include "common.h"

namespace oj {

class Compare {
public:
    static JudgeRes special(const char* spj_bin,
                            const char* in_fname,
                            const char* out_fname,
                            const char* user_fname);

    static JudgeRes compare(const char* expected_fname, const char* actual_fname);

private:
    static bool _find_next_nonspace(int& expected, int& actual, FILE* fexpected, FILE* factual);

}; // END class Compare

} // END namespace oj

#endif // END OJ_COMPARE_H
