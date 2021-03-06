#pragma once

#include <types.h>
#include <traits.hpp>
#include <geometry/geometry.hpp>


template <typename G>
class View {
public:
    typename G::Map position = G::Map::identity();
    real field_of_view = 1.0_r;
    real lens_radius = 1e-2_r;
    real focal_length = 5.0_r;
};

template <typename G>
View<G> interpolate(const View<G> &a, const View<G> &b, real t) {
    return View<G>{
        .position = ::interpolate(a.position, b.position, t),
        .field_of_view = a.field_of_view*(1 - t) + b.field_of_view*t,
        .lens_radius = a.lens_radius*(1 - t) + b.lens_radius*t,
        .focal_length = a.focal_length*(1 - t) + b.focal_length*t,
    };
}

#ifdef HOST

namespace device {
template <typename G>
struct View {
    device_type<typename G::Map> pos;
    real fov;
    real lr;
    real fl;
};
}

template <typename G>
struct ToDevice<View<G>> {
    typedef device::View<G> type;
    static type to_device(const View<G> &v) {
        return type {
            .pos = ::to_device(v.position),
            .fov = ::to_device(v.field_of_view),
            .lr = ::to_device(v.lens_radius),
            .fl = ::to_device(v.focal_length),
        };
    }
};

#endif
