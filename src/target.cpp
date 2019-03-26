#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>

#include "Buffer.h"
#include "Color.h"
#include "Point2d.h"
#include "PPM.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	Buffer buffer(300,300, target::RED, target::BLUE);
	PPM::generator(buffer, "scene");

	return 0;	
}