#pragma once
#include "MWDMath.h"
namespace MWDEngine {
	class MWDMATH_API MWDVector2
	{
	public:
		union
		{
			MWDREAL m[2];
			struct
			{
				MWDREAL x, y;
			};
		};
		MWDVector2(void) {
			x = 0; y = 0;
		};
		MWDVector2(MWDREAL _x, MWDREAL _y) {
			x = _x; y = _y;
		};
		FORCEINLINE void  Set(MWDREAL _x, MWDREAL _y) { x = _x; y = _y; };
		FORCEINLINE void  Set(const MWDVector2 v) { x = v.x; y = v.y; };
		void operator += (const MWDVector2& v) { x += v.x;   y += v.y; };
		void operator -= (const MWDVector2& v) { x -= v.x;   y -= v.y; };
		void operator *= (MWDREAL f) { x *= f;   y *= f; };
		void operator /= (MWDREAL f) { x /= f;   y /= f; };
		void operator += (MWDREAL f) { x += f;   y += f; };
		void operator -= (MWDREAL f) { x -= f;   y -= f; };
		MWDREAL     operator * (const MWDVector2& v)const { return (v.x * x + v.y * y); };
		bool operator ==(const MWDVector2& v)const {
			for (unsigned int i = 0; i < 2; i++)
			{
				if (ABS(m[i] - v.m[i]) > EPSILON_E4)
				{
					return false;
				}
			}
			return true;
		};
		bool operator !=(const MWDVector2& v)const {
			return !(*this==v);
		};
		MWDVector2 operator * (MWDREAL f)const { return MWDVector2(x * f, y * f); };
		MWDVector2 operator / (MWDREAL f)const { return MWDVector2(x / f, y / f); };
		MWDVector2 operator + (MWDREAL f)const { return MWDVector2(x + f, y + f); };
		MWDVector2 operator - (MWDREAL f)const { return MWDVector2(x - f, y - f); };
		MWDVector2 operator + (const MWDVector2& v)const { return MWDVector2(x + v.x, y + v.y); };
		MWDVector2 operator - (const MWDVector2& v)const { return MWDVector2(x - v.x, y - v.y); };
		void Normalize() {
			MWDREAL f = (MWDREAL)SQRT(x * x + y * y);

			if (f != 0.0f)
			{
				x /= f; y /= f;
			}
		};
	};
	
}