#ifndef RAYH
#define RAYH


#include "vec3.h"

class ray{
    public:
    ray(){}
    ray(const vec3& a, const vec3 b){A=a;B=b;}                       //A is the origin B is the direction
    vec3 origin() const {return A;}                                  //origin getter
    vec3 direction() const {return B;}                               //direction getter
    vec3 point_at_parameter(float t) const {return A+t*B;}

    vec3 A,B;
};

#endif