#include "material.hh"

#include <algebra/rotation.hh>


void material_bounce(
    Material *material,
    Rng *rng, PathInfo *path,
    real3 hit_dir, real3 normal, real3 *bounce_dir,
    float3 *light, float3 *emission
) {
    *emission += *light*material->glow;
    if (rand_uniform(rng) < material->transparency) {
        *bounce_dir = hit_dir;
        path->face = !path->face;
    } else if (rand_uniform(rng) < material->gloss) {
        specular_bounce(
            hit_dir, normal, bounce_dir
        );
    } else {
        lambert_bounce(
            material->diffuse_color,
            rng,
            hit_dir, normal, bounce_dir,
            light
        );
        path->diffuse = true;
    }
}

void specular_bounce(
    real3 hit_dir, real3 normal, real3 *bounce_dir
) {
    *bounce_dir = hit_dir - (2*dot(hit_dir, normal))*normal;
}

void lambert_bounce(
    float3 color,
    Rng *rng,
    real3 hit_dir, real3 normal, real3 *bounce_dir,
    float3 *light
) {
    if (dot(normal, hit_dir) > 0) {
        normal = -normal;
    }
    real3 rand = rand_hemisphere_cosine(rng);
    rotation3 rot = rot3_look_at(normal);
    *bounce_dir = rot3_apply(rot, rand);
    
    *light *= color;
}

#ifdef OPENCL_INTEROP
void pack_material(MaterialPk *dst, const Material *src) {
    dst->diffuse_color = pack_float3(src->diffuse_color);
    dst->gloss = src->gloss;
    dst->transparency = src->transparency;
    dst->glow = pack_float3(src->glow);
}
void unpack_material(Material *dst, const MaterialPk *src) {
    dst->diffuse_color = unpack_float3(src->diffuse_color);
    dst->gloss = src->gloss;
    dst->transparency = src->transparency;
    dst->glow = unpack_float3(src->glow);
}
#endif // OPENCL_INTEROP