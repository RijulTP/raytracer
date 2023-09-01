#include <iostream>
#include "../sdltemplate.h"
//#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "aarect.h"
#include "box.h"


#define PI 3.14159265

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
	
							                        // Desired objects can be included in this list
												 // Format centre, radius, material()
	float R = cos(M_PI/4);
	std::cout<<"Hello";

	// Add the boxes to the list of hitable objects
	int objNo = 6;
	hitable *list[objNo];

	for (int i = 0; i < 6; i++) {
		// Define the position of the ith box
		vec3 position(2.0 * cos(i * PI/3), 2.0 * sin(i * PI/3), 0);
		// Create the box
		box hex_box(position + vec3(-0.2, -0.2, -0.2), position + vec3(0.2, 0.2, 0.2), new lambertian(vec3(0.2,0.2,0.2)));
		// Rotate the box around the z-axis by some angle
		hitable *rotated_hex_box = new rotate_z(&hex_box, i*PI/3);
		// Add the rotated box to the vector of boxes
		list[i] = rotated_hex_box;
		//hexagon_boxes.push_back(rotated_hex_box);
}


	hitable *world = new hitable_list(list,objNo);
	vec3 lookfrom(0,0,20);
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