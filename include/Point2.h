#ifndef _POINT2_
#define _POINT2_

namespace target{

	/**
	* Provides the entity point.
	* 
	* @author Vinícius Campos
	* @date 4/14/2019
	*/
	class Point2{
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
			*	Creates a point. 
			*	@param x_ A point of X-axis.
			*	@param y_ A point of Y-axis.
			*/
			Point2(double x_ = 0, double y_ = 0) : x{x_}, y{y_}{}

			/**
			*	Creates a copy of a point. 
			*	@param copy An existing point.
			*/
			Point2(const Point2 & copy) : x{copy.x}, y{copy.y}{}

			/**
			*	Compare the equality of two points.
			*	@param lhs left point.
			*	@param rhs right point.
			*/
			friend bool operator==(const Point2& lhs, const Point2& rhs){ return lhs.x == rhs.x && lhs.y == rhs.y; }
			
			/**
			*	Compare the difference of two points.
			*	@param lhs left point.
			*	@param rhs right point.
			*/
			friend bool operator!=(const Point2& lhs, const Point2& rhs){ return !(lhs==rhs); }

			inline double operator[](int idx) const { return idx == 0 ? x : y; }
			inline double& operator[](int idx) { return idx == 0 ? x : y; }

			friend inline std::ostream& operator<<(std::ostream &os, const Point2 &v){
				os << v.x << " " << v.y;
				return os;
 			}
	};
}
#endif