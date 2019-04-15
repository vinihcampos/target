#ifndef _VEC3_
#define _VEC3_

#include <iostream>
#include <math.h>
#include "Point3d.h"

namespace target{

	class Vec3{
		private:
			Point3d point;

		public:
			Vec3(float x = 0, float y = 0, float z = 0){
				point = Point3d(x,y,z);
			}
			Vec3(const Point3d & point_){
				point = Point3d(point_);
			}

			inline float x() const{ return point.x; }
			inline float y() const{ return point.y; }
			inline float z() const{ return point.z; }
			inline float r() const{ return point.x; }
			inline float g() const{ return point.y; }
			inline float b() const{ return point.z; }

			inline float operator[](int idx) const { return point[idx]; }
			inline float& operator[](int idx){ return point[idx]; }
			
			inline Vec3 operator+(const Vec3 & p){ return Vec3( point.x+p.x(), point.y+p.y(), point.z+p.z() ); }
			inline void operator+=(const Vec3 & p){ point.x += p.x(); point.y += p.y(); point.z += p.z(); }
			inline Point3d operator+(const Point3d p){ return Point3d( point.x+p.x, point.y+p.y, point.z+p.z ); }

			inline Vec3 operator-(const Vec3 & p){ return Vec3( point.x-p.x(), point.y-p.y(), point.z-p.z() ); }
			inline void operator-=(const Vec3 & p){ point.x -= p.x(); point.y -= p.y(); point.z -= p.z(); }
			inline Point3d operator-(const Point3d p){ return Point3d( point.x-p.x, point.y-p.y, point.z-p.z ); }

			inline Vec3 operator*(const Vec3 & p){ return Vec3( point.x*p.x(), point.y*p.y(), point.z*p.z() ); }
			inline void operator*=(const Vec3 & p){ point.x *= p.x(); point.y *= p.y(); point.z *= p.z(); }
			inline Vec3 operator*(const float v){ return Vec3( point.x*v, point.y*v, point.z*v ); }
			inline void operator*=(const float v){ point.x *= v; point.y *= v; point.z *= v; }

			inline Vec3 operator/(const Vec3 & p){ return Vec3( point.x/p.x(), point.y/p.y(), point.z/p.z() ); }
			inline void operator/=(const Vec3 & p){ point.x /= p.x(); point.y /= p.y(); point.z /= p.z(); }
			inline Vec3 operator/(const float v){ return Vec3( point.x/v, point.y/v, point.z/v ); }
			inline void operator/=(const float v){ point.x /= v; point.y /= v; point.z /= v; }

			inline bool operator==(const Vec3 & p) const{
				return point.x == p.x() && point.y == p.y() && point.z == p.z();
			}

			inline float dot(const Vec3 & p){ return point.x * p.x() + point.y * p.y() + point.z * p.z(); }
			inline Vec3 cross(const Vec3 & p){
				return Vec3( point.y*p.z() - point.z*p.y(),
							 point.z*p.x() - point.x*p.z(),
							 point.x*p.y() - point.y*p.x());
			}

			inline Vec3 norm(){
				float root = length();
				return (*this) / root;
			}

			inline float length() const{
				return std::sqrt( point.x*point.x + point.y*point.y + point.z*point.z );
			}

			inline float square_length() const{
				return point.x*point.x + point.y*point.y + point.z*point.z;
			}

			friend inline std::ostream& operator<<(std::ostream &os, const Vec3 &v){
				os << v.x() << " " << v.y() << " " << v.z();
				return os;
 			}
		};

	typedef Vec3 Color;

	/* Predefined colors*/
	static const Color BLACK;
	static const Color WHITE = target::Color(255,255,255);
	static const Color RED = target::Color(255,0,0);
	static const Color GREEN = target::Color(0,255,0);
	static const Color BLUE = target::Color(0,0,255);
	static const Color YELLOW = target::Color(255,255,0);
}

#endif