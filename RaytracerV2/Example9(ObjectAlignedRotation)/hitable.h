#ifndef HITABLEH
#define HITABLEH
#include "ray.h"
#include "aabb.h"
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>

class material;

struct hit_record{
    float t;
    vec3 p;
    vec3 normal;
    material *mat_ptr;
    bool front_face;
    double u;
    double v;
    inline void set_face_normal(const ray& r, const vec3& outward_normal) {
    front_face = dot(r.direction(), outward_normal) < 0;
    normal = front_face ? outward_normal :-outward_normal;
    }
};

class hitable{
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const = 0;
    virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

class rotate_y : public hitable {
    public:
        rotate_y(hitable *ptr, double angle);

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        hitable *ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};

rotate_y::rotate_y(hitable *ptr, double angle) : ptr(ptr) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    vec3 min( infinity,  infinity,  infinity);
    vec3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newx =  cos_theta*x + sin_theta*z;
                auto newz = -sin_theta*x + cos_theta*z;

                vec3 tester(newx, y, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_y::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta*r.origin()[0] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[0] + cos_theta*r.origin()[2];

    direction[0] = cos_theta*r.direction()[0] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[0] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[0] =  cos_theta*rec.p[0] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[0] + cos_theta*rec.p[2];

    normal[0] =  cos_theta*rec.normal[0] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[0] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

class rotate_z : public hitable {
    public:
        rotate_z(hitable *ptr, double angle);

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        hitable *ptr;
        glm::mat3 rotation;
        bool hasbox;

        aabb bbox;
};

/*rotate_z::rotate_z(hitable *ptr, double angle) : ptr(ptr) {
    auto radians = degrees_to_radians(angle);
    rotation = glm::rotate(glm::mat3(1.0f), radians, glm::vec3(0, 0, 1));  // rotation about the z-axis
    hasbox = ptr->bounding_box(0, 1, bbox);

    vec3 min( infinity,  infinity,  infinity);
    vec3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newx =  cos_theta*x - sin_theta*y;
                auto newy =  sin_theta*x + cos_theta*y;

                vec3 tester(newx, newy, z);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}*/

rotate_z::rotate_z(hitable *ptr, double angle) : ptr(ptr) {
    auto radians = degrees_to_radians(angle);
    rotation = glm::rotate(glm::mat3(1.0f), radians, glm::vec3(0, 0, 1));  // rotation about the z-axis
    //rotation = glm::mat3_cast(glm::rotate(glm::mat3(1.0f), radians, glm::vec3(0, 0, 1)));
    glm::mat4 rotation_matrix = glm::mat4(glm::mat3(glm::rotate(glm::mat4(1.0f), radians, glm::vec3(0, 0, 1))));
    rotation = glm::mat3(rotation_matrix);
    hasbox = ptr->bounding_box(0, 1, bbox);
    
    vec3 min(INFINITY, INFINITY, INFINITY);
    vec3 max(-INFINITY, -INFINITY, -INFINITY);

    glm::vec3 corners[8] = {
        glm::vec3(bbox.min().x(), bbox.min().y(), bbox.min().z()),
        glm::vec3(bbox.min().x(), bbox.min().y(), bbox.max().z()),
        glm::vec3(bbox.min().x(), bbox.max().y(), bbox.min().z()),
        glm::vec3(bbox.min().x(), bbox.max().y(), bbox.max().z()),
        glm::vec3(bbox.max().x(), bbox.min().y(), bbox.min().z()),
        glm::vec3(bbox.max().x(), bbox.min().y(), bbox.max().z()),
        glm::vec3(bbox.max().x(), bbox.max().y(), bbox.min().z()),
        glm::vec3(bbox.max().x(), bbox.max().y(), bbox.max().z())
    };

    for (int i = 0; i < 8; i++) {
        corners[i] = corners[i] * rotation;
        min.e[0] = fmin(min.e[0], corners[i].x);
        min.e[1] = fmin(min.e[1], corners[i].y);
        min.e[2] = fmin(min.e[2], corners[i].z);
        max.e[0] = fmax(max.e[0], corners[i].x);
        max.e[1] = fmax(max.e[1], corners[i].y);
        max.e[2] = fmax(max.e[2], corners[i].z);
    }
    bbox = aabb(min, max);
}

bool rotate_z::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = vec3(r.origin().x*rotation(0,0)+r.origin().y*rotation(1,0)+r.origin().z*rotation(2,0),
                      r.origin().x*rotation(0,1)+r.origin().y*rotation(1,1)+r.origin().z*rotation(2,1),
                      r.origin().x*rotation(0,2)+r.origin().y*rotation(1,2)+r.origin().z*rotation(2,2));

    auto direction = vec3(r.direction().x*rotation(0,0)+r.direction().y*rotation(1,0)+r.direction().z*rotation(2,0),
                         r.direction().x*rotation(0,1)+r.direction().y*rotation(1,1)+r.direction().z*rotation(2,1),
                         r.direction().x*rotation(0,2)+r.direction().y*rotation(1,2)+r.direction().z*rotation(2,2));

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = vec3(rec.p.x*rotation(0,0)+rec.p.y*rotation(1,0)+rec.p.z*rotation(2,0),
                  rec.p.x*rotation(0,1)+rec.p.y*rotation(1,1)+rec.p.z*rotation(2,1),
                  rec.p.x*rotation(0,2)+rec.p.y*rotation(1,2)+rec.p.z*rotation(2,2));

    auto normal = vec3(rec.normal.x*rotation(0,0)+rec.normal.y*rotation(1,0)+rec.normal.z*rotation(2,0),
    rec.normal.x*rotation(0,1)+rec.normal.y*rotation(1,1)+rec.normal.z*rotation(2,1),
    rec.normal.x*rotation(0,2)+rec.normal.y*rotation(1,2)+rec.normal.z*rotation(2,2));

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
    }
                 



class rotate_x : public hitable {
    public:
        rotate_x(hitable *ptr, double angle);

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
            output_box = bbox;
            return hasbox;
        }

    public:
        hitable *ptr;
        double sin_theta;
        double cos_theta;
        bool hasbox;
        aabb bbox;
};

rotate_x::rotate_x(hitable *ptr, double angle) : ptr(ptr) {
    auto radians = degrees_to_radians(angle);
    sin_theta = sin(radians);
    cos_theta = cos(radians);
    hasbox = ptr->bounding_box(0, 1, bbox);

    vec3 min( infinity,  infinity,  infinity);
    vec3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                auto x = i*bbox.max().x() + (1-i)*bbox.min().x();
                auto y = j*bbox.max().y() + (1-j)*bbox.min().y();
                auto z = k*bbox.max().z() + (1-k)*bbox.min().z();

                auto newy =  cos_theta*y - sin_theta*z;
                auto newz = sin_theta*y + cos_theta*z;

                vec3 tester(x, newy, newz);

                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], tester[c]);
                    max[c] = fmax(max[c], tester[c]);
                }
            }
        }
    }

    bbox = aabb(min, max);
}

bool rotate_x::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    auto origin = r.origin();
    auto direction = r.direction();

    origin[1] = cos_theta*r.origin()[1] - sin_theta*r.origin()[2];
    origin[2] = sin_theta*r.origin()[1] + cos_theta*r.origin()[2];

    direction[1] = cos_theta*r.direction()[1] - sin_theta*r.direction()[2];
    direction[2] = sin_theta*r.direction()[1] + cos_theta*r.direction()[2];

    ray rotated_r(origin, direction, r.time());

    if (!ptr->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p;
    auto normal = rec.normal;

    p[1] =  cos_theta*rec.p[1] + sin_theta*rec.p[2];
    p[2] = -sin_theta*rec.p[1] + cos_theta*rec.p[2];

    normal[1] =  cos_theta*rec.normal[1] + sin_theta*rec.normal[2];
    normal[2] = -sin_theta*rec.normal[1] + cos_theta*rec.normal[2];

    rec.p = p;
    rec.set_face_normal(rotated_r, normal);

    return true;
}

class translate : public hitable {
    public:
        translate(hitable *ptr, const vec3& displacement)
            : ptr(ptr), offset(displacement) {}

        virtual bool hit(
            const ray& r, float t_min, float t_max, hit_record& rec) const override;

        virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

    public:
        hitable *ptr;
        vec3 offset;
};

bool translate::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p += offset;
    rec.set_face_normal(moved_r, rec.normal);
    return true;
}

bool translate::bounding_box(double time0, double time1, aabb& output_box) const {
    if (!ptr->bounding_box(time0, time1, output_box))
        return false;

    output_box = aabb(
        output_box.min() + offset,
        output_box.max() + offset);

    return true;
}

#endif

