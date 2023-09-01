#include <iostream>
#include "../sdltemplate.h"
//#include "ray.h"
#include "hitable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "aarect.h"
#include "box.h"
#include <vector>
#include <thread>
#include <mutex>

std::mutex mtx;

std::vector<std::thread> threads;
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
	
	//hitable *list[objNo];                        // Desired objects can be included in this list
												 // Format centre, radius, material()
	float R = cos(M_PI/4);
	std::cout<<"Hello";
	/*box box1(vec3(-0.75, 2.0, -0.5), vec3(1.25, 2.3, 0.25), new lambertian(vec3(0.2,0.2,0.2)));

	hitable *hex1 = new rotate_z(&box1,0);
	hex1 = new translate(hex1,vec3(0,0,0));

	hitable *hex2 = new rotate_z(&box1,-25);
	hex2 = new translate(hex2,vec3(1.5,0,0));

	list[0] = hex1;
	list[1] = hex2;
	list[2] = new sphere(vec3(0,-100.5,-1),100,new lambertian(vec3(0.8,0.8,0.0)));*/

	float boxSize = 0.50;
	vec3 boxOrientation(boxSize*2, 0,0);
	vec3 negBoxOrientation(-boxSize*2,0,0);

	// Create the boxes
	box box1(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));
	box box2(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));
	box box3(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));
	box box4(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));
	box box5(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));
	box box6(vec3(-boxSize, 0, -boxSize), vec3(boxSize*2, boxSize, boxSize), new lambertian(vec3(0.2,0.2,0.2)));

	// Rotate and translate the boxes to create a hexagon shape
	rotate_z *box2Rotated = new rotate_z(&box2, 60);
	translate *box2Translated = new translate(box2Rotated, boxOrientation);

	rotate_z *box3Rotated = new rotate_z(&box3, -60);
	translate *box3Translated = new translate(box3Rotated, negBoxOrientation);

	rotate_z *box4Rotated = new rotate_z(&box4, 180);
	translate *box4Translated = new translate(box4Rotated, boxOrientation * 3);

	rotate_z *box5Rotated = new rotate_z(&box5, 240);
	translate *box5Translated = new translate(box5Rotated, boxOrientation * 4);

	rotate_z *box6Rotated = new rotate_z(&box6, 300);
	translate *box6Translated = new translate(box6Rotated, boxOrientation * 5);

	// Add the boxes to the list of hitable objects
	int objNo = 3;
	hitable *list[objNo];
	list[0] = &box1;
	list[1] = box2Translated;
	list[2] = box3Translated;
	//list[3] = box4Translated;
	//list[4] = box5Translated;
	//list[5] = box6Translated;

	// Create the world object
	//hitable *world = new hitable_list(list, objNo);
	hitable *world = new hitable_list(list,objNo);
	vec3 lookfrom(0,0,20);
	vec3 lookat(0,0,0);
	float dist_to_focus = 20.0;
	float aperture = 0.1;

	camera cam(lookfrom,lookat,vec3(0,1,0),20,float(width)/float(height),aperture,dist_to_focus);        //lookfrom, lookat, vup,fov and aspect ratio
	///

	

for (int y = height - 1; y >= 0; y--) {
    for (int x = 0; x < width; x++) {
        // Launch a new thread for each pixel
        threads.push_back(std::thread([x, y,height,width, &world, &cam, ns]() {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x + drand48()) / float(width);
                float v = float(y + drand48()) / float(height);
                ray r = cam.get_ray(u, v);
                vec3 p = r.point_at_parameter(2.0);
                col += color(r, world, 0);
            }
            col /= float(ns);
            col = vec3(sqrt(col.r()), sqrt(col.g()), sqrt(col.b()));
            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);
            // Add lock to avoid concurrent access to the variable
            std::unique_lock<std::mutex> lock(mtx);
            if (x % width == 1) sdltemplate::loop();
            sdltemplate::setDrawColor(sdltemplate::createColor(ir, ig, ib, 255));
            sdltemplate::drawPoint(x, height - y);
        }));
    }
}

// Wait for all threads to finish
for (auto& t : threads) {
    t.join();
}

}