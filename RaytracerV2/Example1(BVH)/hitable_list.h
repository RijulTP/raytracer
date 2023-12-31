#ifndef HITTABLELISTH
#define HITTABLELISTH
#include "hitable.h"
#include "aabb.h"

class hitable_list: public hitable{
    public:
        hitable_list(){};
        hitable_list(hitable **l, int n){list=l;list_size=n;};
        virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const;
        virtual bool bounding_box(
            double time0, double time1, aabb& output_box) const override;
        hitable **list; int list_size;
};

bool hitable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    for(int i=0;i<list_size;i++){  
        if(list[i]->hit(r,t_min,closest_so_far,temp_rec)){
          hit_anything=true;
          closest_so_far=temp_rec.t;
          rec=temp_rec;  
        }     
    }
    return hit_anything;
}

bool hitable_list::bounding_box(double time0, double time1, aabb& output_box) const {
    //if (list.empty()) return false;

    aabb temp_box;
    bool first_box = true;

    for(int i=0;i<list_size;i++){  
        if (!list[i]->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;  
    }
    /*for (const auto& object : list) {
        if (!object->bounding_box(time0, time1, temp_box)) return false;
        output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
        first_box = false;
    }*/

    return true;
}

#endif