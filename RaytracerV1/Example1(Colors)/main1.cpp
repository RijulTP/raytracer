#include <iostream>
#include "../sdltemplate.h"
#include "vec3.h"

int main(){
    int width=800;      //Width and Height
    int height=400;
    sdltemplate::sdl("Ray Tracer",width,height); // A window is created with the dimensions
    sdltemplate::loop();

    for(int y=height-1;y>=00 && sdltemplate::running;y--){   //Up to down
        for(int x=0;x<width;x++){    //Left to right
            vec3 col(float(x) / float(width),float(y) / float(height),0.2);  // RGB
            int ir = int(255.99*col[0]); // Red
			int ig = int(255.99*col[1]); // Green
			int ib = int(255.99*col[2]); // Blue
            sdltemplate::setDrawColor(sdltemplate::createColor(ir,ig,ib,255)); //Draws the point using SDL
			sdltemplate::drawPoint(x,y);
        }
    }
    
    while(sdltemplate::running){
        sdltemplate::loop();
    }
}


