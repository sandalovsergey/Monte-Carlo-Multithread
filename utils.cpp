//
// Created by Sergey Sandalov on 07.01.2019.
//

#include "utils.h"


double countArea(std::vector<double>& x, std::vector<double>& y) {
    long cnt = 0;
    for (long i = 0; i < N; ++i) {
        if(x[i] * x[i] + y[i] * y[i] < 1)
            ++cnt;
    }

    return cnt * 4.0 / N; //square area is 2*2=4
}

void fillMas(std::vector<double>& x, std::vector<double>& y, int startIndex, long length, pcg64& genX) {
    std::uniform_real_distribution<double> distX(-1, 1);
    std::uniform_real_distribution<double> distY(-1, 1);

    for(long i = startIndex; i < startIndex + length; ++i) {
        x[i] = distX(genX);
        y[i] = distY(genX);
    }
}