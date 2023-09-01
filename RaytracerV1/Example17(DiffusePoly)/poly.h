#ifndef POLYH
#define HITABLEH

#include "hitable.h"

class poly: public hitable{
public:
    poly();
    poly(int data[100][2]){
        for (int i = 0; i <7; i++) {
            for (int j = 0; j < 2; j++) {
                polygon[i][j] = data[i][j];
            }
        }
    };
    virtual bool hit(const ray& r, int point[2],hit_record& rec) const;
    int polygon[100][2];
};

bool poly::hit(const ray& r, int point[2],hit_record& rec) const{
    bool is_in=false;
    int x=point[0];
    int y=point[1];
    int i=0;
	int n=6;

    for(int i=0; i < n; ++i){
        int x1=polygon[i][0];
        int x2=polygon[i+1][0];
        int y1=polygon[i][1];
        int y2=polygon[i+1][1];

        if(y < y1 != y < y2 && x < (x2-x1) * (y-y1) / (y2-y1) + x1){
            is_in=!is_in;
        }
    }
    
    return is_in;
};

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}
#endif