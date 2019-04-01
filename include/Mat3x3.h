#ifndef _MAT3X3_
#define _MAT3X3_

#include "Point3d.h"

namespace target{

	class Mat3x3{
		private:
			Point3d a,b,c;

		public:
			Mat3x3(const Point3d & a_,const Point3d & b_,const Point3d & c_) : a{a_}, b{b_}, c{c_}{}
			Mat3x3(Mat3x3 & mat){
				*this = mat;
			}
			Mat3x3(const float & a1=0.f, const float & a2=0.f, const float & a3=0.f,
				   const float & b1=0.f, const float & b2=0.f, const float & b3=0.f,
				   const float & c1=0.f, const float & c2=0.f, const float & c3=0.f){
				a = Point3d(a1,a2,a3);
				b = Point3d(b1,b2,b3);
				c = Point3d(c1,c2,c3);
			}

			inline void zero(){
				Point3d zero_;
				a = b = c = zero_;
			}
			inline bool isZero(){
				Point3d zero_;
				return a == b && b == c && c == zero_;
			}
			float get(const int& i, const int& j);
			void set(const int& i, const int& j, const float & v);
		
	};

}

#endif