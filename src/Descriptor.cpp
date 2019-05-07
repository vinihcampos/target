#include "Descriptor.h"
#include "PPM.h"
#include "Background.h"
#include "OrthoCamera.h"
#include "PerspectiveCamera.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Scene.h"
#include "Integrator.h"
#include "SampleIntegrator.h"
#include "FlatIntegrator.h"
#include "NormalMapIntegrator.h"
#include "DepthMapIntegrator.h"
#include <map>
#include <vector>


void target::Descriptor::run(const std::string & description){

	std::shared_ptr<Buffer> buffer;
	XMLDocument xmlTarget;
	std::shared_ptr<Camera> camera;
	Integrator * integrator;
	std::vector<std::shared_ptr<Primitive>> primitives;

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

	Scene scene(primitives);
	std::shared_ptr<Sampler> sampler = std::shared_ptr<Sampler>(new Sampler());
	std::shared_ptr<Material> farm = std::shared_ptr<Material>(new Material(GREEN));
	std::shared_ptr<Material> nearm = std::shared_ptr<Material>(new Material(BLACK));
	//integrator = new FlatIntegrator(camera, sampler);
	//integrator = new NormalMapIntegrator(camera, sampler);
	integrator = new DepthMapIntegrator(camera, farm, nearm, sampler);

	integrator->render(scene);

	/*for(size_t row = 0; row < buffer.getHeight(); ++row){
		for(size_t col = 0; col < buffer.getWidth(); ++col){
			Ray ray = camera->generate_ray(row,col);
			bool intercepted = false;
			for(std::shared_ptr<Primitive> p : primitives){
				if(p->intersect_p(ray)){
					intercepted = true;
					buffer.pixel( Point2(col, row), RED );	
				}
			}
			if(!intercepted)
				buffer.pixel( Point2(col, row), Background::sample( buffer, Point2(col, row) ) );
		}
	}

	if(!settings["type"].compare("PPM")){
		PPM::generator(buffer, settings["name"]);
	}*/
}

std::map<std::string, std::string> target::Descriptor::processSettings(std::shared_ptr<Buffer> & buffer, XMLElement *& element){
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

void target::Descriptor::processCamera(std::shared_ptr<Buffer> & buffer, std::shared_ptr<Camera> & camera, XMLElement *& element){

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

	buffer = std::shared_ptr<Buffer>(new Buffer(width, height, depth));

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

		camera = std::shared_ptr<OrthoCamera>( new OrthoCamera(buffer, position, target, up, width, height, l, r, b, t) );
	}else if(!type.compare("perspective")){
		double fovy, fd, aspect;
		aspect = width / (1.0 * height); 
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

		camera = std::shared_ptr<PerspectiveCamera>( new PerspectiveCamera(buffer, position, target, up, width, height, fovy, fd, aspect) );
	}
}

void target::Descriptor::processBackground(std::shared_ptr<Buffer> & buffer, XMLElement *& element){
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

	buffer->setTl(colors[0]);
	buffer->setBl(colors[1]);
	buffer->setBr(colors[2]);
	buffer->setTr(colors[3]);

	colors.clear();
}

void target::Descriptor::processScene(std::vector<std::shared_ptr<Primitive>> & primitives, XMLElement *& element){
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

void target::Descriptor::processObject(std::vector<std::shared_ptr<Primitive>> & primitives, XMLElement *& element){
	std::string type = "";
	if(element->Attribute("type") != NULL) 
		type = element->Attribute("type");

	if(!type.compare("Sphere") || !type.compare("sphere")){
		primitives.push_back(processSphere(element));

		std::shared_ptr<Material> m = std::shared_ptr<Material>(new Material(GREEN));
		primitives.back().get()->set_material(m);
	}else{
		std::cerr << "The object type is invalid" << std::endl;
	}
}

std::shared_ptr<target::Sphere> target::Descriptor::processSphere(XMLElement *& element){
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

	return std::shared_ptr<Sphere>(new Sphere(Vec3(x,y,z), radius, name));
}