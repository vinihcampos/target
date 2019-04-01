#include "PPM.h"
#include "Background.h"
#include <string>
#include <fstream>

std::string target::PPM::generator(target::Buffer & buffer, const std::string & fileName){
	
	for(size_t row = 0; row < buffer.getHeight(); ++row){
		for(size_t col = 0; col < buffer.getWidth(); ++col){
			buffer.pixel( Point3d(row, col, 0), Background::interpolate( buffer, Point3d(row, col, 0) ) );
		}
	}

	std::string result = "P3\n";
	result += std::to_string(buffer.getWidth()) + " " + std::to_string(buffer.getHeight()) + "\n";
	result += std::to_string(255) + "\n";

	for(size_t i = 0; i < buffer.getHeight(); ++i){
		for(size_t j = 0; j < buffer.getWidth(); ++j){
			Color c = buffer.get(Point3d(j,i,0));
			result += std::to_string(int(c.r())) + " " + std::to_string(int(c.g())) + " " + std::to_string(int(c.b())) + " ";
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