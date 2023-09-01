#ifndef AARECT_H
#define AARECT_H

#include "rtweekend.h"

#include "hitable.h"

class xy_rect : public hitable {
    public:
        xy_rect();

        xy_rect(float _x0, float _x1, float _y0, float _y1, float _k,material *mat)
            : x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat_ptr(mat) {};

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const;

    public:
        material *mat_ptr;
        float x0, x1, y0, y1, k;
};

bool xy_rect::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto t = (k-r.origin().z()) / r.direction().z();
    if (t < t_min || t > t_max)
        return false;
    auto x = r.origin().x() + t*r.direction().x();
    auto y = r.origin().y() + t*r.direction().y();
    if (x < x0 || x > x1 || y < y0 || y > y1)
        return false;
    rec.u = (x-x0)/(x1-x0);
    rec.v = (y-y0)/(y1-y0);
    rec.t = t;
    auto outward_normal = vec3(0, 0, 1);
    rec.set_face_normal(r, outward_normal);
    rec.mat_ptr = mat_ptr;
    rec.p = r.point_at_parameter(t);
    return true;
}
bool xy_rect::bounding_box(double time0, double time1, aabb& output_box) const{
            // The bounding box must have non-zero width in each dimension, so pad the Z
            // dimension a small amount.
            output_box = aabb(vec3(x0,y0,k-0.0001), vec3(x1, y1, k+0.0001));
            return true;
        };
#endif