#ifndef _DESCRIPTOR_
#define _DESCRIPTOR_

#include "Buffer.h"
#include "Camera.h"
#include "tinyxml2.h"
#include "Primitive.h"
#include "Sphere.h"
#include <string>
#include <map>
#include <vector>

using namespace tinyxml2;

namespace target{
	class Descriptor{
		private:
			static std::map<std::string, std::string> processSettings(Buffer & buffer, XMLElement *& element);
			static void processBackground(Buffer & buffer, XMLElement *& element);
			static void processCamera(Buffer & buffer, Camera *& camera, XMLElement *& element);
			static void processScene(std::vector<Primitive*> & primitives, XMLElement *& element);
			static void processObject(std::vector<Primitive*> & primitives, XMLElement *& element);
			static target::Sphere * processSphere(XMLElement *& element);

		public:
			static void run(const std::string & description);

	};
}

#endif