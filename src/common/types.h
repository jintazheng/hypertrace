#pragma once

#ifndef OPENCL
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;
typedef unsigned long ulong;
#ifdef DEVICE
#define __address_space(as) __attribute__((address_space((uint)as)))
#define __private __address_space(0)
#define __global __address_space(1)
#define __constant __address_space(2)
#define __local __address_space(3)
#else
#define __address_space(as)
#define __private
#define __global
#define __constant
#define __local
#endif
#endif

#ifdef DEVICE
#ifdef X64
typedef ulong size_t;
#elif X32
typedef uint size_t;
#else
#error "Address bit width is not set"
#endif
#else
#include <cstdlib>
#endif

#include <real.h>


#ifdef __cplusplus

static_assert(sizeof(char) == 1, "Primitive type size check error");
static_assert(sizeof(short) == 2, "Primitive type size check error");
static_assert(sizeof(int) == 4, "Primitive type size check error");
static_assert(sizeof(long) == 8, "Primitive type size check error");

static_assert(sizeof(uchar) == 1, "Primitive type size check error");
static_assert(sizeof(ushort) == 2, "Primitive type size check error");
static_assert(sizeof(uint) == 4, "Primitive type size check error");
static_assert(sizeof(ulong) == 8, "Primitive type size check error");

static_assert(sizeof(float) == 4, "Primitive type size check error");
#if defined(HOST) || defined(DEVICE_DOUBLE)
static_assert(sizeof(double) == 8, "Primitive type size check error");
#endif

#ifdef DEVICE
static_assert(sizeof(size_t) == sizeof(void *), "Address bit width check error");
#endif


template <typename T, typename S>
struct pair {
    T first;
    S second;
    pair() = default;
    pair(T f, S s) : first(f), second(s) {}
};
template <typename T, typename S>
constexpr pair<T, S> make_pair(T f, S s) {
    return pair<T, S>(f, s);
}

#endif // __cplusplus
