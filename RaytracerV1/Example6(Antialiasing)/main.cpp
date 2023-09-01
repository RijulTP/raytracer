#include <iostream>
#include "../sdltemplate.h"
//#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"

/*float hit_sphere(const vec3 center, float radius, const ray& r){
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
}*/

//vec3 sphere;
//float sphere_radius;

vec3 color(const ray& r,hitable *world) {
	hit_record rec;
	if(world->hit(r,0.001,MAXFLOAT,rec)){
		return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);

	}
	else{
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5*(unit_dir.y() + 1.0);
		return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
	}

}


int main() {
	//sphere = vec3(0,0,-1);
	//sphere_radius = 0.5;
	int width = 800;
	int height = 400;
	int ns = 100;
	std::cout << "P3\n" << width << " " << height << "\n255\n";
	sdltemplate::sdl("Ray Tracer", width, height);
	sdltemplate::loop();
	vec3 lower_left_corner(-2.0,-1.0,-1.0);  //Lower left corner as mentioned before
	vec3 horizontal(4.0,0.0,0.0);            //Length of the screen
	vec3 vertical(0.0,2.0,0.0);              //Height of the screen
	vec3 origin(0.0,0.0,0.0);                //Origin of the camera as mentioned before(0,0,0)
	///

	int objNo = 4;
	
	hitable *list[objNo];                        // Desired objects can be included in this list
	list[0] = new sphere(vec3(0,0,-1),0.5);
	list[1] = new sphere(vec3(0,-100.5,-1),100);
	list[2] = new sphere(vec3(-1,0,-1),0.25);
	list[3] = new sphere(vec3(1,0,-1),0.25);
	hitable *world = new hitable_list(list,objNo);
	camera cam;
	///
	for(int y=height-1; y>=00; y--) {
		for(int x=0; x<width; x++) {
			vec3 col(0,0,0);
			for(int s=0;s<ns;s++){
				float u = float(x+drand48()) / float(width);   //Eg x=1 and width is 800, then u is 1/800 units, the point is placed 1/800 horizontally
				float v = float(y+drand48()) / float(height);  //Eg y=398 and height is 400, then u is 398/800 units, the point is placed 398/800 vertically
				ray r = cam.get_ray(u,v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r,world);
			}
			col/=float(ns);
			
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			//std::cout << ir << " " << ig << " " << ib << "\n";
			sdltemplate::setDrawColor(sdltemplate::createColor(ir,ig,ib,255));
			sdltemplate::drawPoint(x,height-y);
		}
	}
	while(sdltemplate::running) {
		sdltemplate::loop();
	}
}