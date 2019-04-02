#include <iostream>

#include "Buffer.h"
#include "Descriptor.h"
#include "PPM.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	if(argn < 2){
		cerr << "Please, provide a tiara description scene" << endl;
		return 0;
	}

	Descriptor::run(argv[1]);
	return 0;	
}