#include <iostream>
#include "../sdltemplate.h"
#include "ray.h"

vec3 color(const ray& r) {
	
	vec3 rdirection = r.direction();
	//std::cout<<"Rdirection is"<<rdirection<<"----";
	vec3 unit_dir = unit_vector(r.direction());
	//std::cout<<"UNIT DIR"<< unit_dir<<"---";
	float t = 0.5*(unit_dir.y() + 1.0);
	return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
}

int main() {
	int width = 800;
	int height = 400;
	//std::cout << "P3\n" << width << " " << height << "\n255\n";
	sdltemplate::sdl("Ray Tracer", width, height);
	sdltemplate::loop();
	vec3 lower_left_corner(-2.0,-1.0,-1.0);  //Lower left corner as mentioned before
	vec3 horizontal(4.0,0.0,0.0);            //Length of the screen
	vec3 vertical(0.0,2.0,0.0);              //Height of the screen
	vec3 origin(0.0,0.0,0.0);                //Origin of the camera as mentioned before(0,0,0)

	for(int y=height-1; y>=00; y--) {
		for(int x=0; x<width; x++) {
			float u = float(x) / float(width);   //Eg x=1 and width is 800, then u is 1/800 units, the point is placed 1/800 horizontally
			float v = float(y) / float(height);  //Eg y=398 and height is 400, then u is 398/800 units, the point is placed 398/800 vertically
			//std::cout<<"u and v"<< x<<"/"<<width<<" "<<"----"<<y<<"/"<<height<<"----**---";
			ray r(origin, lower_left_corner + u*horizontal + v*vertical); //ray (a,b) a=origin, b=direction, provides the exact coordinates
			vec3 col = color(r);
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