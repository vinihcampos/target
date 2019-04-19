#ifndef _POINT3D_
#define _POINT3D_

#include <string>
#include <iostream>
#include <math.h>

namespace target{

	/**
	* Provides the entity point.
	* 
	* @author Vinícius Campos
	* @date 4/25/2019
	*/
	class Point3d{
		private:
			bool comp(double a,double b) const {
			    return std::fabs(a - b) <= 0.009;
			}
		public:
			/**
			*	A point of X-axis.
			*/
			double x;
			/**
			*	A point of Y-axis.
			*/
			double y;

			/**
			*	A point of Z-axis.
			*/
			double z;

			/**
			*	Creates a point. 
			*	@param x_ A point of X-axis.
			*	@param y_ A point of Y-axis.
			*	@param z_ A point of Z-axis.
			*/
			Point3d(double x_ = 0, double y_ = 0, double z_ = 0) : x{x_}, y{y_}, z{z_}{}

			/**
			*	Creates a copy of a point. 
			*	@param copy An existing point.
			*/
			Point3d(const Point3d & copy) : x{copy.x}, y{copy.y}, z{copy.z}{}

			/**
			*	Compare the equality of two points.
			*	@param lhs left point.
			*	@param rhs right point.
			*/
			inline bool operator==(const Point3d& p) const { 
				return comp(x,p.x) && comp(y,p.y) && comp(z,p.z); 
			}
			
			/**
			*	Compare the difference of two points.
			*	@param lhs left point.
			*	@param rhs right point.
			*/
			inline bool operator!=(const Point3d& p){ return !(*this==p); }

			inline double operator[](int idx) const { return idx == 0 ? x : idx == 1 ? y : z; }
			inline double& operator[](int idx) { return idx == 0 ? x : idx == 1 ? y : z; }

			friend inline std::ostream& operator<<(std::ostream &os, const Point3d &v){
				os << v.x << " " << v.y << " " << v.z;
				return os;
 			}
	};
}
#endif