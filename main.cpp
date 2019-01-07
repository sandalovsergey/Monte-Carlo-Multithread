#include "Const.h"
#include "utils.h"

void singleThread(std::vector<double>& x, std::vector<double>& y, int startIndex, long length, pcg64& genX, double& res) {
    fillMas(x,y, startIndex, length, genX);
    res = countArea(x,y);
}

void multiThread(std::vector<double>& x, std::vector<double>& y, double& res, pcg64& genX) {
    std::vector<pcg64> generatorsX(threads_num, genX);

    for (int i = 0; i < threads_num; ++i) {
        generatorsX[i].discard(i * length);
    }

    std::vector<std::thread> threads;
    threads.reserve(threads_num);


    for (int i = 0; i < threads_num; ++i) {
        threads.emplace_back(std::thread(fillMas, std::ref(x), std::ref(y), i * length, length,
                std::ref(generatorsX[i])));
    }

    for (auto &t : threads) {
        t.join();
    }

    res = countArea(x,y);
}

int main() {
    std::cout << "The program evaluates the area of the unit circle inscribed in a square [-1;1]x[-1;1]" << std::endl << std::endl;
    std::vector<double> x(N, 0);
    std::vector<double> y(N, 0);
    clock_t start = clock();


    double s1;
    pcg64 genX(1);
    singleThread(x, y, 0, N, genX, s1);
    printf("S1 is %10.8f\n", s1);
    std::cout << "Calculation in single thread took: "
              << ((float)(clock() - start))/CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    for(int i = 0; i < N; ++i) {
        x[i] = 0;
        y[i] = 0;
    }

    double s2;
    start = clock();
    multiThread(x,y,s2, genX);
    printf("S2 is %10.8f\n", s2);
    std::cout << "Calculation in " << threads_num << " threads took: "
         << ((float)(clock() - start))/CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    std::cout << "Abs(S1 - S2): " << std::abs(s1 - s2) << std::endl;

    return 0;
}