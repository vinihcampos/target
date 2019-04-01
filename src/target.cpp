#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <math.h>
#include <fstream>
#include <sstream>

#include "Buffer.h"
#include "Point3d.h"
#include "PPM.h"
#include "Vec3.h"
#include "Mat3x3.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	Buffer buffer(300,300,1, target::BLACK, target::BLUE);
	PPM::generator(buffer, "scene");

	Vec3 u(10,3,5);
	Vec3 v(5,7,10);

	u = u.cross(v);

	cout << u << endl;

	return 0;	
}