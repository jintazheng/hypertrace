#pragma once

#include <types.h>
#include <math.hpp>
#include "traits.hpp"
#include "vector.hpp"
#include "complex.hpp"


template <typename T, int N>
class Rotation {};

template <typename T>
class Rotation<T, 2> {
private:
    complex<T> c;
public:
    Rotation() = default;
    explicit Rotation(T angle) : c(math::cos(angle), math::sin(angle)) {}
    explicit Rotation(complex<T> c) : c(c) {}
    static Rotation identity() {
        return Rotation(complex<T>(1, 0));
    }
    static Rotation look_at(vector<T, 2> pos) {
        return Rotation(normalize(complex<T>(pos.x, pos.y)));
    }

    complex<T> &comp() {
        return c;
    }
    const complex<T> &comp() const {
        return c;
    }

    vector<T, 2> apply(vector<T, 2> pos) const {
        return (c*complex<T>(pos)).vec();
    }
    friend Rotation chain(Rotation a, Rotation b) {
        return Rotation(a.c*b.c);
    }

    friend Rotation inverse(Rotation a) {
        return Rotation(~a.c);
    }
};

template <typename T>
class Rotation<T, 3> {
private:
    quaternion<T> c;
public:
    Rotation() = default;
    Rotation(vector<T, 3> axis, T angle) :
        c(math::cos(angle/2), math::sin(angle/2)*axis)
    {}
    explicit Rotation(quaternion<T> c) : c(c) {}
    static Rotation identity() {
        return Rotation(quaternion<T>(1, 0, 0, 0));
    }
    static Rotation look_at(vector<T, 3> pos) {
        vector<T, 3> dir = normalize(pos);
        vector<T, 3> axis(-dir[1], dir[0], zero<T>());
        T axis_length = length(axis);
        if (axis_length > EPS) {
            axis /= axis_length;
            T angle = math::atan2(axis_length, dir[2]);
            return Rotation(axis, angle);
        } else {
            if (dir[2] > zero<T>()) {
                return Rotation(quaternion<T>(1, 0, 0, 0));
            } else {
                return Rotation(quaternion<T>(0, 1, 0, 0));
            }
        }
    }

    quaternion<T> &quat() {
        return c;
    }
    const quaternion<T> &quat() const {
        return c;
    }

    vector<T, 3> apply(vector<T, 3> pos) const {
        quaternion<T> q(0, pos);
        return (c*q*~c).vec().shuffle(1,2,3);
    }
    friend Rotation chain(Rotation a, Rotation b) {
        return Rotation(a.c*b.c);
    }

    friend Rotation inverse(Rotation a) {
        return Rotation(~a.c);
    }
};

template <typename T, int N>
Rotation<T, N> operator*(Rotation<T, N> a, Rotation<T, N> b) {
    return chain(a, b);
}
template <typename T, int N>
Rotation<T, N> operator!(Rotation<T, N> a) {
    return inverse(a);
}


typedef Rotation<real, 2> Rotation2;
typedef Rotation<real, 3> Rotation3;


#ifdef UNIT_TEST
#include <catch.hpp>
#include "test.hpp"

namespace test {
template <typename T>
class Distrib<Rotation<T, 2>> : Rng {
public:
    Rotation<T, 2> some() {
        return Rotation<T, 2>(2*PI*d<T>().uniform());
    }
};
template <typename T>
class Distrib<Rotation<T, 3>> : Rng {
public:
    Rotation<T, 3> some() {
        return Rotation<T, 3>(
            d<vector<T, 3>>().unit(),
            2*PI*d<T>().uniform()
        );
    }
};
} // namespace test
#endif