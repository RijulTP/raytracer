#include <iostream>

#include "sdltemplate.h"

#include "vec3.h"

#include "ray.h"

float hit_sphere(const vec3 center, float radius, const ray& r){
    vec3 oc=r.origin()-center;
    float a=dot(r.direction(),r.direction());
    float b=2.0*dot(oc,r.direction());
    float c=dot(oc,oc)-radius*radius;
    float discriminant=b*b-4*a*c;
    if (discriminant<0){
        return -1.0;
    }
    else{
        return(-b-sqrt(discriminant))/(2.0*a);
    }

    return (discriminant>0);

}

vec3 sphere;               //Sphere is defined at the centre

float sphere_radius;       

vec3 color(const ray& r){
    float t=hit_sphere(sphere,sphere_radius,r);
    if (t>0.0){
        vec3 N =unit_vector(r.point_at_parameter(t)-vec3(0,0,-1));
        return 0.5*vec3(N.x()+1,N.y()+1,N.z()+1);
    }
     // vec3 values indicate the color, and assigns the color at
                                                                //the point where this condition is satisfied, which gradually creates the circle
    vec3 unit_dir=unit_vector(r.direction());
    t=0.5*(unit_dir.y()+1.0);                // Direction of Background gradient can be set using x,y,z
    return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
}

int main(){

    sphere=vec3(0,0,-1);
    sphere_radius=0.5;

    int width=800;
    int height=400;
    std::cout <<"P3\n" << width << " " << height << "\n255\n";
    sdltemplate::sdl("Ray Tracer",width,height);
    sdltemplate::loop();

    vec3 lower_left_corner(-2.0,-1.0,-1.0);  //Lower left corner of the frame
    vec3 horizontal(4.0,0.0,0.0);            // Values depend on the aspect ratio of window
    vec3 vertical(0.0,2.0,0.0);
    vec3 origin(0.0,0.0,0.0);


    for(int y=height-1;y>=00;y--){   //Up to down
        for(int x=0;x<width;x++){    //Left to right

            float u=float(x) / float(width);
            float v=float(y) / float(height);
            ray r(origin,lower_left_corner+u*horizontal+v*vertical);

            //vec3 col(float(x) / float(width),float(y) / float(height),0.2);  // RGB

            vec3 col=color(r);                                             //Gradient
            int ir = int(255.99*col[0]); // Red
			int ig = int(255.99*col[1]); // Green
			int ib = int(255.99*col[2]); // Blue

            std::cout << ir << " " << ig << " " << ib << "\n";
            sdltemplate::setDrawColor(sdltemplate::createColor(ir,ig,ib,255)); //Draws the point using SDL
			sdltemplate::drawPoint(x,height-y);

        }
    }
    while(sdltemplate::running){
        sdltemplate::loop();
    }

}

