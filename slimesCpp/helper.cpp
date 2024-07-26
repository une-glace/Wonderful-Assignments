#include "helper.h"

bool RandomWithProbability(double prob) {
    std::random_device dev;
    static std::uniform_real_distribution<double> u(0, 1);
    static std::default_random_engine e(dev());
    return u(e) <= prob;
}