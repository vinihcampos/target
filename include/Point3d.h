#ifndef _POINT3D_
#define _POINT3D_

#include <string>

namespace target{

	/**
	* Provides the entity point.
	* 
	* @author Vinícius Campos
	* @date 4/25/2019
	*/
	class Point3d{
		public:
			/**
			*	A point of X-axis.
			*/
			float x;
			/**
			*	A point of Y-axis.
			*/
			float y;

			/**
			*	A point of Z-axis.
			*/
			float z;

			/**
			*	Creates a point. 
			*	@param x_ A point of X-axis.
			*	@param y_ A point of Y-axis.
			*	@param z_ A point of Z-axis.
			*/
			Point3d(float x_ = 0, float y_ = 0, float z_ = 0) : x{x_}, y{y_}, z{z_}{}

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
			inline bool operator==(const Point3d& p){ return x == p.x && y == p.y && z == p.z; }
			
			/**
			*	Compare the difference of two points.
			*	@param lhs left point.
			*	@param rhs right point.
			*/
			inline bool operator!=(const Point3d& p){ return !(*this==p); }

			inline float operator[](int idx) const { return idx == 0 ? x : idx == 1 ? y : z; }
			inline float& operator[](int idx) { return idx == 0 ? x : idx == 1 ? y : z; }
	};
}
#endif