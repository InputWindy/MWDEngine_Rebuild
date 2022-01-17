#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
namespace MWDEngine
{
	DWORD MWDDWCOLORARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
	DWORD MWDDWCOLORBGRA(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
	DWORD MWDDWCOLORRGBA(unsigned char a, unsigned char r, unsigned char g, unsigned char b);
	DWORD MWDDWCOLORABGR(unsigned char a, unsigned char r, unsigned char g, unsigned char b);

	void MWDDWCOLORGetARGB(DWORD ARGB, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b);
	void MWDDWCOLORGetBGRA(DWORD BGRA, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b);
	void MWDDWCOLORGetRGBA(DWORD RGBA, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b);
	void MWDDWCOLORGetABGR(DWORD ABGR, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b);
	//可以标识rgba也可以表示xyzw
	class  MWDMATH_API MWDVector3W
	{
	public:
		union
		{
			MWDREAL m[4];
			struct
			{
				MWDREAL x, y, z, w;
			};
			struct
			{
				MWDREAL r, g, b, a;
			};
		};
		void Print()const {
			cout << "(" << x << "," << y << "," << z << "," << w << ")" << endl;
		}
		MWDVector3W(void) {
			x = 0, y = 0, z = 0, w = 1.0f;
		};
		MWDVector3W(MWDREAL _x, MWDREAL _y, MWDREAL _z, MWDREAL _w = 1.0f) {
			x = _x; y = _y; z = _z; w = _w;
		};
		MWDVector3W(const MWDVector3& V) {
			Set(V);
		};
		MWDVector3W(unsigned char R, unsigned char G, unsigned char B, unsigned char A = 255) {
			r = (R * (MWDREAL)1.0) / 255;
			g = (G * (MWDREAL)1.0) / 255;
			b = (B * (MWDREAL)1.0) / 255;
			a = (A * (MWDREAL)1.0) / 255;
		};
		
		FORCEINLINE void Set(const MWDVector3& V) {
			x = V.x;
			y = V.y;
			z = V.z;
			w = 1.0f;
		};
		FORCEINLINE void Set(const MWDVector3W& V) {
			x = V.x;
			y = V.y;
			z = V.z;
			w = V.w;
		};
		FORCEINLINE void Set(MWDREAL _x, MWDREAL _y, MWDREAL _z, MWDREAL _w = 1.0f) {
			x = _x; y = _y; z = _z; w = _w;
		};

		void operator += (const MWDVector3W& v) {
			x += v.x;   y += v.y;   z += v.z; w += v.w;
		};
		void operator -= (const MWDVector3W& v) {
			x -= v.x;   y -= v.y;   z -= v.z; w -= v.w;
		};
		void operator *= (const MWDVector3W& v) {
			x *= v.x;   y *= v.y;   z *= v.z; w *= v.w;
		};
		void operator /= (const MWDVector3W& v) {
			x /= v.x;   y /= v.y;   z /= v.z; w /= v.w;
		};

		void operator *= (MWDREAL f) {
			x *= f;   y *= f;   z *= f; w *= f;
		};
		void operator /= (MWDREAL f) {
			x /= f;   y /= f;   z /= f; w /= f;
		};
		void operator += (MWDREAL f) {
			x += f; y += f; z += f;
		};
		void operator -= (MWDREAL f) {
			x -= f; y -= f; z -= f;
		};

		bool operator ==(const MWDVector3W& v)const {
			for (unsigned int i = 0; i < 4; i++)
			{
				if (ABS(m[i] - v.m[i]) > EPSILON_E4)
				{
					return false;
				}
			}
			return true;
		};
		bool operator !=(const MWDVector3W& v)const {
			return !(*this == v);
		}

		//加减乘除都会new一个新的MWDVector3W
		MWDVector3W operator * (MWDREAL f)const {
			return MWDVector3W(x * f, y * f, z * f, w * f);
		};
		MWDVector3W operator / (MWDREAL f)const {
			return MWDVector3W(x / f, y / f, z / f, w / f);
		};
		MWDVector3W operator + (MWDREAL f)const {
			return MWDVector3W(x + f, y + f, z + f, w + f);
		};
		MWDVector3W operator - (MWDREAL f)const {
			return MWDVector3W(x - f, y - f, z - f, w - f);
		};

		MWDVector3W operator + (const MWDVector3W& v)const {
			return MWDVector3W(x + v.x, y + v.y, z + v.z, w + v.w);
		};
		MWDVector3W operator - (const MWDVector3W& v)const {
			return MWDVector3W(x - v.x, y - v.y, z - v.z, w - v.w);
		};
		MWDVector3W operator * (const MWDVector3W& v)const {
			return MWDVector3W(x * v.x, y * v.y, z * v.z, w * v.w);
		};
		MWDVector3W operator / (const MWDVector3W& v)const {
			return MWDVector3W(x - v.x, y - v.y, z - v.z, w - v.w);
		};

		bool operator >(const MWDVector3W& v)const {
			if (x > v.x && y > v.y && z > v.z && w > v.w) {
				return true;
			}
			return false;
		}
		bool operator >=(const MWDVector3W& v)const {
			if (x >= v.x && y >= v.y && z >= v.z && w >= v.w) {
				return true;
			}
			return false;
		}
		bool operator <(const MWDVector3W& v)const {
			return !(*this >= v);
		}
		bool operator <=(const MWDVector3W& v)const {
			return !(*this > v);
		}

		DWORD GetDWARGB()const {
			unsigned char R, G, B, A;
			GetUCColor(A, R, G, B);
			return (DWORD)(((((A) & 0xff) << 24) | (((R) & 0xff) << 16) | (((G) & 0xff) << 8) | ((B) & 0xff)));
		};
		DWORD GetDWRGBA()const {
			unsigned char R, G, B, A;
			GetUCColor(A, R, G, B);

			return (DWORD)(((((R) & 0xff) << 24) | (((G) & 0xff) << 16) | (((B) & 0xff) << 8) | ((A) & 0xff)));

		};

		DWORD GetDWBGRA()const {
			unsigned char R, G, B, A;
			GetUCColor(A, R, G, B);
			return ((DWORD)((((B) & 0xff) << 24) | (((G) & 0xff) << 16) | (((R) & 0xff) << 8) | ((A) & 0xff)));

		};
		DWORD GetDWABGR()const {
			unsigned char R, G, B, A;
			GetUCColor(A, R, G, B);
			return ((DWORD)((((A) & 0xff) << 24) | (((B) & 0xff) << 16) | (((G) & 0xff) << 8) | ((R) & 0xff)));

		};

		void GetUCColor(unsigned char& R, unsigned char& G, unsigned char& B, unsigned char& A)const {
			A = (unsigned char)(a * 255);
			R = (unsigned char)(r * 255);
			G = (unsigned char)(g * 255);
			B = (unsigned char)(b * 255);
		};

		void CreateFromARGB(DWORD ARGB) {
			unsigned char a, r, g, b;
			MWDDWCOLORGetARGB(ARGB, a, r, g, b);
			Set(r, g, b, a);
		};
		void CreateFromBGRA(DWORD BGRA) {
			unsigned char a, r, g, b;
			MWDDWCOLORGetBGRA(BGRA, a, r, g, b);
			Set(r, g, b, a);
		};
		void CreateFromRGBA(DWORD RGBA) {
			unsigned char a, r, g, b;
			MWDDWCOLORGetRGBA(RGBA, a, r, g, b);
			Set(r, g, b, a);
		};
		void CreateFormABGR(DWORD ABGR) {
			unsigned char a, r, g, b;
			MWDDWCOLORGetABGR(ABGR, a, r, g, b);
			Set(r, g, b, a);
		};

	}; 

	//四维向量可以标识rgba的颜色
	typedef class MWDVector3W MWDColorRGBA;
	FORCEINLINE  DWORD MWDDWCOLORARGB(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		return ((DWORD)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)));
	}
	FORCEINLINE  DWORD MWDDWCOLORBGRA(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		return ((DWORD)((((b) & 0xff) << 24) | (((g) & 0xff) << 16) | (((r) & 0xff) << 8) | ((a) & 0xff)));
	}
	FORCEINLINE  DWORD MWDDWCOLORRGBA(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		return ((DWORD)((((r) & 0xff) << 24) | (((g) & 0xff) << 16) | (((b) & 0xff) << 8) | ((a) & 0xff)));
	}
	FORCEINLINE  DWORD MWDDWCOLORABGR(unsigned char a, unsigned char r, unsigned char g, unsigned char b)
	{
		return ((DWORD)((((a) & 0xff) << 24) | (((b) & 0xff) << 16) | (((g) & 0xff) << 8) | ((r) & 0xff)));
	}

	FORCEINLINE  void MWDDWCOLORGetARGB(DWORD ARGB, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		a = (ARGB >> 24) & 0xff;
		r = (ARGB >> 16) & 0xff;
		g = (ARGB >> 8) & 0xff;
		b = (ARGB) & 0xff;
	}
	FORCEINLINE  void MWDDWCOLORGetBGRA(DWORD BGRA, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		b = (BGRA >> 24) & 0xff;
		g = (BGRA >> 16) & 0xff;
		r = (BGRA >> 8) & 0xff;
		a = (BGRA) & 0xff;
	}
	FORCEINLINE  void MWDDWCOLORGetRGBA(DWORD RGBA, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		r = (RGBA >> 24) & 0xff;
		g = (RGBA >> 16) & 0xff;
		b = (RGBA >> 8) & 0xff;
		a = (RGBA) & 0xff;
	}
	FORCEINLINE  void MWDDWCOLORGetABGR(DWORD ABGR, unsigned char& a, unsigned char& r, unsigned char& g, unsigned char& b)
	{
		a = (ABGR >> 24) & 0xff;
		b = (ABGR >> 16) & 0xff;
		g = (ABGR >> 8) & 0xff;
		r = (ABGR) & 0xff;
	}
}