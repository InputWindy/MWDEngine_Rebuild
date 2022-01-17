#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDMatrix3X3.h"
#include <iostream>
using namespace std;
namespace MWDEngine {
	
	class  MWDMATH_API MWDQuat
	{
	public:
		union
		{
			MWDREAL m[4];
			struct
			{
				MWDREAL x, y, z, w;
			};
		};
		void Print()const {
			cout<< "(" << x << "," << y << "," << z << "," << w << ")" << endl;
		}
		MWDQuat(void) {
			x = 0.0f, y = 0.0f, z = 0.0f, w = 1.0f;
		};
		MWDQuat(MWDREAL _x, MWDREAL _y, MWDREAL _z, MWDREAL _w) {
			x = _x; y = _y; z = _z; w = _w;
		};
		void Set(MWDREAL _x, MWDREAL _y, MWDREAL _z, MWDREAL _w) {
			x = _x; y = _y; z = _z; w = _w;
		};
		void Set(const MWDQuat& q) {
			x = q.x; y = q.y; z = q.z; w = q.w;
		};
		
		void  Normalize() {
			MWDREAL m = SQRT(x * x + y * y + z * z + w * w);
			MWDREAL Invm = 1.0f / m;
			x *= Invm; y *= Invm; z *= Invm; w *= Invm;
		};
		MWDREAL Dot(const MWDQuat& q)const {
			return (
				x * q.x + y * q.y + z * q.z + w * q.w
				);
		}
		//取共轭
		MWDQuat  GetConjugate()const {
			return MWDQuat(-x, -y, -z, w);
		};
		//获取变换矩阵
		MWDMatrix3X3 GetMatrix()const {
			MWDMatrix3X3 Mat ;
			MWDREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
			x2 = x + x;
			y2 = y + y;
			z2 = z + z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			Mat._00 = 1.0f - (yy + zz);
			Mat._01 = xy + wz;
			Mat._02 = xz - wy;

			Mat._10 = xy - wz;
			Mat._11 = 1.0f - (xx + zz);
			Mat._12 = yz + wx;

			Mat._20 = xz + wy;
			Mat._21 = yz - wx;
			Mat._22 = 1.0f - (xx + yy);
			return Mat;
		};
		//获取欧拉数
		void  GetEulers(MWDREAL& fRoll, MWDREAL& fPitch, MWDREAL& fYaw)const {
			MWDREAL wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;
			x2 = x + x;
			y2 = y + y;
			z2 = z + z;

			xx = x * x2;
			xy = x * y2;
			xz = x * z2;

			yy = y * y2;
			yz = y * z2;
			zz = z * z2;

			wx = w * x2;
			wy = w * y2;
			wz = w * z2;

			MWDREAL _00 = 1.0f - (yy + zz);//
			MWDREAL _01 = xy + wz;//


			MWDREAL _10 = xy - wz;//
			MWDREAL _11 = 1.0f - (xx + zz);//


			MWDREAL _20 = xz + wy;//
			MWDREAL _21 = yz - wx;//
			MWDREAL _22 = 1.0f - (xx + yy);//

			if (_21 > 1.0f)
				fPitch = -MWDPI2;
			else if (_21 < -1.0f)
				fPitch = MWDPI2;
			else
				fPitch = ASIN(-_21);

			if (fabs(_21) > MWDREAL_1)
			{
				fYaw = 0;
				fRoll = ATAN2(-_10, _00);

			}
			else
			{
				fRoll = ATAN2(_01, _11);
				fYaw = ATAN2(_20, _22);

			}
		};
		//读长度
		MWDREAL GetLength()const {
			return SQRT(x * x + y * y + z * z + w * w);
		};
		//取逆
		MWDQuat GetInverse()const {
			return (GetConjugate() / GetLength());
		};
		//获取轴和角度
		void GetAxisAngle(MWDVector3& Axis, MWDREAL& fAngle)const {
			MWDQuat QTemp =*this;
			MWDREAL fHalfAngle = ACOS(QTemp.w);

			fAngle = fHalfAngle * 2.0f;

			MWDREAL SinHalfAngle = 1.0f - QTemp.w * QTemp.w;

			MWDREAL InMWDinHalfAngle = 1.0f / SinHalfAngle;
			Axis.x = QTemp.x * InMWDinHalfAngle;
			Axis.y = QTemp.y * InMWDinHalfAngle;
			Axis.z = QTemp.z * InMWDinHalfAngle;
		};

		void    operator /= (MWDREAL f) {
			w /= f;  x /= f;  y /= f;  z /= f;
		};
		void    operator *= (MWDREAL f) {
			w *= f;  x *= f;  y *= f;  z *= f;
		};
		void    operator *= (const MWDQuat& q) {
			w = w * q.w - x * q.x - y * q.y - z * q.z;
			x = w * q.x + x * q.w + z * q.y - y * q.z;
			y = w * q.y + y * q.w + x * q.z - z * q.x;
			z = w * q.z + z * q.w + y * q.x - x * q.y;
		};
		void    operator += (const MWDQuat& q) {
			w += q.w;  x += q.x;  y += q.y;  z += q.z;
		};
		void    operator -= (const MWDQuat& q) {
			w -= q.w;  x -= q.x;  y -= q.y;  z -= q.z;
		};
		bool operator ==(const MWDQuat& q)const {
			for (unsigned int i = 0; i < 4; i++)
			{
				if (ABS(m[i] - q.m[i]) > EPSILON_E4)
				{
					return false;
				}
			}
			return true;
		};
		bool operator !=(const MWDQuat& q)const {
			return !(*this==q);
		}
		MWDQuat operator /  (MWDREAL f)const {
			return MWDQuat(x / f, y / f, z / f, w / f);
		};
		MWDQuat operator *  (MWDREAL f)const {
			return MWDQuat(x * f, y * f, z * f, w * f);
		};
		MWDQuat operator *  (const MWDQuat& q) const {
			return MWDQuat(
				w * q.x + x * q.w + z * q.y - y * q.z,
				w * q.y + y * q.w + x * q.z - z * q.x,
				w * q.z + z * q.w + y * q.x - x * q.y,
				w * q.w - x * q.x - y * q.y - z * q.z);
		};
		MWDQuat operator +  (const MWDQuat& q) const {
			return MWDQuat(x + q.x, y + q.y, z + q.z, w + q.w);
		};
		MWDQuat operator -  (const MWDQuat& q) const {
			return MWDQuat(x - q.x, y - q.y, z - q.z, w - q.w);
		};
		
		//修改当前实例
		void Pow(MWDREAL exp) {
			if (ABS(w) > MWDREAL_1) {
				return ;
			}

			MWDREAL alpha = ACOS(w);
			MWDREAL newAlpha = alpha * exp;

			MWDREAL mult = SIN(newAlpha) / SIN(alpha);
			Set(x * mult, y * mult, z * mult, COS(newAlpha));
		};
		//修改当前实例
		void Ln() {
			if (ABS(w) > MWDREAL_1)
				return ;
			MWDREAL alpha = ACOS(w);

			MWDREAL Sin = SIN(alpha);
			MWDREAL temp = alpha / Sin;
			Set(temp * x, temp * y, temp * z, 0.0f);
		};
		//修改当前实例
		void Exp() {
			MWDREAL alpha = GetLength();
			MWDREAL Sin = SIN(alpha);
			if (ABS(Sin) > EPSILON_E4)
			{
				MWDREAL temp = Sin / alpha;
				Set(x * temp, y * temp, z * temp, COS(alpha));
			}
			else
			{
				Set(x, y, z, COS(alpha));
			}
		};
	}; 
}
