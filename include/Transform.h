#ifndef _TRANSFORM_
#define _TRANSFORM_

#include "Matrix4x4.h"
#include "Point3.h"
#include "Vec3.h"
#include "Ray.h"
#include "Bounds3.h"
#include "Util.h"
#include <vector>

namespace target{

	class Transform{
		private:
			inline double toRad(const double & theta){
				return theta * PI / 180;
			}
		public:
			Matrix4x4 m, mInv;
			
			Transform() : m{identity()}, mInv{identity()}{}

			Transform(const double mat[4][4]){
				for(int i = 0; i < 4; ++i)
					for (int j = 0; j < 4; ++j)
						m[i][j] = mat[i][j];
				mInv = -m;
			}
			Transform(const Matrix4x4 & m) : m{m}, mInv{-m}{}
			Transform(const Matrix4x4 & m, const Matrix4x4 & mInv) : m{m}, mInv{mInv}{}

			inline Transform operator-(){
				return Transform(mInv, m);
			}

			inline Transform inv(){
				return Transform(mInv, m);	
			}

			inline Transform Rx(const double & theta){
				double theta_rad = toRad(theta);
				double sinT = std::sin(theta_rad);
				double cosT = std::cos(theta_rad);

				Matrix4x4 m(1,   0,    0,0,
							0,cosT,-sinT,0,
							0,sinT, cosT,0,
							0,   0,    0,1);

				return Transform(m, m.T());
			}

			inline Transform Ry(const double & theta){
				double theta_rad = toRad(theta);
				double sinT = std::sin(theta_rad);
				double cosT = std::cos(theta_rad);

				Matrix4x4 m(cosT,0,sinT,0,
							   0,1,   0,0,
						   -sinT,0,cosT,0,
							   0,0,   0,1);

				return Transform(m, m.T());
			}

			inline Transform Rz(const double & theta){
				double theta_rad = toRad(theta);
				double sinT = std::sin(theta_rad);
				double cosT = std::cos(theta_rad);

				Matrix4x4 m(cosT,-sinT,0,0,
							sinT, cosT,0,0,
							   0,    0,0,0,
							   0,    0,0,1);

				return Transform(m, m.T());
			}

			inline Transform R(const double & theta, Vec3 & axis){
				Vec3 a = axis.norm();
				double theta_rad = toRad(theta);
				double sinT = std::sin(theta_rad);
				double cosT = std::cos(theta_rad);

				Matrix4x4 m;

				// 1st basis
				m[0][0] = a.x() * a.x() + (1 - a.x() * a.x()) * cosT;
				m[0][1] = a.x() * a.y() * (1 - cosT) - a.z() * sinT;
				m[0][2] = a.x() * a.z() * (1 - cosT) + a.y() * sinT;
				m[0][3] = 0;

				// 2nd basis
				m[1][0] = a.x() * a.y() * (1 - cosT) + a.z() * sinT;
	            m[1][1] = a.y() * a.y() + (1 - a.y() * a.y()) * cosT;
	            m[1][2] = a.y() * a.z() * (1 - cosT) - a.x() * sinT;
				m[1][3] = 0;

				// 3rd basis
				m[2][0] = a.x() * a.y() * (1 - cosT) - a.y() * sinT;
            	m[2][1] = a.y() * a.y() * (1 - cosT) + a.x() * sinT;
            	m[2][2] = a.z() * a.y() + (1 - a.z() * a.z()) * cosT;
				m[2][3] = 0;

				return Transform(m, m.T());
			}

			/**
			* Transform a Point3
			*/
			inline Point3 point(const Point3 & p) const {
				double x = p.x, y = p.y, z = p.z;

				double xp = m[0][0]*x + m[0][1]*y + m[0][2]*z + m[0][3];
				double yp = m[1][0]*x + m[1][1]*y + m[1][2]*z + m[1][3];
				double zp = m[2][0]*x + m[2][1]*y + m[2][2]*z + m[2][3];
				double wp = m[3][0]*x + m[3][1]*y + m[3][2]*z + m[3][3];
				if (wp == 1) return Point3(xp, yp, zp);
				else return Point3(xp / wp, yp / wp, zp / wp);
			}

			/**
			* Transform a Vec3
			*/			
			inline Vec3 vec(const Vec3 &v) const {
				double x = v.x(),y = v.y(), z = v.z();

				return Vec3(m[0][0]*x + m[0][1]*y + m[0][2]*z,
						 	m[1][0]*x + m[1][1]*y + m[1][2]*z,
						 	m[2][0]*x + m[2][1]*y + m[2][2]*z);
			}

			/**
			* Transform a Normal
			*/			
			inline Vec3 normal(const Vec3 &n) const {
				double x = n.x(),y = n.y(), z = n.z();

				return Vec3(mInv[0][0]*x + mInv[1][0]*y + mInv[2][0]*z,
							mInv[0][1]*x + mInv[1][1]*y + mInv[2][1]*z,
							mInv[0][2]*x + mInv[1][2]*y + mInv[2][2]*z);
			}

			/**
			* Transform a Bounds3
			*/			
			inline Bounds3 bounds(const Bounds3 &b) const {
				const Transform &M = *this;
				Bounds3 ret( Vec3( M.point( Point3(b.pMin.x(), b.pMin.y(), b.pMin.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMax.x(), b.pMin.y(), b.pMin.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMin.x(), b.pMax.y(), b.pMin.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMin.x(), b.pMin.y(), b.pMax.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMin.x(), b.pMax.y(), b.pMax.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMax.x(), b.pMax.y(), b.pMin.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMax.x(), b.pMin.y(), b.pMax.z() ) ) ) );
				ret = Union(ret, Vec3( M.point(Point3(b.pMax.x(), b.pMax.y(), b.pMax.z() ) ) ) );
				return ret;
			}

			/**
			* Transform a Ray
			*/
			inline Ray ray(const Ray &r) const {
				Point3 o = point(r.getOrigin());
				Vec3 d = vec(r.getDirection());
				Ray new_ray(o, d);
				new_ray.tMax = r.tMax;
				return new_ray;
			}

			inline Transform operator*(Transform & t){
				return Transform(m * t.m, t.mInv * mInv);
			}

			bool SwapsHandedness() const {
				double det = m[0][0] * (m[1][1] * m[2][2] - m[1][2] * m[2][1]) -
							 m[0][1] * (m[1][0] * m[2][2] - m[1][2] * m[2][0]) +
							 m[0][2] * (m[1][0] * m[2][1] - m[1][1] * m[2][0]);
				return det < 0;
			}

			friend inline std::shared_ptr<Transform> operator*(const std::shared_ptr<Transform> & t1, const std::shared_ptr<Transform> & t2){
				return std::shared_ptr<Transform>(new Transform(t1->m * t2->m, t2->mInv * t1->mInv));
			}

			friend inline std::shared_ptr<Transform> Compose(const std::vector<std::shared_ptr<Transform>> & transforms){
				std::shared_ptr<Transform> t = std::shared_ptr<Transform>(new Transform());
				for(std::shared_ptr<Transform> tr : transforms){
					t = tr * t;
				}
				return t;
			}
	};

	inline Transform operator-(const Transform &t){
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

	inline Transform T(const double & x, const double & y, const double & z) {
		Vec3 delta(x,y,z);
		
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

}

#endif