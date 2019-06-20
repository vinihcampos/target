#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "Matrix4x4.h"

namespace target{

	class Transform{
		private:
			Matrix4x4 m, mInv;
		public:
			Transform(const double mat[4][4]){
				for(int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						m[i][j] = mat[i][j];
			}
			Transform(const Matrix4x4 & m) : m{m}, mInv{-m}{}
			Transform(const Matrix4x4 & m, const Matrix4x4 & mInv) : m{m}, mInv{mInv}{}

			inline Transform operator-(){
				return Transform(mInv, m);
			}

			friend inline Transform operator-(const Transform &t){
				return -t;
			}

			inline Transform T(const Vec3 &delta) {
				Matrix4x4 m(1, 0, 0, delta.x(),
							0, 1, 0, delta.y(),
							0, 0, 1, delta.z(),
							0, 0, 0, 1);
				Matrix4x4 minv(1, 0, 0, -delta.x(),
								0, 1, 0, -delta.y(),
								0, 0, 1, -delta.z(),
								0, 0, 0, 1);
				
				return Transform(m, minv);
			}

			inline Transform S(const double & x, const double & y, const double & z){
				Matrix4x4 m(x, 0, 0, 0,
							0, y, 0, 0,
							0, 0, z, 0,
							0, 0, 0, 1);

				Matrix4x4 minv(1.0/x, 0, 0, 0,
							   0, 1.0/y, 0, 0, 
							   0, 0, 1.0/z, 0,
							   0, 0,   0,  1);
				return Transform(m, minv);
			}

			inline Transform S(const Vec3 & delta){
				return S(delta.x(), delta.y(), delta.z());
			}
		
	};

}

#endif