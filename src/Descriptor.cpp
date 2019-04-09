#include "Descriptor.h"
#include "PPM.h"
#include "Background.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include <map>
#include <vector>

void target::Descriptor::run(const std::string & description){

	Buffer buffer;
	XMLDocument xmlTarget;
	Camera * camera;
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
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	for(size_t row = 0; row < buffer.getHeight(); ++row){
		for(size_t col = 0; col < buffer.getWidth(); ++col){
			Ray ray = camera->generate_ray(row,col);
			for(size_t depth = 0; depth < buffer.getDepth(); ++depth){
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
				auto x = pChild->FloatAttribute("x", 0.0);
				auto y = pChild->FloatAttribute("y", 0.0);
				auto z = pChild->FloatAttribute("z", 0.0);
				position = Vec3(x,y,z);
			}else if(!elementName.compare("Target") || !elementName.compare("target")){
				auto x =  pChild->FloatAttribute("x", 0.0);
				auto y =  pChild->FloatAttribute("y", 0.0);
				auto z =  pChild->FloatAttribute("z", 0.0);
				target = Vec3(x,y,z);
			}else if(!elementName.compare("Up") || !elementName.compare("up")){
				auto x = pChild->FloatAttribute("x", 0.0);
				auto y = pChild->FloatAttribute("y", 0.0);
				auto z = pChild->FloatAttribute("z", 0.0);
				up = Vec3(x,y,z);
			}else if(!elementName.compare("Vpdim") || !elementName.compare("vpdim")){
				l = pChild->FloatAttribute("l", 0.0);
				r = pChild->FloatAttribute("r", 0.0);
				b = pChild->FloatAttribute("b", 0.0);
				t = pChild->FloatAttribute("t", 0.0);
			}
		}

		camera = new OrthoCamera(position, target, up, width, height, l, r, b, t);
	}else if(!type.compare("perspective")){
		float fovy, fd, aspect;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Position") || !elementName.compare("position")){
				auto x = pChild->FloatAttribute("x", 0.0);
				auto y = pChild->FloatAttribute("y", 0.0);
				auto z = pChild->FloatAttribute("z", 0.0);
				position = Vec3(x,y,z);
			}else if(!elementName.compare("Target") || !elementName.compare("target")){
				auto x =  pChild->FloatAttribute("x", 0.0);
				auto y =  pChild->FloatAttribute("y", 0.0);
				auto z =  pChild->FloatAttribute("z", 0.0);
				target = Vec3(x,y,z);
			}else if(!elementName.compare("Up") || !elementName.compare("up")){
				auto x = pChild->FloatAttribute("x", 0.0);
				auto y = pChild->FloatAttribute("y", 0.0);
				auto z = pChild->FloatAttribute("z", 0.0);
				up = Vec3(x,y,z);
			}else if(!elementName.compare("Fovy") || !elementName.compare("fovy")){
				fovy = pChild->IntAttribute("value", 0);
			}else if(!elementName.compare("Aspect") || !elementName.compare("aspect")){
				aspect = pChild->IntAttribute("value", 0);
			}else if(!elementName.compare("Fdistance") || !elementName.compare("fdistance")){
				fd = pChild->IntAttribute("value", 0);
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