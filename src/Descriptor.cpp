#include "Descriptor.h"
#include "PPM.h"
#include "Background.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Primitive.h"
#include "Sphere.h"
#include <map>
#include <vector>


void target::Descriptor::run(const std::string & description){

	Buffer buffer;
	XMLDocument xmlTarget;
	Camera * camera;
	std::vector<Primitive*> primitives;
	auto file = xmlTarget.LoadFile(description.c_str());
	auto * pRootElement = xmlTarget.RootElement();

	std::string elementName = "";
	std::map<std::string, std::string> settings;
	settings["name"] = "scene";
	settings["type"] = "PPM";

	for(XMLElement * pChild = pRootElement->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Camera") || !elementName.compare("camera")){
			target::Descriptor::processCamera(buffer, camera, pChild);
		}
	}

	for(XMLElement * pChild = pRootElement->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Camera") || !elementName.compare("camera")){
			continue;
		}else if(!elementName.compare("Background") || !elementName.compare("background")){
			target::Descriptor::processBackground(buffer, pChild);
		}else if(!elementName.compare("Settings") || !elementName.compare("settings")){
			settings = target::Descriptor::processSettings(buffer, pChild);
		}else if(!elementName.compare("Scene") || !elementName.compare("scene")){
			target::Descriptor::processScene(primitives, pChild);
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	//primitives.push_back(new Sphere(Vec3(-1,.5,-5), .4, "sphere1")); 
	//primitives.push_back(new Sphere(Vec3(1,-.5,-8), .4, "sphere2")); 
	//primitives.push_back(new Sphere(Vec3(-1,-1.5,-3.5), .4, "sphere3")); 

	for(size_t row = 0; row < buffer.getHeight(); ++row){
		for(size_t col = 0; col < buffer.getWidth(); ++col){
			Ray ray = camera->generate_ray(row,col);
			/*if(out_perspcamera_samp_300.find(std::make_pair(row,col)) != out_perspcamera_samp_300.end())
				//std::cout << (out_perspcamera_samp_300[std::make_pair(row,col)] == ray) << std::endl;
				std::cout << "[" << row << "," << col << "], " << (out_perspcamera_samp_300[std::make_pair(row,col)] == ray) << ", " << ray << ", " << out_perspcamera_samp_300[std::make_pair(row,col)] << std::endl;*/
			for(size_t depth = 0; depth < buffer.getDepth(); ++depth){
				bool intercepted = false;
				for(Primitive *p : primitives){
					if(p->intersect_p(ray)){
						intercepted = true;
						buffer.pixel( Point3d(col, row, depth), RED );	
					}
				}
				if(!intercepted)
					buffer.pixel( Point3d(col, row, depth), Background::interpolate( buffer, Point3d(col, row, depth) ) );
			}
		}
	}

	if(!settings["type"].compare("PPM")){
		PPM::generator(buffer, settings["name"]);
	}
}

std::map<std::string, std::string> target::Descriptor::processSettings(Buffer & buffer, XMLElement *& element){
	std::map<std::string, std::string> settings;

	std::string elementName;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Output") || !elementName.compare("output")){
			std::string name = "scene";
			std::string type = "PPM";

			if(pChild->Attribute("name") != NULL) 
				name = pChild->Attribute("name");

			if(pChild->Attribute("type") != NULL) 
				type = pChild->Attribute("type");

			settings["name"] = name;
			settings["type"] = type;
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	return settings;
}

void target::Descriptor::processCamera(Buffer & buffer, Camera *& camera, XMLElement *& element){

	int width, height, depth = 1;
	std::string elementName;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Width") || !elementName.compare("width")){
			width = pChild->IntAttribute("value", 400);
		}else if(!elementName.compare("Height") || !elementName.compare("height")){
			height = pChild->IntAttribute("value", 400);
		}else if(!elementName.compare("Depth") || !elementName.compare("depth")){
			depth = pChild->IntAttribute("value", 1);
		}
	}

	buffer = Buffer(width, height, depth);

	std::string type;
	if(element->Attribute("type") != NULL) 
		type = element->Attribute("type");

	Vec3 position, target, up;
	if(!type.compare("orthographic")){
		float l,r,b,t;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Position") || !elementName.compare("position")){
				auto x = pChild->DoubleAttribute("x", 0.0);
				auto y = pChild->DoubleAttribute("y", 0.0);
				auto z = pChild->DoubleAttribute("z", 0.0);
				position = Vec3(x,y,z);
			}else if(!elementName.compare("Target") || !elementName.compare("target")){
				auto x =  pChild->DoubleAttribute("x", 0.0);
				auto y =  pChild->DoubleAttribute("y", 0.0);
				auto z =  pChild->DoubleAttribute("z", 0.0);
				target = Vec3(x,y,z);
			}else if(!elementName.compare("Up") || !elementName.compare("up")){
				auto x = pChild->DoubleAttribute("x", 0.0);
				auto y = pChild->DoubleAttribute("y", 0.0);
				auto z = pChild->DoubleAttribute("z", 0.0);
				up = Vec3(x,y,z);
			}else if(!elementName.compare("Vpdim") || !elementName.compare("vpdim")){
				l = pChild->DoubleAttribute("l", 0.0);
				r = pChild->DoubleAttribute("r", 0.0);
				b = pChild->DoubleAttribute("b", 0.0);
				t = pChild->DoubleAttribute("t", 0.0);
			}
		}

		camera = new OrthoCamera(position, target, up, width, height, l, r, b, t);
	}else if(!type.compare("perspective")){
		double fovy, fd, aspect;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Position") || !elementName.compare("position")){
				auto x = pChild->DoubleAttribute("x", 0.0);
				auto y = pChild->DoubleAttribute("y", 0.0);
				auto z = pChild->DoubleAttribute("z", 0.0);
				position = Vec3(x,y,z);
			}else if(!elementName.compare("Target") || !elementName.compare("target")){
				auto x =  pChild->DoubleAttribute("x", 0.0);
				auto y =  pChild->DoubleAttribute("y", 0.0);
				auto z =  pChild->DoubleAttribute("z", 0.0);
				target = Vec3(x,y,z);
			}else if(!elementName.compare("Up") || !elementName.compare("up")){
				auto x = pChild->DoubleAttribute("x", 0.0);
				auto y = pChild->DoubleAttribute("y", 0.0);
				auto z = pChild->DoubleAttribute("z", 0.0);
				up = Vec3(x,y,z);
			}else if(!elementName.compare("Fovy") || !elementName.compare("fovy")){
				fovy = pChild->DoubleAttribute("value", 0);
			}else if(!elementName.compare("Aspect") || !elementName.compare("aspect")){
				aspect = pChild->DoubleAttribute("value", 0);
			}else if(!elementName.compare("Fdistance") || !elementName.compare("fdistance")){
				fd = pChild->DoubleAttribute("value", 0);
			}
		}

		camera = new PerspectiveCamera(position, target, up, width, height, fovy, fd, aspect);
	}
}

void target::Descriptor::processBackground(Buffer & buffer, XMLElement *& element){
	std::vector<Color> colors;
	std::string elementName;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Color") || !elementName.compare("color")){
			auto r = pChild->IntAttribute("r", 0);
			auto g = pChild->IntAttribute("g", 0);
			auto b = pChild->IntAttribute("b", 0);
			colors.push_back(Color(r,g,b));
		}
	}

	Color last = colors.back();
	while(colors.size() < 4)
		colors.push_back(last);

	buffer.setTl(colors[0]);
	buffer.setBl(colors[1]);
	buffer.setBr(colors[2]);
	buffer.setTr(colors[3]);

	colors.clear();
}

void target::Descriptor::processScene(std::vector<Primitive*> & primitives, XMLElement *& element){
	std::string elementName;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Object") || !elementName.compare("object")){
			processObject(primitives, pChild);
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}
}

void target::Descriptor::processObject(std::vector<Primitive*> & primitives, XMLElement *& element){
	std::string type = "";
	if(element->Attribute("type") != NULL) 
		type = element->Attribute("type");

	if(!type.compare("Sphere") || !type.compare("sphere")){
		primitives.push_back(processSphere(element));
	}else{
		std::cerr << "The object type is invalid" << std::endl;
	}
}

target::Sphere * target::Descriptor::processSphere(XMLElement *& element){
	std::string name = "";
	if(element->Attribute("name") != NULL) 
		name = element->Attribute("name");

	std::string elementName;
	double radius, x, y, z;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Radius") || !elementName.compare("radius")){
			radius = pChild->DoubleAttribute("value", 0.0);
		}else if(!elementName.compare("Center") || !elementName.compare("center")){
			x = pChild->DoubleAttribute("x", 0.0);
			y = pChild->DoubleAttribute("y", 0.0);
			z = pChild->DoubleAttribute("z", 0.0);
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	return new Sphere(Vec3(x,y,z), radius, name);
}