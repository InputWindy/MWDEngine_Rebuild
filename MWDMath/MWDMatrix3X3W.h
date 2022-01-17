#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDVector3W.h"
#include "MWDMatrix3X3.h"
namespace MWDEngine {
	class  MWDMATH_API MWDMatrix3X3W
	{
	public:
		union
		{
			MWDREAL m[16];
			struct
			{
				MWDREAL _00, _01, _02, _03;
				MWDREAL _10, _11, _12, _13;
				MWDREAL _20, _21, _22, _23;
				MWDREAL _30, _31, _32, _33;
			};
			MWDREAL M[4][4];
		};
		void Print()const {
			cout << endl;
			cout << _00 << "," << _01 << "," << _02 << "," << _03 << endl;
			cout << _10 << "," << _11 << "," << _12 << "," << _13 << endl;
			cout << _20 << "," << _21 << "," << _22 << "," << _23 << endl;
			cout << _30 << "," << _31 << "," << _32 << "," << _33 << endl;
			cout << endl;
		}
		MWDMatrix3X3W(void) {
			Identity();
		};
		MWDMatrix3X3W(MWDREAL m00, MWDREAL m01, MWDREAL m02, MWDREAL m03,MWDREAL m10, MWDREAL m11, MWDREAL m12, MWDREAL m13,MWDREAL m20, MWDREAL m21, MWDREAL m22, MWDREAL m23,MWDREAL m30, MWDREAL m31, MWDREAL m32, MWDREAL m33) {
			_00 = m00; _01 = m01; _02 = m02; _03 = m03;
			_10 = m10; _11 = m11; _12 = m12; _13 = m13;
			_20 = m20; _21 = m21; _22 = m22; _23 = m23;
			_30 = m30; _31 = m31; _32 = m32; _33 = m33;
		};
		void CreateFrom3X3(const MWDMatrix3X3& Mat) {
			Identity();
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					M[i][j] = Mat.M[i][j];
				}

			}
		};
		void CreateTranslate(MWDREAL dx, MWDREAL dy, MWDREAL dz) {
			Identity();
			_30 = dx;
			_31 = dy;
			_32 = dz;
		};
		void CreateTranslate(const MWDVector3& V) {
			Identity();
			_30 = V.x;
			_31 = V.y;
			_32 = V.z;
		};
		void CreateInWorldObject(const MWDVector3& U, const MWDVector3& V, const MWDVector3& N, const MWDVector3& Point) {
			MWDVector3 u = U, v = V, n = N,p = Point;
			_00 = U.x;
			_10 = U.y;
			_20 = U.z;

			_01 = V.x;
			_11 = V.y;
			_21 = V.z;

			_02 = N.x;
			_12 = N.y;
			_22 = N.z;

			_03 = 0.0f; _13 = 0.0f; _23 = 0.0f; _33 = 1.0f;

			_30 = -(u.Dot(p));
			_31 = -(v.Dot(p));
			_32 = -(n.Dot(p));
		};
		void CreateFormBillboard(const MWDVector3& vcPos,const MWDMatrix3X3& CameraRotMatrix,bool bAxisY) {
			MWDMatrix3X3 Mat;
			Mat.InverseOf(CameraRotMatrix);
			CreateFrom3X3(Mat);
			_30 = vcPos.x;
			_31 = vcPos.y;
			_32 = vcPos.z;
			_33 = 1.0f;
			if (bAxisY)
			{
				_10 = 0.0f;
				_11 = 1.0f;
				_12 = 0.0f;
			}
		};
		bool CreateFromLookDir(const MWDVector3& vcPos,const MWDVector3& vcDir,const MWDVector3& vcWorldUp = MWDVector3(0, 1, 0)) {
			MWDVector3 vcTemp, vcUp,vcWP = vcWorldUp;
			MWDVector3 Dir = vcDir;
			Dir.Normalize();
			MWDREAL fDot = vcWP.Dot( Dir);
			vcTemp = Dir * fDot;
			/*
					  /|
		worldup-->	 / |<---up
					/__|__
			vcTemp
			*/
			vcUp = vcWorldUp - vcTemp;
			MWDREAL fL = vcUp.GetLength();
			if (fL < EPSILON_E4)
			{
				MWDVector3 vcY;
				vcY.Set(0.0f, 0.0f, 1.0f);
				vcTemp = Dir * Dir.z;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();
				if (fL < EPSILON_E4)
				{
					vcY.Set(1.0f, 0.0f, 0.0f);
					vcTemp = Dir * Dir.x;
					vcUp = vcY - vcTemp;
					fL = vcUp.GetLength();
					if (fL < EPSILON_E4) return 0;
				}
			}
			vcUp /= fL;
			MWDVector3 vcRight;
			vcRight.Cross(vcUp, Dir);
			CreateInWorldObject(vcRight, vcUp, Dir, vcPos);
			return 1;
		};
		bool CreateFromLookAt(const MWDVector3& vcPos,const MWDVector3& vcLookAt,const MWDVector3& vcWorldUp = MWDVector3(0, 1, 0)) {
			MWDVector3 vcDir;
			vcDir = vcLookAt - vcPos;
			return CreateFromLookDir(vcPos, vcDir, vcWorldUp);
		};
		bool CreatePerspective(MWDREAL fFov,MWDREAL fAspect,MWDREAL fZN,MWDREAL fZF) {
			if (ABS(fZF - fZN) < 0.01f)
				return 0;

			MWDREAL sinFOV2 = SIN(fFov / 2);

			if (ABS(sinFOV2) < EPSILON_E5)
				return 0;

			MWDREAL cosFOV2 = COS(fFov / 2);

			MWDREAL w = 1.0f * (cosFOV2 / sinFOV2);
			MWDREAL h = fAspect * (cosFOV2 / sinFOV2);
			MWDREAL Q = fZF / (fZF - fZN);
			Identity();
			_00 = w;
			_11 = h;
			_22 = Q;
			_23 = 1.0f;
			_32 = -Q * fZN;
			_33 = 0;
			return 1;
		};
		bool CreateOrthogonal(MWDREAL fW,MWDREAL fH,MWDREAL fZN,MWDREAL fZF) {
			if (ABS(fZF - fZN) < 0.01f)
				return 0;
			Identity();
			_00 = 2.0f / fW;
			_11 = 2.0f / fH;
			_22 = 1.0f / (fZF - fZN);
			_32 = -fZN / (fZF - fZN);
			return 1;
		};
		bool CreateViewPort(MWDREAL fX, MWDREAL fY, MWDREAL fWidth, MWDREAL fHeight, MWDREAL fMinz, MWDREAL fMaxz) {
			if (fMinz > fMaxz)
				return 0;
			Identity();
			_00 = fWidth * 0.5f;
			_11 = -fHeight * 0.5f;
			_22 = fMaxz - fMinz;
			_30 = fX + _00;
			_31 = fY - _11;
			_32 = fMinz;

			return 1;
		};
		FORCEINLINE MWDMatrix3X3 Get3X3()const
		{
			MWDMatrix3X3 Mat;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++)
				{
					Mat.M[i][j] = M[i][j];
				}
			}
			return Mat;
		}
		MWDMatrix3X3W operator *(MWDREAL f)const {
			MWDMatrix3X3W Temp;
			Temp.m[0] = m[0] * f;
			Temp.m[1] = m[1] * f;
			Temp.m[2] = m[2] * f;
			Temp.m[3] = m[3] * f;
			Temp.m[4] = m[4] * f;
			Temp.m[5] = m[5] * f;
			Temp.m[6] = m[6] * f;
			Temp.m[7] = m[7] * f;
			Temp.m[8] = m[8] * f;
			Temp.m[9] = m[9] * f;
			Temp.m[10] = m[10] * f;
			Temp.m[11] = m[11] * f;
			Temp.m[12] = m[12] * f;
			Temp.m[13] = m[13] * f;
			Temp.m[14] = m[14] * f;
			Temp.m[15] = m[15] * f;
			return Temp;
		};
		MWDMatrix3X3W operator +(MWDREAL f)const {
			MWDMatrix3X3W Temp;
			Temp.m[0] = m[0] + f;
			Temp.m[1] = m[1] + f;
			Temp.m[2] = m[2] + f;
			Temp.m[3] = m[3] + f;
			Temp.m[4] = m[4] + f;
			Temp.m[5] = m[5] + f;
			Temp.m[6] = m[6] + f;
			Temp.m[7] = m[7] + f;
			Temp.m[8] = m[8] + f;
			Temp.m[9] = m[9] + f;
			Temp.m[10] = m[10] + f;
			Temp.m[11] = m[11] + f;
			Temp.m[12] = m[12] + f;
			Temp.m[13] = m[13] + f;
			Temp.m[14] = m[14] + f;
			Temp.m[15] = m[15] + f;
			return Temp;
		};
		MWDMatrix3X3W operator -(MWDREAL f)const {
			MWDMatrix3X3W Temp;
			Temp.m[0] = m[0] - f;
			Temp.m[1] = m[1] - f;
			Temp.m[2] = m[2] - f;
			Temp.m[3] = m[3] - f;
			Temp.m[4] = m[4] - f;
			Temp.m[5] = m[5] - f;
			Temp.m[6] = m[6] - f;
			Temp.m[7] = m[7] - f;
			Temp.m[8] = m[8] - f;
			Temp.m[9] = m[9] - f;
			Temp.m[10] = m[10] - f;
			Temp.m[11] = m[11] - f;
			Temp.m[12] = m[12] - f;
			Temp.m[13] = m[13] - f;
			Temp.m[14] = m[14] - f;
			Temp.m[15] = m[15] - f;
			return Temp;
		};
		MWDMatrix3X3W operator +(const MWDMatrix3X3W& Matirx)const {
			MWDMatrix3X3W Temp;
			Temp.m[0] = m[0] + Matirx.m[0];
			Temp.m[1] = m[1] + Matirx.m[1];
			Temp.m[2] = m[2] + Matirx.m[2];
			Temp.m[3] = m[3] + Matirx.m[3];
			Temp.m[4] = m[4] + Matirx.m[4];
			Temp.m[5] = m[5] + Matirx.m[5];
			Temp.m[6] = m[6] + Matirx.m[6];
			Temp.m[7] = m[7] + Matirx.m[7];
			Temp.m[8] = m[8] + Matirx.m[8];
			Temp.m[9] = m[9] + Matirx.m[9];
			Temp.m[10] = m[10] + Matirx.m[10];
			Temp.m[11] = m[11] + Matirx.m[11];
			Temp.m[12] = m[12] + Matirx.m[12];
			Temp.m[13] = m[13] + Matirx.m[13];
			Temp.m[14] = m[14] + Matirx.m[14];
			Temp.m[15] = m[15] + Matirx.m[15];
			return Temp;
		}
		MWDMatrix3X3W operator -(const MWDMatrix3X3W& Matirx)const {
			MWDMatrix3X3W Temp;
			Temp.m[0] = m[0] - Matirx.m[0];
			Temp.m[1] = m[1] - Matirx.m[1];
			Temp.m[2] = m[2] - Matirx.m[2];
			Temp.m[3] = m[3] - Matirx.m[3];
			Temp.m[4] = m[4] - Matirx.m[4];
			Temp.m[5] = m[5] - Matirx.m[5];
			Temp.m[6] = m[6] - Matirx.m[6];
			Temp.m[7] = m[7] - Matirx.m[7];
			Temp.m[8] = m[8] - Matirx.m[8];
			Temp.m[9] = m[9] - Matirx.m[9];
			Temp.m[10] = m[10] - Matirx.m[10];
			Temp.m[11] = m[11] - Matirx.m[11];
			Temp.m[12] = m[12] - Matirx.m[12];
			Temp.m[13] = m[13] - Matirx.m[13];
			Temp.m[14] = m[14] - Matirx.m[14];
			Temp.m[15] = m[15] - Matirx.m[15];
			return Temp;
		}

		void operator *= (MWDREAL f) {
			m[0] = m[0] * f;
			m[1] = m[1] * f;
			m[2] = m[2] * f;
			m[3] = m[3] * f;
			m[4] = m[4] * f;
			m[5] = m[5] * f;
			m[6] = m[6] * f;
			m[7] = m[7] * f;
			m[8] = m[8] * f;
			m[9] = m[9] * f;
			m[10] = m[10] * f;
			m[11] = m[11] * f;
			m[12] = m[12] * f;
			m[13] = m[13] * f;
			m[14] = m[14] * f;
			m[15] = m[15] * f;
		};
		void operator += (MWDREAL f) {
			m[0] = m[0] + f;
			m[1] = m[1] + f;
			m[2] = m[2] + f;
			m[3] = m[3] + f;
			m[4] = m[4] + f;
			m[5] = m[5] + f;
			m[6] = m[6] + f;
			m[7] = m[7] + f;
			m[8] = m[8] + f;
			m[9] = m[9] + f;
			m[10] = m[10] + f;
			m[11] = m[11] + f;
			m[12] = m[12] + f;
			m[13] = m[13] + f;
			m[14] = m[14] + f;
			m[15] = m[15] + f;
		};
		void operator -= (MWDREAL f) {
			m[0] = m[0] - f;
			m[1] = m[1] - f;
			m[2] = m[2] - f;
			m[3] = m[3] - f;
			m[4] = m[4] - f;
			m[5] = m[5] - f;
			m[6] = m[6] - f;
			m[7] = m[7] - f;
			m[8] = m[8] - f;
			m[9] = m[9] - f;
			m[10] = m[10] - f;
			m[11] = m[11] - f;
			m[12] = m[12] - f;
			m[13] = m[13] - f;
			m[14] = m[14] - f;
			m[15] = m[15] - f;
		};
		void operator += (const MWDMatrix3X3W& Matirx) {
			MWDMatrix3X3W Temp;
			m[0] = m[0] + Matirx.m[0];
			m[1] = m[1] + Matirx.m[1];
			m[2] = m[2] + Matirx.m[2];
			m[3] = m[3] + Matirx.m[3];
			m[4] = m[4] + Matirx.m[4];
			m[5] = m[5] + Matirx.m[5];
			m[6] = m[6] + Matirx.m[6];
			m[7] = m[7] + Matirx.m[7];
			m[8] = m[8] + Matirx.m[8];
			m[9] = m[9] + Matirx.m[9];
			m[10] = m[10] + Matirx.m[10];
			m[11] = m[11] + Matirx.m[11];
			m[12] = m[12] + Matirx.m[12];
			m[13] = m[13] + Matirx.m[13];
			m[14] = m[14] + Matirx.m[14];
			m[15] = m[15] + Matirx.m[15];
		};
		void operator -= (const MWDMatrix3X3W& Matirx) {
			MWDMatrix3X3W Temp;
			m[0] = m[0] - Matirx.m[0];
			m[1] = m[1] - Matirx.m[1];
			m[2] = m[2] - Matirx.m[2];
			m[3] = m[3] - Matirx.m[3];
			m[4] = m[4] - Matirx.m[4];
			m[5] = m[5] - Matirx.m[5];
			m[6] = m[6] - Matirx.m[6];
			m[7] = m[7] - Matirx.m[7];
			m[8] = m[8] - Matirx.m[8];
			m[9] = m[9] - Matirx.m[9];
			m[10] = m[10] - Matirx.m[10];
			m[11] = m[11] - Matirx.m[11];
			m[12] = m[12] - Matirx.m[12];
			m[13] = m[13] - Matirx.m[13];
			m[14] = m[14] - Matirx.m[14];
			m[15] = m[15] - Matirx.m[15];
		};

		bool operator ==(const MWDMatrix3X3W& v)const {
			for (unsigned int i = 0; i < 4; i++)
			{
				for (unsigned int j = 0; j < 4; j++)
				{
					if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
					{
						return false;
					}
				}
			}
			return true;
		};
		bool operator !=(const MWDMatrix3X3W& v)const {
			return !(*this == v);
		}
		void Add3X3(const MWDMatrix3X3& Mat) {
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					M[i][j] = Mat.M[i][j];
				}
			}
		};
		
		//添加平移操作
		void AddTranslate(const MWDVector3& V) {
			_30 = V.x;
			_31 = V.y;
			_32 = V.z;
		};
		//添加平移操作
		void AddTranslate(MWDREAL dx, MWDREAL dy, MWDREAL dz) {
			_30 = dx;
			_31 = dy;
			_32 = dz;
		};
		//生成单位矩阵
		FORCEINLINE void Identity(void) {
			MWDMemset(m, 0, sizeof(MWDMatrix3X3W));
			_00 = _11 = _22 = _33 = 1.0f;
		};
		FORCEINLINE void SetZero() {
			MWDMemset(M, 0, sizeof(MWDMatrix3X3W));
		};

		static MWDMatrix3X3W Transpose(const MWDMatrix3X3W& Matrix) {
			MWDMatrix3X3W ret;
			ret._00 = Matrix._00;
			ret._10 = Matrix._01;
			ret._20 = Matrix._02;
			ret._30 = Matrix._03;

			ret._01 = Matrix._10;
			ret._11 = Matrix._11;
			ret._21 = Matrix._12;
			ret._31 = Matrix._13;

			ret._02 = Matrix._20;
			ret._12 = Matrix._21;
			ret._22 = Matrix._22;
			ret._32 = Matrix._23;

			ret._03 = Matrix._30;
			ret._13 = Matrix._31;
			ret._23 = Matrix._32;
			ret._33 = Matrix._33;
			return ret;
		};
		static MWDMatrix3X3W Inverse(const MWDMatrix3X3W& Matirx) {
			MWDMatrix3X3W ret;
			MWDMatrix3X3W mTrans;
			MWDREAL    fTemp[12], fDet;

			mTrans = MWDMatrix3X3W::Transpose(Matirx);

			fTemp[0] = mTrans._22 * mTrans._33;
			fTemp[1] = mTrans._23 * mTrans._32;
			fTemp[2] = mTrans._21 * mTrans._33;
			fTemp[3] = mTrans._23 * mTrans._31;
			fTemp[4] = mTrans._21 * mTrans._32;
			fTemp[5] = mTrans._22 * mTrans._31;
			fTemp[6] = mTrans._20 * mTrans._33;
			fTemp[7] = mTrans._23 * mTrans._30;
			fTemp[8] = mTrans._20 * mTrans._32;
			fTemp[9] = mTrans._22 * mTrans._30;
			fTemp[10] = mTrans._20 * mTrans._31;
			fTemp[11] = mTrans._21 * mTrans._30;


			ret._00 = fTemp[0] * mTrans._11 + fTemp[3] * mTrans._12 + fTemp[4] * mTrans._13;
			ret._00 -= fTemp[1] * mTrans._11 + fTemp[2] * mTrans._12 + fTemp[5] * mTrans._13;
			ret._01 = fTemp[1] * mTrans._10 + fTemp[6] * mTrans._12 + fTemp[9] * mTrans._13;
			ret._01 -= fTemp[0] * mTrans._10 + fTemp[7] * mTrans._12 + fTemp[8] * mTrans._13;
			ret._02 = fTemp[2] * mTrans._10 + fTemp[7] * mTrans._11 + fTemp[10] * mTrans._13;
			ret._02 -= fTemp[3] * mTrans._10 + fTemp[6] * mTrans._11 + fTemp[11] * mTrans._13;
			ret._03 = fTemp[5] * mTrans._10 + fTemp[8] * mTrans._11 + fTemp[11] * mTrans._12;
			ret._03 -= fTemp[4] * mTrans._10 + fTemp[9] * mTrans._11 + fTemp[10] * mTrans._12;
			ret._10 = fTemp[1] * mTrans._01 + fTemp[2] * mTrans._02 + fTemp[5] * mTrans._03;
			ret._10 -= fTemp[0] * mTrans._01 + fTemp[3] * mTrans._02 + fTemp[4] * mTrans._03;
			ret._11 = fTemp[0] * mTrans._00 + fTemp[7] * mTrans._02 + fTemp[8] * mTrans._03;
			ret._11 -= fTemp[1] * mTrans._00 + fTemp[6] * mTrans._02 + fTemp[9] * mTrans._03;
			ret._12 = fTemp[3] * mTrans._00 + fTemp[6] * mTrans._01 + fTemp[11] * mTrans._03;
			ret._12 -= fTemp[2] * mTrans._00 + fTemp[7] * mTrans._01 + fTemp[10] * mTrans._03;
			ret._13 = fTemp[4] * mTrans._00 + fTemp[9] * mTrans._01 + fTemp[10] * mTrans._02;
			ret._13 -= fTemp[5] * mTrans._00 + fTemp[8] * mTrans._01 + fTemp[11] * mTrans._02;


			fTemp[0] = mTrans._02 * mTrans._13;
			fTemp[1] = mTrans._03 * mTrans._12;
			fTemp[2] = mTrans._01 * mTrans._13;
			fTemp[3] = mTrans._03 * mTrans._11;
			fTemp[4] = mTrans._01 * mTrans._12;
			fTemp[5] = mTrans._02 * mTrans._11;
			fTemp[6] = mTrans._00 * mTrans._13;
			fTemp[7] = mTrans._03 * mTrans._10;
			fTemp[8] = mTrans._00 * mTrans._12;
			fTemp[9] = mTrans._02 * mTrans._10;
			fTemp[10] = mTrans._00 * mTrans._11;
			fTemp[11] = mTrans._01 * mTrans._10;


			ret._20 = fTemp[0] * mTrans._31 + fTemp[3] * mTrans._32 + fTemp[4] * mTrans._33;
			ret._20 -= fTemp[1] * mTrans._31 + fTemp[2] * mTrans._32 + fTemp[5] * mTrans._33;
			ret._21 = fTemp[1] * mTrans._30 + fTemp[6] * mTrans._32 + fTemp[9] * mTrans._33;
			ret._21 -= fTemp[0] * mTrans._30 + fTemp[7] * mTrans._32 + fTemp[8] * mTrans._33;
			ret._22 = fTemp[2] * mTrans._30 + fTemp[7] * mTrans._31 + fTemp[10] * mTrans._33;
			ret._22 -= fTemp[3] * mTrans._30 + fTemp[6] * mTrans._31 + fTemp[11] * mTrans._33;
			ret._23 = fTemp[5] * mTrans._30 + fTemp[8] * mTrans._31 + fTemp[11] * mTrans._32;
			ret._23 -= fTemp[4] * mTrans._30 + fTemp[9] * mTrans._31 + fTemp[10] * mTrans._32;
			ret._30 = fTemp[2] * mTrans._22 + fTemp[5] * mTrans._23 + fTemp[1] * mTrans._21;
			ret._30 -= fTemp[4] * mTrans._23 + fTemp[0] * mTrans._21 + fTemp[3] * mTrans._22;
			ret._31 = fTemp[8] * mTrans._23 + fTemp[0] * mTrans._20 + fTemp[7] * mTrans._22;
			ret._31 -= fTemp[6] * mTrans._22 + fTemp[9] * mTrans._23 + fTemp[1] * mTrans._20;
			ret._32 = fTemp[6] * mTrans._21 + fTemp[11] * mTrans._23 + fTemp[3] * mTrans._20;
			ret._32 -= fTemp[10] * mTrans._23 + fTemp[2] * mTrans._20 + fTemp[7] * mTrans._21;
			ret._33 = fTemp[10] * mTrans._22 + fTemp[4] * mTrans._20 + fTemp[9] * mTrans._21;
			ret._33 -= fTemp[8] * mTrans._21 + fTemp[11] * mTrans._22 + fTemp[5] * mTrans._20;

			fDet = mTrans._00 * ret._00 + mTrans._01 * ret._01 + mTrans._02 * ret._02 + mTrans._03 * ret._03;

			fDet = 1 / fDet;

			ret._00 *= fDet;
			ret._01 *= fDet;
			ret._02 *= fDet;
			ret._03 *= fDet;

			ret._10 *= fDet;
			ret._11 *= fDet;
			ret._12 *= fDet;
			ret._13 *= fDet;

			ret._20 *= fDet;
			ret._21 *= fDet;
			ret._22 *= fDet;
			ret._23 *= fDet;

			ret._30 *= fDet;
			ret._31 *= fDet;
			ret._32 *= fDet;
			ret._33 *= fDet;
			return ret;
		};

		FORCEINLINE MWDMatrix3X3W GetTranspose()const {
			return MWDMatrix3X3W::Transpose(*this);
		};
		FORCEINLINE MWDMatrix3X3W GetInverse()const {
			return MWDMatrix3X3W::Inverse(*this);
		};

		FORCEINLINE MWDVector3 GetTranslation(void)const {
			return MWDVector3(_30, _31, _32);
		};
		FORCEINLINE MWDMatrix3X3W operator * (const MWDMatrix3X3W& Matirx)const {
			MWDMatrix3X3W mResult;

			mResult.M[0][0] = 0;
			mResult.M[1][1] = 0;
			mResult.M[2][2] = 0;
			mResult.M[3][3] = 0;
			for (unsigned char i = 0; i < 4; i++)
				for (unsigned char j = 0; j < 4; j++)
					for (unsigned int k = 0; k < 4; k++)
						mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
			return mResult;
		};
		FORCEINLINE MWDVector3 operator * (const MWDVector3& vc)const {
			MWDVector3 vcResult ;

			vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
			vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
			vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;

			MWDREAL w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

			vcResult.x = vcResult.x / w;
			vcResult.y = vcResult.y / w;
			vcResult.z = vcResult.z / w;

			return vcResult;
		};
		FORCEINLINE MWDVector3W operator * (const MWDVector3W& vc)const {
			MWDVector3W vcResult;
			vcResult.x = vc.x * _00 + vc.y * _10 + vc.z * _20 + _30;
			vcResult.y = vc.x * _01 + vc.y * _11 + vc.z * _21 + _31;
			vcResult.z = vc.x * _02 + vc.y * _12 + vc.z * _22 + _32;
			vcResult.w = vc.x * _03 + vc.y * _13 + vc.z * _23 + _33;

			vcResult.x = vcResult.x / vcResult.w;
			vcResult.y = vcResult.y / vcResult.w;
			vcResult.z = vcResult.z / vcResult.w;
			vcResult.w = 1.0f;

			return vcResult;
		};
		FORCEINLINE MWDVector3 Apply3X3(const MWDVector3& v)const {
			MWDREAL x, y, z;
			x = v.x * _00 + v.y * _10 + v.z * _20;
			y = v.x * _01 + v.y * _11 + v.z * _21;
			z = v.x * _02 + v.y * _12 + v.z * _22;

			return MWDVector3(x, y, z);
		};
		FORCEINLINE MWDVector3 ApplyTranlaste(const MWDVector3& Point)const {
			return MWDVector3(Point.x + _30, Point.y + _31, Point.z + _32);
		};
		void GetRowVector(MWDVector3W Row[4])const {
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
					Row[i].m[j] = M[i][j];
			}
		};
		void GetColumnVector(MWDVector3W Column[4])const {
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
					Column[i].m[j] = M[j][i];
			}
		};
		void GetRowVector(MWDVector3W& Row0, MWDVector3W& Row1, MWDVector3W& Row2, MWDVector3W& Row3)const {
			Row0.x = M[0][0]; Row0.y = M[0][1]; Row0.z = M[0][2]; Row0.w = M[0][3];
			Row1.x = M[1][0]; Row1.y = M[1][1]; Row1.z = M[1][2]; Row1.w = M[1][3];
			Row2.x = M[2][0]; Row2.y = M[2][1]; Row2.z = M[2][2]; Row2.w = M[2][3];
			Row3.x = M[3][0]; Row3.y = M[3][1]; Row3.z = M[3][2]; Row3.w = M[3][3];
		};
		void GetColumnVector(MWDVector3W& Column0, MWDVector3W& Column1, MWDVector3W& Column2, MWDVector3W& Column3)const {
			Column0.x = M[0][0]; Column0.y = M[1][0]; Column0.z = M[2][0]; Column0.w = M[3][0];
			Column1.x = M[0][1]; Column1.y = M[1][1]; Column1.z = M[2][1]; Column1.w = M[3][1];
			Column2.x = M[0][2]; Column2.y = M[1][2]; Column2.z = M[2][2]; Column2.w = M[3][2];
			Column3.x = M[0][3]; Column3.y = M[1][3]; Column3.z = M[2][3]; Column3.w = M[3][3];
		};
	}; 
	static MWDMatrix3X3W ms_Materix3X3WIdentity;
}
