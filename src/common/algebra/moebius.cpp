#include "moebius.hpp"

/*
quaternion mo_apply(Moebius m, quaternion p) {
    
}

quaternion mo_deriv(Moebius m, quaternion p, quaternion v) {
    quaternion u = cq_mul(m.s[0], p) + q_new(m.s[1], C0);
    quaternion d = cq_mul(m.s[2], p) + q_new(m.s[3], C0);
    real d2 = q_abs2(d);
    quaternion s1 = q_div(cq_mul(m.s[0], v), d);
    quaternion s21 = q_conj(cq_mul(m.s[2], v));
    quaternion s22 = ((real)2*dot(d, cq_mul(m.s[2], v))/d2)*q_conj(d);
    quaternion s2 = q_mul(u, (s21 - s22)/d2);
    return s1 + s2;
}

Moebius mo_interpolate(Moebius a, Moebius b, real t) {
    return mo_chain(a, mo_pow(mo_chain(mo_inverse(a), b), t));
}

real mo_diff(Moebius a, Moebius b) {
    return mo_fabs(mo_sub(a, b));
}
*/

#ifdef UNIT_TEST
#include <catch.hpp>
#include "test.hpp"

using namespace test;

TEST_CASE("Moebius transformation", "[moebius]") {
    Rng rng;

    SECTION("Chaining") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            Moebius<comp> a(rng.distrib<comp2x2>().normalized());
            Moebius<comp> b(rng.distrib<comp2x2>().normalized());
            quat c = rng.distrib<quat>().normal();
            REQUIRE(chain(a, b).apply(c) == approx(a.apply(b.apply(c))));
        }
    }
    /*
    SECTION("Derivation") {
        for (int i = 0; i < TEST_ATTEMPTS; ++i) {
            Moebius a = random_moebius(rng);
            quaternion p = rand_q_normal(rng);
            quaternion v = rand_q_nonzero(rng);
            
            REQUIRE(
                mo_deriv(a, p, v) ==
                ApproxV((mo_apply(a, p + EPS*v) - mo_apply(a, p))/EPS)
            );
        }
    }
    */
};
#endif // UNIT_TEST
