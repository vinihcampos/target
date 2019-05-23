#include <iostream>

#include "Descriptor.h"
#include "Camera.h"
#include "Integrator.h"
#include <string>
#include <memory>

using namespace std;
using namespace target;

int main(int argn, char const *argv[]){
	
	std::shared_ptr<Integrator> mIntegrator;
	std::shared_ptr<Camera> mCamera;
	std::shared_ptr<Scene> mScene;

	std::string path_to_save = "./";

	if(argn < 2){
		cerr << "Please, provide a TARget description scene" << endl;
		return 0;
	}else if(argn >= 3){
		path_to_save = argv[2];
	}
	
	Descriptor::run(argv[1], mIntegrator, mCamera, mScene, path_to_save);
	mIntegrator->render(*mScene);
	return 0;	
}