#include <iostream>

#include "Buffer.h"
#include "Descriptor.h"
#include "PPM.h"
#include "Ray.h"
#include "OrthoCamera.h"
//#include "PerspectiveCamera.h"
#include "Vec3.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	if(argn < 2){
		cerr << "Please, provide a tiara description scene" << endl;
		return 0;
	}

	Vec3 a(10,5,35);
	std::cout << a << std::endl;
	std::cout << a.norm() << std::endl;

	OrthoCamera camera( Vec3(0,0,0), Vec3(0,0,-10), Vec3(0,1,1), -3, 3, -2.25, 2.25 );

	Descriptor::run(argv[1]);
	return 0;	
}