#include <iostream>

#include "Descriptor.h"
#include "Camera.h"
#include "Integrator.h"
#include <memory>

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	std::shared_ptr<Integrator> mIntegrator;
	std::shared_ptr<Camera> mCamera;
	std::shared_ptr<Scene> mScene;

	if(argn < 2){
		cerr << "Please, provide a TARget description scene" << endl;
		return 0;
	}
	
	Descriptor::run(argv[1], mIntegrator, mCamera, mScene);
	mIntegrator->render(*mScene);
	return 0;	
}