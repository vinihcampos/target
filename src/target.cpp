#include <iostream>

#include "Buffer.h"
#include "Descriptor.h"

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	if(argn < 2){
		cerr << "Please, provide a TARget description scene" << endl;
		return 0;
	}
	
	Descriptor::run(argv[1]);
	return 0;	
}