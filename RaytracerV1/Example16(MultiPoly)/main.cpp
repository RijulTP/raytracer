#include <iostream>
#include "../sdltemplate.h"
#include "ray.h"
#include "hitable_list.h"
#include "poly.h"
//int polygon[][2] = { {100, 200}, {50, 275}, {150,275},{175,200},{100,200}};
int polygon[][2] = { {1, 200}, {50, 275}, {150,275},{200,200},{150,125},{50,125},{1, 200} };  //single layer
//int polygon[][2] = { {1, 200}, {50, 275}, {150,275},{200,200},{150,125},{50,125},{1, 200} ,{50, 200}, {50, 225}, {150,225},{150,200},{100,175},{50,175},{50, 200} }; //double layer


bool hit_poly(int point[2], int polygon[][2]){
    bool is_in=false;
    int x=point[0];
    int y=point[1];
    int i=0;
	int n=7;

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
}


vec3 sphere;
float sphere_radius;
int YLINE = 200;
int XLINE = 200;
int XCENTRE = 400;
int YCENTRE = 200;

int X1 = 50;
int Y1 = 50;

int X2 = 150;
int Y2 = 150;

int X3 = 250;
int Y3 = 250;


//int polygon [][2] = { {0, 400}, {100, 550}, {300,550},{350,400},{300,250},{100,250},{0, 400}};



vec3 color(const ray& r,int xCoord,int yCoord,hitable *world) {
	hit_record rec;
	int point[2]={xCoord,yCoord};
	if(world->hit(r,point,rec)){
		return 0.5*vec3(rec.normal.x()+1,rec.normal.y()+1,rec.normal.z()+1);

	}
	else{
		vec3 unit_dir = unit_vector(r.direction());
		float t = 0.5*(unit_dir.y() + 1.0);
		return (1.0-t)*vec3(1.0,1.0,1.0)+t*vec3(0.5,0.7,1.0);
	}

}

int main() {
	sphere = vec3(0,0,-1);
	sphere_radius = 0.5;
	int width = 800;
	int height = 400;
	//std::cout << "P3\n" << width << " " << height << "\n255\n";
	sdltemplate::sdl("Ray Tracer", width, height);
	sdltemplate::loop();
	vec3 lower_left_corner(-2.0,-1.0,-1.0);  //Lower left corner as mentioned before
	vec3 horizontal(4.0,0.0,0.0);            //Length of the screen
	vec3 vertical(0.0,2.0,0.0);              //Height of the screen
	vec3 origin(0.0,0.0,0.0);                //Origin of the camera as mentioned before(0,0,0)


	int objNo = 2;
	
	hitable *list[objNo];                        // Desired objects can be included in this list
	//int polygon1[100][2] = { {1, 200}, {50, 275}, {150,275},{200,200},{150,125},{50,125},{1, 200} };
	int polygon2[100][2] = { {301, 200}, {350, 275}, {450,275},{500,200},{450,125},{350,125},{301, 200} };
	int triangle1[100][2] = { {301, 200}, {350, 275}, {450,275},{500,200},{450,125},{350,125},{301,200} };
	int triangle2[100][2] = { {100, 200}, {150, 275}, {250,275},{300,200},{250,125},{150,125},{100,200} };
	//int polygon2[100][2] = { {1, 200}, {50, 275}, {150,275},{200,200},{150,125},{50,125},{1, 200}, {50, 275} };
	list[0] = new poly(triangle1);
	list[1] = new poly(triangle2);
	//list[1] = new poly(polygon1);
	hitable *world = new hitable_list(list,objNo);

	for(int y=height-1; y>=00; y--) {
		for(int x=0; x<width; x++) {
			float u = float(x) / float(width);   //Eg x=1 and width is 800, then u is 1/800 units, the point is placed 1/800 horizontally
			float v = float(y) / float(height);  //Eg y=398 and height is 400, then u is 398/800 units, the point is placed 398/800 vertically
			//std::cout<<"u and v"<< x<<"/"<<width<<" "<<"----"<<y<<"/"<<height<<"----**---";
			ray r(origin, lower_left_corner + u*horizontal + v*vertical); //ray (a,b) a=origin, b=direction, provides the exact coordinates
			vec3 col = color(r,x,y,world);
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