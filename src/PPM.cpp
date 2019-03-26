#include "PPM.h"
#include "Background.h"
#include <string>
#include <fstream>

std::string target::PPM::generator(target::Buffer & buffer, const std::string & fileName){
	
	for(size_t row = 0; row < buffer.getHeight(); ++row){
		for(size_t col = 0; col < buffer.getWidth(); ++col){
			buffer.pixel( Point2d(row, col), Background::interpolate( buffer, Point2d(row, col) ) );
		}
	}

	std::string result = "P3\n";
	result += std::to_string(buffer.getWidth()) + " " + std::to_string(buffer.getHeight()) + "\n";
	result += std::to_string(255) + "\n";

	for(size_t i = 0; i < buffer.getHeight(); ++i){
		for(size_t j = 0; j < buffer.getWidth(); ++j){
			Color c = buffer.get(Point2d(j,i));
			result += std::to_string(c.r) + " " + std::to_string(c.g) + " " + std::to_string(c.b) + " ";
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