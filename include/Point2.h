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
			int x;
			/**
			*	A point of Y-axis.
			*/
			int y;

			/**
			*	Creates a point. 
			*	@param x_ A point of X-axis.
			*	@param y_ A point of Y-axis.
			*/
			Point2(int x_ = 0, int y_ = 0) : x{x_}, y{y_}{}

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
	};
}
#endif