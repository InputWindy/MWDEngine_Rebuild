#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class  MWDMATH_API MWDMatrix3X3
	{
	public:
		union
		{
			MWDREAL m[9];
			struct
			{
				MWDREAL _00, _01, _02;
				MWDREAL _10, _11, _12;
				MWDREAL _20, _21, _22;
			};
			MWDREAL M[3][3];
		};
		MWDMatrix3X3(void) {
			Identity();
		};
		MWDMatrix3X3(MWDREAL m00, MWDREAL m01, MWDREAL m02,MWDREAL m10, MWDREAL m11, MWDREAL m12,MWDREAL m20, MWDREAL m21, MWDREAL m22) {
			_00 = m00; _01 = m01; _02 = m02;
			_10 = m10; _11 = m11; _12 = m12;
			_20 = m20; _21 = m21; _22 = m22;
		};
		
		void GetEluer(MWDREAL& Yaw, MWDREAL& Pitch, MWDREAL& Roll)const {
			if (_21 > 1.0f)
				Pitch = -MWDPI2;
			else if (_21 < -1.0f)
				Pitch = MWDPI2;
			else
				Pitch = ASIN(-_21);

			if (fabs(_21) > MWDREAL_1)
			{
				Yaw = 0;
				Roll = ATAN2(-_10, _00);
			}
			else
			{
				Roll = ATAN2(_01, _11);
				Yaw = ATAN2(_20, _22);
			}
		};
	
		void GetAxisAngle(MWDVector3& Axis, MWDREAL& fAngle)const {
			fAngle = ACOS(Clamp((_00 + _11 + _22 - 1) * 0.5f, 1.0f, -1.0f));
			if (fAngle < MWDPI - EPSILON_E4)
			{
				MWDVector3 Temp;
				Temp.Set(M[1][2] - M[2][1], M[2][0] - M[0][2], M[0][1] - M[1][0]);
				Temp.Normalize();
				Axis = Temp;
			}
			else
			{
				int flag = 0;
				MWDREAL fMax = _00;
				if (fMax < _11)
				{
					fMax = _11;
					flag = 1;
				}
				if (fMax < _22)
				{
					fMax = _22;
					flag = 2;
				}


				if (flag == 0)
				{
					MWDREAL wTemp = SQRT(_00 - _11 - _22 + 1) * 0.5f;
					MWDREAL InWTemp = 1.0f / wTemp;
					Axis.m[0] = wTemp;
					Axis.m[1] = M[0][1] * 0.5f * InWTemp;
					Axis.m[2] = M[0][2] * 0.5f * InWTemp;
				}
				else if (flag == 1)
				{
					MWDREAL wTemp = SQRT(_11 - _00 - _22 + 1) * 0.5f;
					MWDREAL InWTemp = 1.0f / wTemp;
					Axis.m[1] = wTemp;
					Axis.m[0] = M[0][1] * 0.5f * InWTemp;
					Axis.m[2] = M[1][2] * 0.5f * InWTemp;
				}
				else
				{
					MWDREAL wTemp = SQRT(_22 - _00 - _11 + 1) * 0.5f;
					MWDREAL InWTemp = 1.0f / wTemp;
					Axis.m[2] = wTemp;
					Axis.m[0] = M[0][2] * 0.5f * InWTemp;
					Axis.m[1] = M[1][2] * 0.5f * InWTemp;
				}
			}
		};
	
		
		MWDMatrix3X3 operator *(MWDREAL f)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] * f;
			Temp.m[1] = m[1] * f;
			Temp.m[2] = m[2] * f;
			Temp.m[3] = m[3] * f;
			Temp.m[4] = m[4] * f;
			Temp.m[5] = m[5] * f;
			Temp.m[6] = m[6] * f;
			Temp.m[7] = m[7] * f;
			Temp.m[8] = m[8] * f;

			return Temp;
		};
		MWDMatrix3X3 operator /(MWDREAL f)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] / f;
			Temp.m[1] = m[1] / f;
			Temp.m[2] = m[2] / f;
			Temp.m[3] = m[3] / f;
			Temp.m[4] = m[4] / f;
			Temp.m[5] = m[5] / f;
			Temp.m[6] = m[6] / f;
			Temp.m[7] = m[7] / f;
			Temp.m[8] = m[8] / f;

			return Temp;
		};
		//全部加上一个常数f
		MWDMatrix3X3 operator +(MWDREAL f)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] + f;
			Temp.m[1] = m[1] + f;
			Temp.m[2] = m[2] + f;
			Temp.m[3] = m[3] + f;
			Temp.m[4] = m[4] + f;
			Temp.m[5] = m[5] + f;
			Temp.m[6] = m[6] + f;
			Temp.m[7] = m[7] + f;
			Temp.m[8] = m[8] + f;
			return Temp;
		};
		//全部减去一个常数f
		MWDMatrix3X3 operator -(MWDREAL f)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] - f;
			Temp.m[1] = m[1] - f;
			Temp.m[2] = m[2] - f;
			Temp.m[3] = m[3] - f;
			Temp.m[4] = m[4] - f;
			Temp.m[5] = m[5] - f;
			Temp.m[6] = m[6] - f;
			Temp.m[7] = m[7] - f;
			Temp.m[8] = m[8] - f;
			return Temp;
		};
		MWDMatrix3X3 operator +(const MWDMatrix3X3& Matirx)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] + Matirx.m[0];
			Temp.m[1] = m[1] + Matirx.m[1];
			Temp.m[2] = m[2] + Matirx.m[2];
			Temp.m[3] = m[3] + Matirx.m[3];
			Temp.m[4] = m[4] + Matirx.m[4];
			Temp.m[5] = m[5] + Matirx.m[5];
			Temp.m[6] = m[6] + Matirx.m[6];
			Temp.m[7] = m[7] + Matirx.m[7];
			Temp.m[8] = m[8] + Matirx.m[8];
			return Temp;
		};
		MWDMatrix3X3 operator -(const MWDMatrix3X3& Matirx)const {
			MWDMatrix3X3 Temp;
			Temp.m[0] = m[0] - Matirx.m[0];
			Temp.m[1] = m[1] - Matirx.m[1];
			Temp.m[2] = m[2] - Matirx.m[2];
			Temp.m[3] = m[3] - Matirx.m[3];
			Temp.m[4] = m[4] - Matirx.m[4];
			Temp.m[5] = m[5] - Matirx.m[5];
			Temp.m[6] = m[6] - Matirx.m[6];
			Temp.m[7] = m[7] - Matirx.m[7];
			Temp.m[8] = m[8] - Matirx.m[8];

			return Temp;
		};
		FORCEINLINE MWDMatrix3X3 operator * (const MWDMatrix3X3& Matirx)const {
			MWDMatrix3X3 mResult;
			mResult.M[0][0] = 0;
			mResult.M[1][1] = 0;
			mResult.M[2][2] = 0;
			for (unsigned char i = 0; i < 3; i++)
				for (unsigned char j = 0; j < 3; j++)
					for (unsigned int k = 0; k < 3; k++)
						mResult.M[i][j] += M[i][k] * Matirx.M[k][j];
			return mResult;
		};
		
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
		};
		void operator += (const MWDMatrix3X3& Matirx) {
			m[0] = m[0] + Matirx.m[0];
			m[1] = m[1] + Matirx.m[1];
			m[2] = m[2] + Matirx.m[2];
			m[3] = m[3] + Matirx.m[3];
			m[4] = m[4] + Matirx.m[4];
			m[5] = m[5] + Matirx.m[5];
			m[6] = m[6] + Matirx.m[6];
			m[7] = m[7] + Matirx.m[7];
			m[8] = m[8] + Matirx.m[8];
		};
		void operator -= (const MWDMatrix3X3& Matirx) {
			m[0] = m[0] - Matirx.m[0];
			m[1] = m[1] - Matirx.m[1];
			m[2] = m[2] - Matirx.m[2];
			m[3] = m[3] - Matirx.m[3];
			m[4] = m[4] - Matirx.m[4];
			m[5] = m[5] - Matirx.m[5];
			m[6] = m[6] - Matirx.m[6];
			m[7] = m[7] - Matirx.m[7];
			m[8] = m[8] - Matirx.m[8];
		};
		bool operator ==(const MWDMatrix3X3& v)const {
			for (unsigned int i = 0; i < 3; i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					if (ABS(M[i][j] - v.M[i][j]) > EPSILON_E4)
					{
						return false;
					}
				}

			}
			return true;
		};
		bool operator != (const MWDMatrix3X3 & v) const{
			return !(*this == v);
		}

		void GetRowVector(MWDVector3 Row[3])const {
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					Row[i].m[j] = M[i][j];
			}
		};
		void GetColumnVector(MWDVector3 Column[3])const {
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
					Column[i].m[j] = M[j][i];
			}
		};
		void GetRowVector(MWDVector3& Row0, MWDVector3& Row1, MWDVector3& Row2)const {
			Row0.x = M[0][0]; Row0.y = M[0][1]; Row0.z = M[0][2];
			Row1.x = M[1][0]; Row1.y = M[1][1]; Row1.z = M[1][2];
			Row2.x = M[2][0]; Row2.y = M[2][1]; Row2.z = M[2][2];
		};
		void GetColumnVector(MWDVector3& Column0, MWDVector3& Column1, MWDVector3& Column2)const {
			Column0.x = M[0][0]; Column0.y = M[1][0]; Column0.z = M[2][0];
			Column1.x = M[0][1]; Column1.y = M[1][1]; Column1.z = M[2][1];
			Column2.x = M[0][2]; Column2.y = M[1][2]; Column2.z = M[2][2];
		};
	
		void GetUVN(MWDVector3 UVN[3])const {
			UVN[0].x = M[0][0]; UVN[0].y = M[0][1]; UVN[0].z = M[0][2];
			UVN[1].x = M[1][0]; UVN[1].y = M[1][1]; UVN[1].z = M[1][2];
			UVN[2].x = M[2][0]; UVN[2].y = M[2][1]; UVN[2].z = M[2][2];
		};
		void GetUVN(MWDVector3& U, MWDVector3& V, MWDVector3& N)const {
			U.x = M[0][0]; U.y = M[0][1]; U.z = M[0][2];
			V.x = M[1][0]; V.y = M[1][1]; V.z = M[1][2];
			N.x = M[2][0]; N.y = M[2][1]; N.z = M[2][2];
		};
		void GetEigenSystem(MWDREAL EigenValue[3], MWDVector3 Eigen[3])const {
			MWDREAL Max;
			unsigned int p, q;
			MWDREAL r;
			MWDREAL t;
			MWDREAL cos, sin;
			MWDMatrix3X3 Temp, TempTranspose, EigenMatrix, A;
			A = *this;
			do
			{
				Max = A.M[0][2];
				for (unsigned int i = 0; i < 3; i++)
				{
					for (unsigned int j = i; j < 3; j++)
					{
						if (i != j)
						{
							if (ABS(A.M[i][j]) > ABS(Max))
							{
								Max = A.M[i][j];
								p = i;
								q = j;

							}
						}
					}

				}
				if (ABS(Max) < EPSILON_E4) break;
				r = (A.M[p][p] - A.M[q][q]) / Max;
				if (r > EPSILON_E4)
					t = 1.0f / (ABS(r) + SQRT(r * r + 1));
				else if (r < -EPSILON_E4)
					t = -1.0f / (ABS(r) + SQRT(r * r + 1));
				else
					t = 1.0f;
				cos = 1.0f / SQRT(1 + t * t);
				sin = t * cos;
				Temp.M[0][0] = cos;	Temp.M[0][1] = -sin;	Temp.M[0][2] = 0;
				Temp.M[1][0] = sin;	Temp.M[1][1] = cos;		Temp.M[1][2] = 0;
				Temp.M[2][0] = 0;	Temp.M[2][1] = 0;		Temp.M[2][2] = 1;

				EigenMatrix = EigenMatrix * Temp;
				TempTranspose.TransposeOf(Temp);

				A = TempTranspose * A * Temp;

			} while (ABS(Max) > EPSILON_E4);

			for (unsigned int i = 0; i < 3; i++)
			{
				EigenValue[i] = A.M[i][i];
			}
			EigenMatrix.GetUVN(Eigen);
		};
		//生成单位矩阵
		FORCEINLINE void Identity(void) {
			MWDMemset(m, 0, sizeof(MWDMatrix3X3));
			_00 = _11 = _22 = 1.0f;
		};
		//转置
		FORCEINLINE void TransposeOf(const MWDMatrix3X3& Matrix) {
			_00 = Matrix._00;
			_10 = Matrix._01;
			_20 = Matrix._02;


			_01 = Matrix._10;
			_11 = Matrix._11;
			_21 = Matrix._12;


			_02 = Matrix._20;
			_12 = Matrix._21;
			_22 = Matrix._22;
		};
		FORCEINLINE void InverseOf(const MWDMatrix3X3& Matirx) {
			MWDREAL det = Matirx.Det();

			if (ABS(det) < EPSILON_E4)
				return;

			MWDREAL det_inv = 1.0f / det;
			M[0][0] = det_inv * (Matirx.M[1][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[1][2]);
			M[1][0] = -det_inv * (Matirx.M[1][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[1][2]);
			M[2][0] = det_inv * (Matirx.M[1][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[1][1]);

			M[0][1] = -det_inv * (Matirx.M[0][1] * Matirx.M[2][2] - Matirx.M[2][1] * Matirx.M[0][2]);
			M[1][1] = det_inv * (Matirx.M[0][0] * Matirx.M[2][2] - Matirx.M[2][0] * Matirx.M[0][2]);
			M[2][1] = -det_inv * (Matirx.M[0][0] * Matirx.M[2][1] - Matirx.M[2][0] * Matirx.M[0][1]);

			M[0][2] = det_inv * (Matirx.M[0][1] * Matirx.M[1][2] - Matirx.M[1][1] * Matirx.M[0][2]);
			M[1][2] = -det_inv * (Matirx.M[0][0] * Matirx.M[1][2] - Matirx.M[1][0] * Matirx.M[0][2]);
			M[2][2] = det_inv * (Matirx.M[0][0] * Matirx.M[1][1] - Matirx.M[1][0] * Matirx.M[0][1]);
		};
		FORCEINLINE MWDREAL Det()const {
			return (M[0][0] * (M[1][1] * M[2][2] - M[2][1] * M[1][2]) -
				M[0][1] * (M[1][0] * M[2][2] - M[2][0] * M[1][2]) +
				M[0][2] * (M[1][0] * M[2][1] - M[2][0] * M[1][1]));
		};
		
		FORCEINLINE void CreateFromTwoVector(const MWDVector3& v1, const MWDVector3& v2) {
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					M[i][j] = v1.m[i] * v2.m[j];
				}
			}
		};
		FORCEINLINE void SetZero() {
			MWDMemset(m, 0, sizeof(MWDMatrix3X3));
		};
		void GetScale(MWDVector3& Scale)const {
			MWDVector3 RowVector[3];
			GetRowVector(RowVector);
			for (unsigned int i = 0; i < 3; i++)
			{
				Scale.m[i] = RowVector[i].GetLength();
				RowVector[i] /= Scale.m[i];
			}
		};
		void GetScaleAndRotated(MWDVector3& Scale) {
			MWDVector3 RowVector[3];
			GetRowVector(RowVector);

			for (unsigned int i = 0; i < 3; i++)
			{
				Scale.m[i] = RowVector[i].GetLength();
				RowVector[i] /= Scale.m[i];
			}
			*this = MWDMatrix3X3(RowVector[0].x, RowVector[0].y, RowVector[0].z,
				RowVector[1].x, RowVector[1].y, RowVector[1].z,
				RowVector[2].x, RowVector[2].y, RowVector[2].z);
		};
		void Print() const{
			cout << endl;
			cout << _00 << "," << _01 << "," << _02 << endl;
			cout << _10 << "," << _11 << "," << _12 << endl;
			cout << _20 << "," << _21 << "," << _22 << endl;
			cout << endl;
		}
		const static MWDMatrix3X3 ms_CameraViewRight;
		const static MWDMatrix3X3 ms_CameraViewLeft;
		const static MWDMatrix3X3 ms_CameraViewUp;
		const static MWDMatrix3X3 ms_CameraViewDown;
		const static MWDMatrix3X3 ms_CameraViewFront;
		const static MWDMatrix3X3 ms_CameraViewBack;
		const static MWDMatrix3X3 ms_Identity;

	}; 
}
