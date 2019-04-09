#ifndef _DESCRIPTOR_
#define _DESCRIPTOR_

#include "Buffer.h"
#include "Camera.h"
#include "tinyxml2.h"
#include <string>
#include <map>

using namespace tinyxml2;

namespace target{
	class Descriptor{
		private:
			static std::map<std::string, std::string> processSettings(Buffer & buffer, XMLElement *& element);
			static void processBackground(Buffer & buffer, XMLElement *& element);
			static void processCamera(Buffer & buffer, Camera *& camera, XMLElement *& element);

		public:
			static void run(const std::string & description);

	};
}

#endif