#ifndef _BOUNDS3_
#define _BOUNDS3_

#include "Vec3.h"
#include <limits>

namespace target{

	class Bounds3{
		public:
			double minNum, maxNum;
			Vec3 pMin, pMax;
			
			Bounds3(){
				minNum = std::numeric_limits<double>::lowest();
				maxNum = std::numeric_limits<double>::max();
				pMin = Vec3(maxNum, maxNum, maxNum);
				pMax = Vec3(minNum, minNum, minNum);
			}			
			Bounds3(const Vec3 &p) : pMin(p), pMax(p) { }
			Bounds3(const Vec3 &p1, const Vec3 &p2) : 
				pMin(std::min(p1.x(), p2.x()), std::min(p1.y(), p2.y()), std::min(p1.z(), p2.z())), 
				pMax(std::max(p1.x(), p2.x()), std::max(p1.y(), p2.y()), std::max(p1.z(), p2.z())) {}

			const Vec3 &operator[](int i) const{
				switch(i){
					case 0:
						return pMin;
						break;
					case 1:
						return pMax;
						break;
					default:
						throw "Index out of bounds!";
				}
			}

			Vec3 &operator[](int i){
				switch(i){
					case 0:
						return pMin;
						break;
					case 1:
						return pMax;
						break;
					default:
						throw "Index out of bounds!";
				}
			}

			Vec3 diag() { return pMax - pMin; }
	};

	inline Bounds3 Union(const Bounds3 &b1, const Bounds3 &b2) {
		return Bounds3( Vec3( std::min(b1.pMin.x(),b2.pMin.x()),
								std::min(b1.pMin.y(),b2.pMin.y()),
								std::min(b1.pMin.z(),b2.pMin.z())),
						Vec3(	std::max(b1.pMax.x(),b2.pMax.x()),
								std::max(b1.pMax.y(),b2.pMax.y()),
								std::max(b1.pMax.z(),b2.pMax.z())));
	}
}

#endif