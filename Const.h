#include "pcg_random.hpp"
#include <thread>
#include <vector>
#include <c++/random>
const long N = 20000000l;
const int threads_num = 4;
const long generator_length = N / threads_num;
