//
// Created by Sergey Sandalov on 07.01.2019.
//

#ifndef MKMULTITHREAD_UTILS_H
#define MKMULTITHREAD_UTILS_H
#include "pcg_random.hpp"
#include <thread>
#include <vector>
#include <c++/random>
#include "Const.h"

double countArea(std::vector<double>& x, std::vector<double>& y);
void fillMas(std::vector<double>& x, std::vector<double>& y, int startIndex, long length, pcg64& genX);

#endif //MKMULTITHREAD_UTILS_H
