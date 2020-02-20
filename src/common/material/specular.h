#pragma once

#include <algebra/quaternion.h>

#include <color.h>
#include <ray.h>


void specular_emit(
    color surface_color,
    const Ray *ray, Ray *new_ray,
    quaternion pos, quaternion norm    
) {
    new_ray->start = pos;
    quaternion hit_dir = qqq_move_dir(ray->start, ray->direction, pos);
    new_ray->direction = qq_sub(
        hit_dir,
        rq_mul((real)2*qq_dot(hit_dir, norm), norm)
    );
    new_ray->intensity = color_mulcc(ray->intensity, surface_color);
}