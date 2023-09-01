#include <iostream>
#include "../sdltemplate.h"
//#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "aarect.h"
#include "box.h"


vec3 color(const ray& r,hitable *world, int depth) {
	hit_record rec;
	if(world->hit(r,0.001,MAXFLOAT,rec)){
		//vec3 target = rec.p+rec.normal+random_in_unit_sphere();
		//return 0.5*color(ray(rec.p,target-rec.p),world);
		ray scattered;
		vec3 attenuation;
		if(depth < 50 && rec.mat_ptr->scatter(r,rec,attenuation,scattered)){
			return attenuation*color(scattered,world,depth+1);
		}
		else{
			return vec3(0,0,0);
		}
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
	std::cout<<"Hello";
	vec3 lower_left_corner(-2.0,-1.0,-1.0);  //Lower left corner as mentioned before
	vec3 horizontal(4.0,0.0,0.0);            //Length of the screen
	vec3 vertical(0.0,2.0,0.0);              //Height of the screen
	vec3 origin(0.0,0.0,0.0);                //Origin of the camera as mentioned before(0,0,0)
	///
std::cout<<"Hello";
	int objNo = 1;
	
	hitable *list[objNo];                        // Desired objects can be included in this list
												 // Format centre, radius, material()
	float R = cos(M_PI/4);
	//list[1] = new xy_rect(-3.0, 2.0, -3.0, 3.0, -2.0, new lambertian(vec3(0.85,0.85,0.85)));
	//list[1] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));
	std::cout<<"Hello";
	//list[0] = new box(vec3(130, 0, 65), vec3(295, 165, 230), new lambertian(vec3(0.2,0.2,0.2)));
	list[0] = new box(vec3(-1.50, -1.50, -1.0), vec3(1.0, 1.50, 0.5), new lambertian(vec3(0.2,0.2,0.2)));
	std::cout<<"Hello";
	/*list[2] = new sphere(vec3(1,0,-1),0.5,new metal(vec3(0.8,0.6,0.2),0.2));
	list[3] = new sphere(vec3(-1,0,-1),0.5,new dielectric(1.5));
	list[4] = new sphere(vec3(-1,0,-1),-0.45,new dielectric(1.5));*/
	hitable *world = new hitable_list(list,objNo);
	vec3 lookfrom(13,6,8);
	vec3 lookat(0,0,0);
	float dist_to_focus = 20.0;
	float aperture = 0.1;

	camera cam(lookfrom,lookat,vec3(0,1,0),20,float(width)/float(height),aperture,dist_to_focus);        //lookfrom, lookat, vup,fov and aspect ratio
	///
	for(int y=height-1; y>=00; y--) {
		for(int x=0; x<width; x++) {
			vec3 col(0,0,0);
			for(int s=0;s<ns;s++){
				float u = float(x+drand48()) / float(width);   //Eg x=1 and width is 800, then u is 1/800 units, the point is placed 1/800 horizontally
				float v = float(y+drand48()) / float(height);  //Eg y=398 and height is 400, then u is 398/800 units, the point is placed 398/800 vertically
				ray r = cam.get_ray(u,v);
				vec3 p = r.point_at_parameter(2.0);
				col += color(r,world,0);
			}
			col/=float(ns);
			col = vec3(sqrt(col.r()),sqrt(col.g()),sqrt(col.b()));
			int ir = int(255.99*col[0]);
			int ig = int(255.99*col[1]);
			int ib = int(255.99*col[2]);
			if(x%width == 1) sdltemplate::loop();
			//std::cout << ir << " " << ig << " " << ib << "\n";
			sdltemplate::setDrawColor(sdltemplate::createColor(ir,ig,ib,255));
			sdltemplate::drawPoint(x,height-y);
		}
	}
	while(sdltemplate::running) {
		sdltemplate::loop();
	}
}