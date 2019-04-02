#include "PPM.h"
#include "Background.h"
#include <string>
#include <fstream>

std::string target::PPM::generator(target::Buffer & buffer, const std::string & fileName){
	
	std::string result = "P3\n";
	result += std::to_string(buffer.getWidth()) + " " + std::to_string(buffer.getHeight()) + "\n";
	result += std::to_string(255) + "\n";

	for(size_t i = 0; i < buffer.getHeight(); ++i){
		for(size_t j = 0; j < buffer.getWidth(); ++j){
			for(size_t k = 0; k < buffer.getDepth(); ++k){
				Color c = buffer.get(Point3d(j,i,k));
				result += std::to_string(int(c.r())) + " " + std::to_string(int(c.g())) + " " + std::to_string(int(c.b())) + " ";
			}			
		}
		result += "\n";
	}

	if(fileName.compare("")){
		std::ofstream file;
		file.open(fileName + ".ppm");
		file << result;
		file.close();
	}

	return result;
}