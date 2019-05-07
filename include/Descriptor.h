#ifndef _DESCRIPTOR_
#define _DESCRIPTOR_

#include "Buffer.h"
#include "Camera.h"
#include "tinyxml2.h"
#include "Primitive.h"
#include "Sphere.h"
#include "Integrator.h"
#include "Camera.h"
#include "Scene.h"
#include "Material.h"
#include <string>
#include <map>
#include <vector>
#include <memory>

using namespace tinyxml2;

namespace target{
	class Descriptor{
		private:
			static std::map<std::string, std::string> processSettings(std::shared_ptr<Buffer> & buffer, XMLElement *& element);
			static void processBackground(std::shared_ptr<Buffer> & buffer, XMLElement *& element);
			static void processCamera(std::shared_ptr<Buffer> & buffer, std::shared_ptr<Camera> & camera, XMLElement *& element);
			static void processScene(std::vector<std::shared_ptr<Primitive>> & primitives, std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element);
			static void processObject(std::vector<std::shared_ptr<Primitive>> & primitives, std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element);
			static void processMaterials(std::map<std::string, std::shared_ptr<Material>> & materials, XMLElement *& element);
			static Color processFlatMaterial(XMLElement *& element);
			static std::shared_ptr<target::Sphere> processSphere(XMLElement *& element);

		public:
			static void run(const std::string & description, 
				std::shared_ptr<Integrator> & mIntegraor, 
				std::shared_ptr<Camera> & mCamera, 
				std::shared_ptr<Scene> & mScene);

	};
}

#endif