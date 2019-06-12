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
#include "BlinnPhongIntegrator.h"
#include "FlatMaterial.h"
#include "BlinnPhongMaterial.h"
#include "AmbientLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "DirectionalLight.h"
#include "Triangle.h"
#include "cyTriMesh.h"
#include <sstream>
#include <map>
#include <vector>

void target::Descriptor::run(const std::string & description, std::shared_ptr<Integrator> & integrator, 
				std::shared_ptr<Camera> & camera, std::shared_ptr<Scene> & mScene, const std::string & path_to_save){

	std::shared_ptr<Buffer> buffer;
	XMLDocument xmlTarget;
	std::vector<std::shared_ptr<Primitive>> primitives;
	std::map<std::string, std::shared_ptr<Material>> materials;
	std::shared_ptr<SampleIntegrator> si;
	std::vector<std::shared_ptr<Light>> lights;

	auto file = xmlTarget.LoadFile(description.c_str());
	auto * pRootElement = xmlTarget.RootElement();

	std::string elementName = "";
	std::map<std::string, std::string> settings;
	settings["name"] = "scene";
	settings["type"] = "PPM";
	std::string integrator_type = "flat";

	for(XMLElement * pChild = pRootElement->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Camera") || !elementName.compare("camera")){
			target::Descriptor::processCamera(buffer, camera, pChild);
		}else if(!elementName.compare("Materials") || !elementName.compare("materials")){
			target::Descriptor::processMaterials(materials, pChild);
		}else if(!elementName.compare("Running") || !elementName.compare("running")){
			target::Descriptor::processSetup(si,  pChild);
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
			target::Descriptor::processScene(primitives, materials, lights, pChild);
		}else if(!elementName.compare("Materials") || !elementName.compare("materials")){
			continue;
		}else if(!elementName.compare("Running") || !elementName.compare("running")){
			continue;
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	mScene = std::shared_ptr<Scene>(new Scene(primitives, lights));
	std::shared_ptr<Sampler> sampler = std::shared_ptr<Sampler>(new Sampler());
	si->set_camera(camera);
	si->set_name(path_to_save + "/" + settings["name"]);
	si->set_sampler(sampler);

	integrator = std::shared_ptr<SampleIntegrator>(si);
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

void target::Descriptor::processScene(std::vector<std::shared_ptr<Primitive>> & primitives, std::map<std::string, std::shared_ptr<Material>> & materials, std::vector<std::shared_ptr<Light>> & lights, XMLElement *& element){
	std::string elementName;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Object") || !elementName.compare("object")){
			processObject(primitives, materials, pChild);
		}else if(!elementName.compare("Light") || !elementName.compare("light")){
			processLight(lights, pChild);
		}else if(!elementName.compare("Aggregate") || !elementName.compare("aggregate")){
			processAggregate(primitives, materials, pChild);
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}
}

void target::Descriptor::processObject(std::vector<std::shared_ptr<Primitive>> & primitives, std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element){
	std::string type = "";
	std::string material_name = "";

	if(element->Attribute("type") != NULL) 
		type = element->Attribute("type");

	if(element->Attribute("material") != NULL) 
		material_name = element->Attribute("material");

	if(!type.compare("Sphere") || !type.compare("sphere")){
		primitives.push_back(processSphere(element));

		if(material_name.compare("")){
			primitives.back().get()->set_material(materials[material_name]);
		}else{
			std::shared_ptr<Material> m = std::shared_ptr<Material>(new Material(GREEN));
			primitives.back().get()->set_material(m);
		}
	}else if(!type.compare("Mesh") || !type.compare("mesh")){
		std::vector< std::shared_ptr<Triangle> > triangles = processTriangle(element);
		for(std::shared_ptr<Triangle> t : triangles){
			primitives.push_back(t);

			if(material_name.compare("")){
				primitives.back().get()->set_material(materials[material_name]);
			}else{
				std::shared_ptr<Material> m = std::shared_ptr<Material>(new Material(GREEN));
				primitives.back().get()->set_material(m);
			}
		}
	}else{
		std::cerr << "The object type is invalid" << std::endl;
	}
}

void target::Descriptor::processAggregate(std::vector<std::shared_ptr<Primitive>> & primitives, std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element){
	std::string split_method_str = "";
	int max_prims_node;
	SplitMethod split_method;

	if(element->Attribute("split_method") != NULL) 
		split_method_str = element->Attribute("split_method");

	if(!split_method_str.compare("middle")){
		split_method = SplitMethod::Middle;
	}else{
		split_method = SplitMethod::EqualCounts;
	}


	max_prims_node = element->IntAttribute("max_prims_node", 1);

	std::vector<std::shared_ptr<Primitive>> primitives_bvh;

	std::string elementName;
	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Object") || !elementName.compare("object")){
			processObject(primitives_bvh, materials, pChild);
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	std::shared_ptr<BVHAccel> bvh = std::shared_ptr<BVHAccel>(new BVHAccel(primitives_bvh, max_prims_node, split_method));
	bvh->BVHBuild();

	primitives.push_back(bvh);
}

void target::Descriptor::processLight(std::vector<std::shared_ptr<Light>> & lights, XMLElement *& element){
	std::string type = "";
	std::string light_name = "";

	if(element->Attribute("type") != NULL) 
		type = element->Attribute("type");

	if(element->Attribute("name") != NULL) 
		light_name = element->Attribute("name");

	if(!type.compare("ambient")){
		Color intesity;
		std::string elementName;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Intensity") || !elementName.compare("intensity")){
				double r = pChild->DoubleAttribute("r", 0.0);
				double g = pChild->DoubleAttribute("g", 0.0);
				double b = pChild->DoubleAttribute("b", 0.0);
				intesity = Color(r,g,b);
			}else{
				std::cerr << "The element " << elementName << " is invalid" << std::endl;
			}
		}
		lights.push_back( std::shared_ptr<AmbientLight>(new AmbientLight(LightType::AMBIENT, light_name, intesity)) );
	}else if(!type.compare("point")){
		Color intesity;
		Point3 position;
		std::string elementName;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Intensity") || !elementName.compare("intensity")){
				double r = pChild->DoubleAttribute("r", 0.0);
				double g = pChild->DoubleAttribute("g", 0.0);
				double b = pChild->DoubleAttribute("b", 0.0);
				intesity = Color(r,g,b);
			}else if(!elementName.compare("Position") || !elementName.compare("position")){
				double x = pChild->DoubleAttribute("x", 0.0);
				double y = pChild->DoubleAttribute("y", 0.0);
				double z = pChild->DoubleAttribute("z", 0.0);
				position = Point3(x,y,z);
			}else{
				std::cerr << "The element " << elementName << " is invalid" << std::endl;
			}
		}
		lights.push_back( std::shared_ptr<PointLight>(new PointLight(LightType::POINT, light_name, intesity,position)) );
	}else if(!type.compare("directional")){
		Color intesity;
		Vec3 direction;
		std::string elementName;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Intensity") || !elementName.compare("intensity")){
				double r = pChild->DoubleAttribute("r", 0.0);
				double g = pChild->DoubleAttribute("g", 0.0);
				double b = pChild->DoubleAttribute("b", 0.0);
				intesity = Color(r,g,b);
			}else if(!elementName.compare("Direction") || !elementName.compare("direction")){
				double x = pChild->DoubleAttribute("x", 0.0);
				double y = pChild->DoubleAttribute("y", 0.0);
				double z = pChild->DoubleAttribute("z", 0.0);
				direction = Vec3(x,y,z);
			}else{
				std::cerr << "The element " << elementName << " is invalid" << std::endl;
			}
		}
		lights.push_back( std::shared_ptr<DirectionalLight>(new DirectionalLight(LightType::DIRECTIONAL, light_name, intesity, direction)) );
	}else if(!type.compare("spot")){
		Color intesity;
		Point3 position;
		Point3 point_at;
		double cutoff = 0.0;
		double falloff = 0.0;
		std::string elementName;
		for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
			elementName = pChild->Name();
			if(!elementName.compare("Intensity") || !elementName.compare("intensity")){
				double r = pChild->DoubleAttribute("r", 0.0);
				double g = pChild->DoubleAttribute("g", 0.0);
				double b = pChild->DoubleAttribute("b", 0.0);
				intesity = Color(r,g,b);
			}else if(!elementName.compare("Position") || !elementName.compare("position")){
				double x = pChild->DoubleAttribute("x", 0.0);
				double y = pChild->DoubleAttribute("y", 0.0);
				double z = pChild->DoubleAttribute("z", 0.0);
				position = Point3(x,y,z);
			}else if(!elementName.compare("Point_at") || !elementName.compare("point_at")){
				double x = pChild->DoubleAttribute("x", 0.0);
				double y = pChild->DoubleAttribute("y", 0.0);
				double z = pChild->DoubleAttribute("z", 0.0);
				point_at = Point3(x,y,z);
			}else if(!elementName.compare("Cutoff") || !elementName.compare("cutoff")){
				cutoff = pChild->DoubleAttribute("value", 0.0);
				cutoff = (cutoff * PI) / 180.0;
				cutoff = std::cos(cutoff);
			}else if(!elementName.compare("Falloff") || !elementName.compare("falloff")){
				falloff = pChild->DoubleAttribute("value", 0.0);
				falloff = (falloff * PI) / 180.0;
				falloff = std::cos(falloff);
			}else{
				std::cerr << "The element " << elementName << " is invalid" << std::endl;
			}
		}
		lights.push_back( std::shared_ptr<SpotLight>(new SpotLight(LightType::SPOT, light_name, intesity, position, 
			point_at, cutoff, falloff)) );
	}
}

std::shared_ptr<target::Sphere> target::Descriptor::processSphere(XMLElement *& element){
	std::string name = "";
	if(element->Attribute("name") != NULL) 
		name = element->Attribute("name");

	std::string elementName;
	double radius = 1;
	double x = 0;
	double y = 0;
	double z = 0;
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

std::vector< std::shared_ptr<target::Triangle> > target::Descriptor::processTriangle(XMLElement *& element){
	std::string name = "";
	if(element->Attribute("name") != NULL) 
		name = element->Attribute("name");

	std::string elementName;
	int n_triangles = 0;
	int n_vertices = 0;
	std::vector<Point3> vertices;
	std::vector<Vec3> normals;
	std::vector<int> indices;
	double v;

	bool clk = true;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("clockwise") || !elementName.compare("Clockwise")){
			clk = pChild->BoolAttribute("value", true);
		}
	}

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Filename") || !elementName.compare("filename")){
			std::string filename_obj = pChild->Attribute("value");
			return processMeshObject(filename_obj, name, clk);
		}
	}

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Ntriangles") || !elementName.compare("ntriangles")){
			n_triangles = pChild->IntAttribute("value", 0);
		}else if(!elementName.compare("Indices") || !elementName.compare("indices")){
			std::string indices_str = pChild->Attribute("value");
			std::stringstream ss; ss << indices_str;
			while(ss >> v){
				indices.push_back(v);
			}
		}else if(!elementName.compare("Vertices") || !elementName.compare("vertices")){
			std::string vertices_str = pChild->Attribute("value");
			std::stringstream ss; ss << vertices_str;
			while(ss >> v){
				Point3 p; p.x = v;
				ss >> v;  p.y = v;
				ss >> v;  p.z = v;
				vertices.push_back(p);
			}
		}else if(!elementName.compare("Normals") || !elementName.compare("normals")){
			std::string normals_str = pChild->Attribute("value");
			std::stringstream ss; ss << normals_str;
			while(ss >> v){
				Vec3 n; n.setX(v);
				ss >> v;  n.setY(v);
				ss >> v;  n.setZ(v);
				normals.push_back(n);
			}
		}else if(!elementName.compare("Uv") || !elementName.compare("uv")){
			continue;
		}else if(!elementName.compare("clockwise") || !elementName.compare("Clockwise")){
			continue;
		}else{
			std::cerr << "The element " << elementName << " is invalid" << std::endl;
		}
	}

	std::shared_ptr<TriangleMesh> mesh = std::shared_ptr<TriangleMesh>(new TriangleMesh(indices, vertices, normals));
	std::vector<std::shared_ptr<Triangle> > triangles;

	for(int i = 0; i < n_triangles; ++i){
		std::string name_tri = name + std::to_string(i);
		triangles.push_back( std::shared_ptr<Triangle>(new Triangle(name, mesh, i, clk)) );
	}

	return triangles;
}

std::vector< std::shared_ptr<target::Triangle> > target::Descriptor::processMeshObject(const std::string & filename, const std::string & name, const bool & clk){
	cy::TriMesh cyMesh;
	if(cyMesh.LoadFromFileObj(filename.c_str(), false)){

		int n_triangles = 0;
		int n_vertices = 0;
		std::vector<int> indices;
		std::vector<Point3> vertices;
		std::vector<Vec3> normals;

		n_triangles = cyMesh.NF();
		n_vertices = cyMesh.NV();

		// Getting the indices
		for(int i = 0; i < cyMesh.NF(); ++i){
			cy::TriMesh::TriFace f = cyMesh.F(i);
			for(int j = 0; j < 3; ++j) 
				indices.push_back(f.v[j]);
		}

		// Getting the vertices
		for(int i = 0; i < cyMesh.NV(); ++i){
			cy::Point3f p = cyMesh.V(i);
			vertices.push_back( Point3( p[0], p[1], p[2] ) );
		}

		// Getting the normals
		if((cyMesh.NVN() != cyMesh.NV()) || !cyMesh.HasNormals()){
			cyMesh.ComputeNormals(clk);
		}

		for(int i = 0; i < cyMesh.NVN(); ++i){
			cy::Point3f p = cyMesh.VN(i);
			normals.push_back( Vec3(p[0], p[1], p[2]) );
		}

		std::shared_ptr<TriangleMesh> mesh = std::shared_ptr<TriangleMesh>(new TriangleMesh(indices, vertices, normals));
		std::vector<std::shared_ptr<Triangle> > triangles;

		for(int i = 0; i < n_triangles; ++i){
			std::string name_tri = name + std::to_string(i);
			triangles.push_back( std::shared_ptr<Triangle>(new Triangle(name, mesh, i, clk)) );
		}

		return triangles;

	}else{
		throw "Fail to load object!\n";
	}
}

void target::Descriptor::processMaterials(std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element){
	std::string elementName;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Material") || !elementName.compare("material")){
			std::string type = "";
			std::string name = pChild->Attribute("name");

			if(pChild->Attribute("type") != NULL) {
				type = pChild->Attribute("type");
				if(!type.compare("flat") || !type.compare("Flat")){
					materials[name] =  std::shared_ptr<Material>(new FlatMaterial(processFlatMaterial(pChild)));
				}else if(!type.compare("blinn") || !type.compare("Blinn")){
					materials[name] = processBlinnMaterial(pChild);
				}
			}
		}
	}
}

target::Color target::Descriptor::processFlatMaterial(XMLElement *& element){
	std::string elementName;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Diffuse") || !elementName.compare("diffuse")){
			double r = pChild->DoubleAttribute("r", 0.0);
			double g = pChild->DoubleAttribute("g", 0.0);
			double b = pChild->DoubleAttribute("b", 0.0);

			return Color(r,g,b);
		}
	}

	return BLACK;
}

std::shared_ptr<target::Material> target::Descriptor::processBlinnMaterial(XMLElement *& element){
	std::string elementName;
	Color difuse, ambient, specular, mirror;
	double glossiness;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Diffuse") || !elementName.compare("diffuse")){
			double r = pChild->DoubleAttribute("r", 0.0);
			double g = pChild->DoubleAttribute("g", 0.0);
			double b = pChild->DoubleAttribute("b", 0.0);

			difuse = Color(r,g,b);
		}else if(!elementName.compare("Ambient") || !elementName.compare("ambient")){
			double r = pChild->DoubleAttribute("r", 0.0);
			double g = pChild->DoubleAttribute("g", 0.0);
			double b = pChild->DoubleAttribute("b", 0.0);

			ambient = Color(r,g,b);
		}else if(!elementName.compare("Specular") || !elementName.compare("specular")){
			double r = pChild->DoubleAttribute("r", 0.0);
			double g = pChild->DoubleAttribute("g", 0.0);
			double b = pChild->DoubleAttribute("b", 0.0);

			specular = Color(r,g,b);
		}else if(!elementName.compare("Mirror") || !elementName.compare("mirror")){
			double r = pChild->DoubleAttribute("r", 0.0);
			double g = pChild->DoubleAttribute("g", 0.0);
			double b = pChild->DoubleAttribute("b", 0.0);

			mirror = Color(r,g,b);
		}else if(!elementName.compare("Glossiness") || !elementName.compare("glossiness")){
			glossiness = pChild->DoubleAttribute("value", 0);
		}
	}

	return std::shared_ptr<Material>(new BlinnPhongMaterial(difuse, ambient, specular, glossiness, mirror));
}

void target::Descriptor::processSetup(std::shared_ptr<SampleIntegrator> & integrator, XMLElement *& element){
	std::string elementName;

	for(XMLElement * pChild = element->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement()){
		elementName = pChild->Name();
		if(!elementName.compare("Integrator") || !elementName.compare("integrator")){
			std::string type = pChild->Attribute("type");
			int spp, depth;
			if(!type.compare("depth")){
				std::shared_ptr<Material> far;
				std::shared_ptr<Material> near;
				for(XMLElement * pChild_ = pChild->FirstChildElement(); pChild_ != NULL; pChild_ = pChild_->NextSiblingElement()){
					elementName = pChild_->Name();
					if(!elementName.compare("Near") || !elementName.compare("near")){
						double r = pChild_->DoubleAttribute("r", 0.0);
						double g = pChild_->DoubleAttribute("g", 0.0);
						double b = pChild_->DoubleAttribute("b", 0.0);
						near = std::shared_ptr<Material>(new Material(Color(r,g,b)));
					}else if(!elementName.compare("Far") || !elementName.compare("far")){
						double r = pChild_->DoubleAttribute("r", 0.0);
						double g = pChild_->DoubleAttribute("g", 0.0);
						double b = pChild_->DoubleAttribute("b", 0.0);
						far = std::shared_ptr<Material>(new Material(Color(r,g,b)));
					}
				}

				integrator = std::shared_ptr<DepthMapIntegrator>(new DepthMapIntegrator(far, near));
			}else if(!type.compare("flat") || !type.compare("normal")){
				for(XMLElement * pChild_ = pChild->FirstChildElement(); pChild_ != NULL; pChild_ = pChild_->NextSiblingElement()){
					elementName = pChild_->Name();
					if(!elementName.compare("Spp") || !elementName.compare("spp")){
						spp = pChild->IntAttribute("value", 0);
					}
				}

				if(!type.compare("flat")){
					integrator = std::shared_ptr<FlatIntegrator>(new FlatIntegrator());
				}else{
					integrator = std::shared_ptr<NormalMapIntegrator>(new NormalMapIntegrator());
				}

			}else if(!type.compare("blinn")){
				for(XMLElement * pChild_ = pChild->FirstChildElement(); pChild_ != NULL; pChild_ = pChild_->NextSiblingElement()){
					elementName = pChild_->Name();
					if(!elementName.compare("Spp") || !elementName.compare("spp")){
						spp = pChild->IntAttribute("value", 0);
					}else if(!elementName.compare("Depth") || !elementName.compare("depth")){
						depth = pChild->IntAttribute("depth", 0);
					}
				}
				integrator = std::shared_ptr<BlinnPhongIntegrator>(new BlinnPhongIntegrator());
			}
		}
	}
}
