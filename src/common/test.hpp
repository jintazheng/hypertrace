#pragma once

#include <random>

#include <real.h>


#define TEST_ATTEMPTS 16

namespace test {

template <typename T>
class Distrib;

class Rng {
    friend class Distrib<real>;
private:
    std::minstd_rand gen;
    std::uniform_real_distribution<> unif;
    std::normal_distribution<> norm;

public:
    Rng(uint32_t seed) : gen(seed) {}
    Rng() : Rng(0xDEADBEEF) {}

    template <typename T>
    Distrib<T> &d() {
        return *reinterpret_cast<Distrib<T>*>(this);
    }
};
template <typename T>
Distrib<T> &random(Rng &rng) {
    return rng.d<T>();
}

template <>
class Distrib<real> : Rng {
public:
    real uniform() {
        return unif(gen);
    }
    real normal() {
        return norm(gen);
    }
};

inline Approx approx(real x) {
    return Approx(x);
}

}

#define COMMA ,

#define TEST_DEFINE_CMP_OPS(Template, prefix, Self, V) \
Template \
prefix bool operator==(V a, Self b) { \
    return b == a; \
} \
Template \
prefix bool operator!=(Self a, V b) { \
    return !(a == b); \
} \
Template \
prefix bool operator!=(V a, Self b) { \
    return b != a; \
} \

