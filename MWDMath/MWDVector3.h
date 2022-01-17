#pragma once
#include "MWDMath.h"
#include <iostream>
using namespace std;
namespace MWDEngine {
	class MWDMatrix3X3;
	class MWDMatrix3X3W;
	class MWDQuat;
	class  MWDMATH_API MWDVector3
	{
	public:
		union
		{
			MWDREAL m[3];
			struct
			{
				MWDREAL x, y, z;
			};
		};
		MWDVector3(void) {
			x = 0; y = 0; z = 0;
		};
		MWDVector3(MWDREAL _x, MWDREAL _y, MWDREAL _z) {
			x = _x; y = _y; z = _z;
		};

		FORCEINLINE void  Set(MWDREAL _x, MWDREAL _y, MWDREAL _z) { x = _x; y = _y; z = _z; };
		FORCEINLINE void  Set(const MWDVector3& v) { x = v.x; y = v.y; z = v.z; };
		FORCEINLINE MWDREAL GetLength(void)const {
			return SQRT(x * x + y * y + z * z);
		};
		FORCEINLINE MWDREAL GetSqrLength(void) const {
			return (x * x + y * y + z * z);
		};
		FORCEINLINE MWDVector3 GetV3() {
			MWDVector3 out;
			out.x = x;
			out.y = y;
			out.z = z;
			return out;
		};
		FORCEINLINE void  Negate(void) { x = -x;  y = -y;  z = -z; };
		FORCEINLINE void  Normalize(void) {
			MWDREAL f = (MWDREAL)SQRT(x * x + y * y + z * z);
			if (f != 0.0f)
			{
				x /= f; y /= f; z /= f;
			}
		};
		MWDREAL Dot( const MWDVector3& v2) {
			return (x * v2.x + y * v2.y + z * v2.z);
		};
		MWDVector3 Cross( const MWDVector3& v2) {
			return MWDVector3(
				y * v2.z - z * v2.y,
				z * v2.x - x * v2.z,
				x * v2.y - y * v2.x
			);
		};
		MWDVector3 Cross(const MWDVector3& v1, const MWDVector3& v2) {
			return MWDVector3(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
			);
		};
		void  Create(const MWDVector3& v1,const MWDVector3& v2) {
			x = v2.x - v1.x;
			y = v2.y - v1.y;
			z = v2.z - v1.z;
		};
		MWDVector3& operator += (const MWDVector3& v) {
			x += v.x;   y += v.y;   z += v.z;
			return *this;
		};
		MWDVector3& operator -= (const MWDVector3& v) {
			x -= v.x;   y -= v.y;   z -= v.z;
			return *this;
		};
		MWDVector3& operator *= (MWDREAL f) {
			x *= f;   y *= f;   z *= f;
			return *this;
		};
		MWDVector3& operator /= (MWDREAL f) {
			x /= f;   y /= f;   z /= f;
			return *this;
		};
		MWDVector3& operator += (MWDREAL f) {
			x += f;
			y += f;
			z += f;
			return *this;
		};
		MWDVector3& operator -= (MWDREAL f) {
			x -= f;
			y -= f;
			z -= f;
			return *this;
		};
		MWDVector3& operator /= (const MWDVector3& v) {
			Set(x / v.x, y / v.y, z / v.z);
			return *this;
		};
		MWDVector3& operator *= (const MWDVector3& v) {
			Set(x * v.x, y * v.y, z * v.z);
			return *this;
		};

		bool operator ==(const MWDVector3& v)const {
			for (unsigned int i = 0; i < 3; i++)
			{
				if (ABS(m[i] - v.m[i]) > EPSILON_E4)
				{
					return false;
				}
			}
			return true;
		};
		bool operator !=(const MWDVector3& v)const {
			return !(*this == v);
		}

		//加减乘除都会new一个新的Vector3
		MWDVector3 operator * (MWDREAL f)const {
			return MWDVector3(x * f, y * f, z * f);
		};
		MWDVector3 operator / (MWDREAL f)const {
			return MWDVector3(x / f, y / f, z / f);
		};
		MWDVector3 operator + (MWDREAL f)const {
			return MWDVector3(x +f, y + f, z + f);
		};
		MWDVector3 operator - (MWDREAL f)const {
			return MWDVector3(x - f, y - f, z - f);
		};
		
		MWDVector3 operator + (const MWDVector3& v)const {
			return MWDVector3(x + v.x, y + v.y, z + v.z);
		};
		MWDVector3 operator - (const MWDVector3& v)const {
			return MWDVector3(x - v.x, y - v.y, z - v.z);
		};
		MWDVector3 operator / (const MWDVector3& v)const {
			return MWDVector3(x / v.x, y / v.y, z / v.z);
		};
		MWDVector3 operator * (const MWDVector3& v)const {
			return MWDVector3(x * v.x, y * v.y, z * v.z);
		};
		
		void Print()const {
			cout<<"(" << x << "," << y << "," << z <<")" << endl;
		}

		static MWDVector3 Up() {
			return MWDVector3(0, 1, 0);
		}
		static MWDVector3 Right() {
			return MWDVector3(1,0,0);
		}
		static MWDVector3 Forward() {
			return MWDVector3(0,0,1);
		}
		static MWDVector3 Zero() {
			return MWDVector3(0, 0, 0);
		}
	};

}

