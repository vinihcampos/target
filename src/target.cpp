#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>

#include "Canvas.h"
#include "Color.h"
#include "Point2d.h"
#include "PPM.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	Canvas canvas(300,300, target::RED, target::Color(255,0,255), target::BLUE, target::Color(255,0,255));
	PPM::generator(canvas, "scene");

	return 0;	
}