#ifndef _BOUNDS3_
#define _BOUNDS3_

#include "Vec3.h"
#include "Ray.h"
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

			inline const Vec3 &operator[](int i) const{
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

			inline Vec3 &operator[](int i){
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

			inline bool operator==(const Bounds3 & b) const{
				return pMin == b.pMin && pMax == b.pMax;
			}

			inline Vec3 diag() { return pMax - pMin; }

			inline int MaximumExtent(){
				Vec3 d = diag();
				if (d.x() > d.y() && d.x() > d.z()) return 0;
				else if (d.y() > d.z()) return 1;
				else return 2;
			}

			inline bool intersect_p(Ray & ray, const Vec3 &invDir, const int dirIsNeg[3]) const{
				const Bounds3 &bounds = *this;

				//Check for ray intersection against x and y slabs
				double tMin = (bounds[ dirIsNeg[0]].x() - ray.getOrigin().x) * invDir.x();
				double tMax = (bounds[1-dirIsNeg[0]].x() - ray.getOrigin().x) * invDir.x();
				double tyMin = (bounds[ dirIsNeg[1]].y() - ray.getOrigin().y) * invDir.y();
				double tyMax = (bounds[1-dirIsNeg[1]].y() - ray.getOrigin().y) * invDir.y();
				// Update tMax and tyMax to ensure robust bounds intersection
				if (tMin > tyMax || tyMin > tMax) return false;
				if (tyMin > tMin) tMin = tyMin;
				if (tyMax < tMax) tMax = tyMax;

				double tzMin = (bounds[ dirIsNeg[2]].z() - ray.getOrigin().z) * invDir.z();
				double tzMax = (bounds[1-dirIsNeg[2]].z() - ray.getOrigin().z) * invDir.z();
				// Update tMax and tzMax to ensure robust bounds intersection
				if (tMin > tzMax || tzMin > tMax) return false;
				if (tzMin > tMin) tMin = tzMin;
				if (tzMax < tMax) tMax = tzMax;

				//Check for ray intersection against z slab
				return (tMin < ray.tMax) && (tMax > 0);
			}
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