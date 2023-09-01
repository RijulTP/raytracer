#ifndef BOX_H
#define BOX_H

#include "rtweekend.h"

#include "aarect.h"
#include "hitable_list.h"


class box : public hitable  {
    public:
        box() {}
        box(const vec3& p0, const vec3& p1, material *ptr);

        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = aabb(box_min, box_max);
            return true;
        }

    public:
        vec3 box_min;
        vec3 box_max;
        hitable *sides[6];
        hitable *boxobject;
};

box::box(const vec3& p0, const vec3& p1, material *ptr) {
    //hitable *sides[6];
    std::cout<<"Hello";
    box_min = p0;
    box_max = p1;
    
    sides[0] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), new lambertian(vec3(1.0,0.0,0.0)));
    //sides[0] = new xy_rect(-3.0, 2.0, -3.0, 3.0, -2.0, new lambertian(vec3(1.0,0.0,0.0)));

    sides[1] = new xy_rect(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), new lambertian(vec3(1.0,0.0,0.0)));
    //sides[1] = new xy_rect(-3.0, 2.0, -3.0, 3.0, 1.0, new lambertian(vec3(1.0,0.0,0.0)));
    
    sides[2] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), new lambertian(vec3(0.0,1.0,0.0)));
    //sides[2] = new xz_rect(-3.0, 2.0, -3.0, 3.0, -2.0, new lambertian(vec3(0.0,1.0,0.0)));
    
    sides[3] = new xz_rect(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), new lambertian(vec3(0.0,1.0,0.0)));
    //sides[3] = new xz_rect(-3.0, 2.0, -3.0, 3.0, 2.0, new lambertian(vec3(0.0,1.0,0.0)));

    sides[4] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), new lambertian(vec3(0.0,0.0,1.0)));
    //sides[4] = new yz_rect(-3.0, 2.0, -3.0, 3.0, -2.0, new lambertian(vec3(0.0,0.0,1.0)));

    sides[5] = new yz_rect(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), new lambertian(vec3(0.0,0.0,1.0)));
    //sides[5] = new yz_rect(-3.0, 2.0, -3.0, 3.0, 2.0, new lambertian(vec3(0.0,0.0,1.0)));

    boxobject = new hitable_list(sides,6);
    this->boxobject = boxobject;
    }

bool box::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    return(boxobject->hit(r, t_min, t_max, rec));
}

#endif