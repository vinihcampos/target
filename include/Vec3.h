#ifndef _VEC3_
#define _VEC3_

#include <iostream>
#include <math.h>
#include "Point3.h"

namespace target{

	class Vec3{
		private:
			Point3 point;

			bool comp(double a,double b) const {
			    return std::fabs(a - b) <= 0.009;
			}

		public:
			Vec3(double x = 0, double y = 0, double z = 0){
				point = Point3(x,y,z);
			}
			Vec3(const Point3 & point_){
				point = Point3(point_);
			}

			inline double x() const{ return point.x; }
			inline double y() const{ return point.y; }
			inline double z() const{ return point.z; }
			inline double r() const{ return point.x; }
			inline double g() const{ return point.y; }
			inline double b() const{ return point.z; }
			inline void setX(double x){ point.x = x; }
			inline void setY(double y){ point.y = y; }
			inline void setZ(double z){ point.z = z; }

			inline double operator[](int idx) const { return point[idx]; }
			inline double& operator[](int idx){ return point[idx]; }
			
			inline Vec3 operator+(const Vec3 & p){ return Vec3( point.x+p.x(), point.y+p.y(), point.z+p.z() ); }
			inline void operator+=(const Vec3 & p){ point.x += p.x(); point.y += p.y(); point.z += p.z(); }
			inline Point3 operator+(const Point3 p){ return Point3( point.x+p.x, point.y+p.y, point.z+p.z ); }

			inline Vec3 operator-(const Vec3 & p){ return Vec3( point.x-p.x(), point.y-p.y(), point.z-p.z() ); }
			inline void operator-=(const Vec3 & p){ point.x -= p.x(); point.y -= p.y(); point.z -= p.z(); }
			inline Point3 operator-(const Point3 p){ return Point3( point.x-p.x, point.y-p.y, point.z-p.z ); }

			inline Vec3 operator*(const Vec3 & p){ return Vec3( point.x*p.x(), point.y*p.y(), point.z*p.z() ); }
			inline void operator*=(const Vec3 & p){ point.x *= p.x(); point.y *= p.y(); point.z *= p.z(); }
			inline Vec3 operator*(const double v){ return Vec3( point.x*v, point.y*v, point.z*v ); }
			inline void operator*=(const double v){ point.x *= v; point.y *= v; point.z *= v; }

			inline Vec3 operator/(const Vec3 & p){ return Vec3( point.x/p.x(), point.y/p.y(), point.z/p.z() ); }
			inline void operator/=(const Vec3 & p){ point.x /= p.x(); point.y /= p.y(); point.z /= p.z(); }
			inline Vec3 operator/(const double v){ return Vec3( point.x/v, point.y/v, point.z/v ); }
			inline void operator/=(const double v){ point.x /= v; point.y /= v; point.z /= v; }

			inline bool operator==(const Vec3 & p) const{
				return comp(point.x,p.x()) && comp(point.y,p.y()) && comp(point.z,p.z());
			}

			inline double dot(const Vec3 & p){ return point.x * p.x() + point.y * p.y() + point.z * p.z(); }
			inline Vec3 cross(const Vec3 & p){
				return Vec3( point.y*p.z() - point.z*p.y(),
							 point.z*p.x() - point.x*p.z(),
							 point.x*p.y() - point.y*p.x());
			}

			inline Vec3 norm(){
				double root = length();
				return (*this) / root;
			}

			inline double length() const{
				return std::sqrt( point.x*point.x + point.y*point.y + point.z*point.z );
			}

			inline double square_length() const{
				return point.x*point.x + point.y*point.y + point.z*point.z;
			}

			inline Vec3 clamp(double minv, double maxv){
				for (int i = 0; i < 3; ++i){
					if(point[i] < minv)
						point[i] = minv;
					else if(point[i] > maxv)
						point[i] = maxv;
				}
				return *this;
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