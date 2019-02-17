#include "Const.h"
void fillCoord(pcg64 genX, pcg64 genY, std::vector<double>& coordX, std::vector<double>& coordY,
        int start_index, long length) {
    std::uniform_real_distribution<double> distX(-1, 1);
    std::uniform_real_distribution<double> distY(-1,1);


    for(long i = start_index; i < start_index + length; ++i){
        coordX[i] = distX(genX);
        coordY[i] = distY(genY);
    }
}


double singleThread(pcg64 genX, pcg64 genY, std::vector<double> coordX, std::vector<double> coordY,
        int start_index,long length) {

    fillCoord(genX, genY, coordX, coordY, start_index, length);

    long cnt = 0;

    for (long i = 0; i < N; ++i) {
        if(coordX[i]*coordX[i] + coordY[i]*coordY[i] < 1)
            ++cnt;
    }

    return cnt*4.0 / N;
}

void count(std::vector<double>& res, int id, std::vector<pcg64> generatorsX, std::vector<pcg64> generatorsY) {
    pcg64 rngX = generatorsX[id];
    pcg64 rngY = generatorsY[id];
    std::uniform_real_distribution<double> distX(-1, 1);
    std::uniform_real_distribution<double> distY(-1,1);

    long cnt = 0;
    double x,y;
    for (long i = 0; i < generator_length; ++i) {
        x = distX(rngX);
        y = distY(rngY);
        if (x * x + y * y < 1)
            ++cnt;
    }

    res[id] = cnt;
}

double multiThread(std::vector<pcg64> generatorsX, std::vector<pcg64> generatorsY){
    for (int i = 0; i < threads_num; ++i) {
        generatorsX[i].discard(generator_length * i);
        generatorsY[i].discard(generator_length * i);
    }

    std::vector<std::thread> t;
    t.reserve(threads_num);

    std::vector<double> res;
    res.reserve(threads_num);

    for (int i = 0; i < threads_num; ++i) {
        t.emplace_back(std::thread(count, std::ref(res), i, generatorsX, generatorsY));
    }

    for (auto &t_cur : t) {
        t_cur.join();
    }

    long result = 0;
    for (int i = 0; i < threads_num; ++i) {
        result += res[i];
    }

    return result*4.0 / N;
}


int main() {
    std::cout << "The program evaluates the area of the unit circle inscribed in a square [-1;1]x[-1;1]" << std::endl << std::endl;
    clock_t start = clock();

    double s1;
    pcg64 genX(1);
    pcg64 genY(10);
    std::vector<double> coordX(N, 0);
    std::vector<double> coordY(N, 0);

    s1 = singleThread(genX, genY, coordX, coordY, 0, N);
    printf("S1 is %10.8f\n", s1);
    std::cout << "Calculation in single thread took: "
              << ((float)(clock() - start))/CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    for(long i = 0; i < N; ++i) {
        coordX[i] = 0;
        coordY[i] = 0;
    }

    double s2;

    std::vector<pcg64> generatorsX(threads_num, pcg64(1));
    std::vector<pcg64> generatorsY(threads_num, pcg64(10));

    start = clock();

    s2 = multiThread(generatorsX, generatorsY);
    printf("S2 is %10.8f\n", s2);
    std::cout << "Calculation in " << threads_num << " threads took: "
              << ((float)(clock() - start))/CLOCKS_PER_SEC << " seconds" << std::endl << std::endl;

    std::cout << "Abs(S1 - S2): " << std::scientific<< std::abs(s1 - s2) << std::endl;




    return 0;
}