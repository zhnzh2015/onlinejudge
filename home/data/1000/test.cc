#include <iostream>
#include <ctime>

using namespace std;

int main() {

    clock_t last;
    while (true) {
        sleep(1);
        last = clock();
        double cnt = 1;
        for (int k = 0; k < 100000000; ++k) {
            cnt *= 1.0000001;
        }
        fprintf(stdout, "cnt = %lf\n", cnt);
        fprintf(stdout, "Using Time %.1lf ms\n\n", (clock() - last) * 10000 / CLOCKS_PER_SEC / 10.0);
    }

    return 0;
}
