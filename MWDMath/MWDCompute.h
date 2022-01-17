#pragma once
#include "MWDAABB3.h"
//#include "MWDBezierCurve3.h"
//#include "MWDBezierSurface3.h"
//#include "MWDB_SplineCurve3.h"
//#include "MWDB_SplineSurface3.h"
//#include "MWDControlCurve3.h"
//#include "MWDControlSurface3.h"
#include "MWDCurve3.h"
//#include "MWDCurve3Subdivision.h"
//#include "MWDCurve3SubdivisionByError.h"
//#include "MWDCurve3SubdivisionByLength.h"
//#include "MWDCurve3SubdivisionByUniform.h"
//#include "MWDCurveSet3.h"
#include "MWDCylinder3.h"
//#include "MWDCylinderSurface3.h"
//#include "MWDGeneralizedCylinderSurface3.h"
#include "MWDLine3.h"
#include "MWDMath.h"
#include "MWDMatrix3X3.h"
#include "MWDMatrix3X3W.h"
//#include "MWDNURBSCurve3.h"
//#include "MWDNURBSSurface3.h"
#include "MWDOBB3.h"
#include "MWDPlane3.h"
#include "MWDPolygon3.h"
#include "MWDQuat.h"
#include "MWDRay3.h"
#include "MWDRect2.h"
#include "MWDRectangle3.h"
//#include "MWDRevolutionSurface3.h"
#include "MWDSegment3.h"
//#include "MWDSegmentCurve3.h"
#include "MWDSphere3.h"
#include "MWDSurface3.h"
//#include "MWDSurface3SubDivision.h"
//#include "MWDSurface3SubdivisionByLength.h"
//#include "MWDSurface3SubDivisionByUniform.h"
#include "MWDTriangle3.h"
//#include "MWDTubeSurface3.h"
#include "MWDVector2.h"
#include "MWDVector3.h"
#include "MWDVector3W.h"
namespace MWDEngine {
	//判断2D相交
	class MWDMATH_API MWD {
	public:
		//globalMath
		static bool  IsUniformScale(MWDREAL fScale) {
			if (ABS(fScale - 1.0f) < EPSILON_E4)
			{
				return true;
			}
			return false;
		};
		static bool  IsZeroTranslate(const MWDVector3& Translate) {
			if (Translate.GetSqrLength() < EPSILON_E4)
			{
				return true;
			}
			return false;
		};
		static bool  IsIdentityRotate(const MWDMatrix3X3& Rotate) {
			for (unsigned int i = 0; i < 3; i++)
			{
				for (unsigned int j = 0; j < 3; j++)
				{
					if (i != j)
					{
						if (ABS(Rotate.M[i][j]) > EPSILON_E4)
						{
							return false;
						}
					}
					else
					{
						if (ABS(Rotate.M[i][j] - 1.0f) > EPSILON_E4)
						{
							return false;
						}
					}

				}
			}
			return true;
		};
		static bool  IsIdentityRotate(const MWDQuat& Rotate) {
			MWDVector3 R(Rotate.x, Rotate.y, Rotate.z);
			if (R.GetSqrLength() < EPSILON_E4)
			{
				return true;
			}
			return false;
		};
		static MWDREAL  LineInterpolation(MWDREAL t1, MWDREAL t2, MWDREAL t) {
			return t1 + (t2 - t1) * t;
		};
		static MWDVector3  LineInterpolation(const MWDVector3& t1, const MWDVector3& t2, MWDREAL t) {
			return t1 + (t2 - t1) * t;
		};
		static MWDQuat  LineInterpolation(const MWDQuat& t1, const MWDQuat& t2, MWDREAL t) {
			MWDREAL fCos = t2.Dot(t1);
			MWDQuat Temp = t1;
			if (fCos < 0.0f)
			{
				Temp = t1 * (-1.0f);
			}
			return Temp + (t2 - Temp) * t;
		};
		static MWDVector3W  LineInterpolation(const MWDVector3W& t1, const MWDVector3W& t2, MWDREAL t) {
			return t1 + (t2 - t1) * t;
		};

		//vector3
		static MWDVector3 Cross(const MWDVector3& v1, const MWDVector3& v2) {
			return MWDVector3(
				v1.y * v2.z - v1.z * v2.y,
				v1.z * v2.x - v1.x * v2.z,
				v1.x * v2.y - v1.y * v2.x
				);
		};
		static MWDREAL Dot(const MWDVector3& v1, const MWDVector3& v2) {
			return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
		};
		//计算两向量夹角
		static MWDREAL AngleWith(const MWDVector3& v1, const MWDVector3& v2) { return (MWDREAL)acos(MWD::Dot(v1, v2) / (v1.GetLength() * v2.GetLength())); };
		static MWDVector3 GetDir(const MWDVector3& v1) {
			MWDREAL L = v1.GetLength();
			return MWDVector3(
				v1.x / L,
				v1.y / L,
				v1.z / L
			);
		}
		/*            N     _
				 \	  /|\   /|
			Dir	  \	   |   /  Reflect
				  _\|  |  /
		--------------------------*/
		//输入法线，计算对应的反射方向
		static MWDVector3 ReflectDir(const MWDVector3& i,const MWDVector3& N) {
			//v = i - 2 * dot(i, n) * n
			return MWDVector3(i - N* 2 * MWD::Dot(i, N));
		};
		static MWDQuat Vec3MulQuat(const MWDVector3& v,const MWDQuat& q) {
			return MWDQuat(
				q.w * v.x + q.z * v.y - q.y * v.z,
				q.w * v.y + q.x * v.z - q.z * v.x,
				q.w * v.z + q.y * v.x - q.x * v.y,
				-(q.x * v.x + q.y * v.y + q.z * v.z));
		};
		static MWDVector3 Vec3MulMat3x3 (const MWDVector3& v, const MWDMatrix3X3& m) {
			return MWDVector3(
				v.x * m._00 + v.y * m._10 + v.z * m._20,
				v.x * m._01 + v.y * m._11 + v.z * m._21,
				v.x * m._02 + v.y * m._12 + v.z * m._22);
		};
		static MWDVector3 Vec3MulMat3x3W(const MWDVector3& v, const MWDMatrix3X3W& m) {
			MWDVector3 out;
			out.Set(v.x * m._00 + v.y * m._10 + v.z * m._20,
				v.x * m._01 + v.y * m._11 + v.z * m._21,
				v.x * m._02 + v.y * m._12 + v.z * m._22);
			MWDREAL w = v.x * m._03 + v.y * m._13 + v.z * m._23 + m._33;

			out.x = out.x / w;
			out.y = out.y / w;
			out.z = out.z / w;
			return out;
		};
		static bool IsParallel(const MWDVector3& V1,const MWDVector3& V2) {
			MWDREAL t0, t1;
			bool temp = 0;
			t0 = V1.x * V2.y;
			t1 = V1.y * V2.x;

			if (ABS(t0 - t1) > EPSILON_E4)
				temp = 1;

			t0 = V1.y * V2.z;
			t1 = V1.z * V2.y;

			if (ABS(t0 - t1) > EPSILON_E4 && temp)
				return 1;
			else
				return 0;
		};
		static MWDREAL SquaredDistance(const MWDVector3& P1,const MWDVector3& P2) { return (P1.x * P2.x + P1.y * P2.y + P1.z * P2.z); };
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDLine3& Line, MWDREAL& fLineParameter) { return SquaredDistance(Line,P1, fLineParameter); };
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDRay3& Ray, MWDREAL& fRayParameter) { return SquaredDistance(Ray,P1, fRayParameter); };
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDSegment3& Segment, MWDREAL& fSegmentParameter) { return  SquaredDistance(Segment ,P1, fSegmentParameter); };
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDTriangle3& Triangle, MWDREAL fTriangleParameter[3]) {
			MWDVector3 TrianglePoint[3];
			Triangle.GetPoint(TrianglePoint);
			MWDVector3 Diff = TrianglePoint[0] - P1;
			MWDVector3 Edge0 = TrianglePoint[1] - TrianglePoint[0];
			MWDVector3 Edge1 = TrianglePoint[2] - TrianglePoint[0];
			MWDREAL fA00 = Edge0.GetSqrLength();
			MWDREAL fA01 = Edge0.Dot(Edge1);
			MWDREAL fA11 = Edge1.GetSqrLength();
			MWDREAL fB0 = Diff.Dot(Edge0);
			MWDREAL fB1 = Diff.Dot(Edge1);
			MWDREAL fC = Diff.GetSqrLength();
			MWDREAL fDet = ABS(fA00 * fA11 - fA01 * fA01);
			MWDREAL fS = fA01 * fB1 - fA11 * fB0;
			MWDREAL fT = fA01 * fB0 - fA00 * fB1;
			MWDREAL fSqrDistance;
			if (fS + fT <= fDet)
			{
				if (fS < 0.0f)
				{
					if (fT < 0.0f)  // region 4
					{
						if (fB0 < 0.0f)
						{
							fT = 0.0f;
							if (-fB0 >= fA00)
							{
								fS = 1.0f;
								fSqrDistance = fA00 + (2.0f) * fB0 + fC;
							}
							else
							{
								fS = -fB0 / fA00;
								fSqrDistance = fB0 * fS + fC;
							}
						}
						else
						{
							fS = 0.0f;
							if (fB1 >= 0.0f)
							{
								fT = 0.0f;
								fSqrDistance = fC;
							}
							else if (-fB1 >= fA11)
							{
								fT = 1.0f;
								fSqrDistance = fA11 + 2.0f * fB1 + fC;
							}
							else
							{
								fT = -fB1 / fA11;
								fSqrDistance = fB1 * fT + fC;
							}
						}
					}
					else  // region 3
					{
						fS = 0.0f;
						if (fB1 >= 0.0f)
						{
							fT = 0.0f;
							fSqrDistance = fC;
						}
						else if (-fB1 >= fA11)
						{
							fT = 1.0f;
							fSqrDistance = fA11 + 2.0f * fB1 + fC;
						}
						else
						{
							fT = -fB1 / fA11;
							fSqrDistance = fB1 * fT + fC;
						}
					}
				}
				else if (fT < 0.0f)  // region 5
				{
					fT = 0.0f;
					if (fB0 >= 0.0f)
					{
						fS = 0.0f;
						fSqrDistance = fC;
					}
					else if (-fB0 >= fA00)
					{
						fS = 1.0f;
						fSqrDistance = fA00 + 2.0f * fB0 + fC;
					}
					else
					{
						fS = -fB0 / fA00;
						fSqrDistance = fB0 * fS + fC;
					}
				}
				else  // region 0
				{
					MWDREAL fInvDet = 1.0f / fDet;
					fS *= fInvDet;
					fT *= fInvDet;
					fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
						fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
				}
			}
			else
			{
				MWDREAL fTmp0, fTmp1, fNumer, fDenom;
				if (fS < 0.0f)  // region 2
				{
					fTmp0 = fA01 + fB0;
					fTmp1 = fA11 + fB1;
					if (fTmp1 > fTmp0)
					{
						fNumer = fTmp1 - fTmp0;
						fDenom = fA00 - 2.0f * fA01 + fA11;
						if (fNumer >= fDenom)
						{
							fS = 1.0f;
							fT = 0.0f;
							fSqrDistance = fA00 + 2.0f * fB0 + fC;
						}
						else
						{
							fS = fNumer / fDenom;
							fT = 1.0f - fS;
							fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
								fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
						}
					}
					else
					{
						fS = 0.0f;
						if (fTmp1 <= 0.0f)
						{
							fT = 1.0f;
							fSqrDistance = fA11 + 2.0f * fB1 + fC;
						}
						else if (fB1 >= 0.0f)
						{
							fT = 0.0f;
							fSqrDistance = fC;
						}
						else
						{
							fT = -fB1 / fA11;
							fSqrDistance = fB1 * fT + fC;
						}
					}
				}
				else if (fT < 0.0f)  // region 6
				{
					fTmp0 = fA01 + fB1;
					fTmp1 = fA00 + fB0;
					if (fTmp1 > fTmp0)
					{
						fNumer = fTmp1 - fTmp0;
						fDenom = fA00 - 2.0f * fA01 + fA11;
						if (fNumer >= fDenom)
						{
							fT = 1.0f;
							fS = 0.0f;
							fSqrDistance = fA11 + 2.0f * fB1 + fC;
						}
						else
						{
							fT = fNumer / fDenom;
							fS = 1.0f - fT;
							fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
								fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
						}
					}
					else
					{
						fT = 0.0f;
						if (fTmp1 <= 0.0f)
						{
							fS = 1.0f;
							fSqrDistance = fA00 + 2.0f * fB0 + fC;
						}
						else if (fB0 >= 0.0f)
						{
							fS = 0.0f;
							fSqrDistance = fC;
						}
						else
						{
							fS = -fB0 / fA00;
							fSqrDistance = fB0 * fS + fC;
						}
					}
				}
				else  // region 1
				{
					fNumer = fA11 + fB1 - fA01 - fB0;
					if (fNumer <= 0.0f)
					{
						fS = 0.0f;
						fT = 1.0f;
						fSqrDistance = fA11 + 2.0f * fB1 + fC;
					}
					else
					{
						fDenom = fA00 - 2.0f * fA01 + fA11;
						if (fNumer >= fDenom)
						{
							fS = 1.0f;
							fT = 0.0f;
							fSqrDistance = fA00 + 2.0f * fB0 + fC;
						}
						else
						{
							fS = fNumer / fDenom;
							fT = 1.0f - fS;
							fSqrDistance = fS * (fA00 * fS + fA01 * fT + 2.0f * fB0) +
								fT * (fA01 * fS + fA11 * fT + 2.0f * fB1) + fC;
						}
					}
				}
			}
			if (fSqrDistance < 0.0f)
			{
				fSqrDistance = 0.0f;
			}
			fTriangleParameter[1] = fS;
			fTriangleParameter[2] = fT;
			fTriangleParameter[0] = 1 - fTriangleParameter[1] - fTriangleParameter[2];
			return fSqrDistance;
		};
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDRectangle3& Rectangle, MWDREAL fRectangleParameter[2]) {
			MWDVector3 Diff = Rectangle.GetCenter() - P1;
			MWDVector3 A[2];
			Rectangle.GetA(A);
			MWDREAL fA[2];
			Rectangle.GetfA(fA);
			MWDREAL fB0 = Diff.Dot(A[0]);
			MWDREAL fB1 = Diff.Dot(A[1]);
			MWDREAL fS = -fB0;
			MWDREAL fT = -fB1;
			MWDREAL fSqrDistance = Diff.GetSqrLength();
			if (fS < -fA[0])
			{
				fS = -fA[0];
			}
			else if (fS > fA[0])
			{
				fS = fA[0];
			}
			fSqrDistance += fS * (fS + 2.0f * fB0);
			if (fT < -fA[1])
			{
				fT = -fA[1];
			}
			else if (fT > fA[1])
			{
				fT = fA[1];
			}
			fSqrDistance += fT * (fT + 2.0f * fB1);
			if (fSqrDistance < 0.0f)
			{
				fSqrDistance = 0.0f;
			}
			fRectangleParameter[0] = fS;
			fRectangleParameter[1] = fT;
			return fSqrDistance;
		};
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDOBB3& OBB, MWDREAL fOBBParameter[3]) {
			return SquaredDistance(OBB ,P1, fOBBParameter);
		};
		static MWDREAL Distance(const MWDVector3& P1, const MWDSphere3& Sphere, MWDVector3& SpherePoint) {
			return Distance(Sphere ,P1, SpherePoint);
		};
		static MWDREAL Distance(const MWDVector3& P1, const MWDPlane3& Plane, MWDVector3& PlanePoint) {
			return Distance(Plane ,P1, PlanePoint);
		};
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDAABB3& AABB, MWDREAL fAABBParameter[3]) {
			return SquaredDistance(AABB ,P1, fAABBParameter);
		};
		static MWDREAL SquaredDistance(const MWDVector3& P1, const MWDPolygon3& Polygon, int& IndexTriangle, MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Polygon ,P1, IndexTriangle, fTriangleParameter);
		};
		static int RelationWith(const MWDVector3& P1, const MWDSphere3& Sphere) {
			MWDREAL sqrDist = SquaredDistance(P1,Sphere.m_Center);
			MWDREAL diff = sqrDist - Sphere.m_fRadius * Sphere.m_fRadius;
			if (diff > EPSILON_E4)
				return MWDOUT;
			else if (diff < -EPSILON_E4)
				return MWDIN;
			else
				return MWDON;
		};
		static int RelationWith(const MWDVector3& P1, const MWDPlane3& Plane) {
			return RelationWith(Plane ,P1);
		};
		static int RelationWith(const MWDVector3& P1, const MWDOBB3& OBB) {
			MWDMatrix3X3W m;
			GetTransform(OBB,m);
			MWDVector3 PointTemp = Vec3MulMat3x3W(P1, m); 
			MWDREAL fA[3];
			OBB.GetfA(fA);
			MWDREAL fDiffX = ABS(PointTemp.x) - fA[0];
			MWDREAL fDiffY = ABS(PointTemp.y) - fA[1];
			MWDREAL fDiffZ = ABS(PointTemp.z) - fA[2];
			if (fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
				return MWDOUT;
			if (fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
				return MWDIN;
			return MWDON;
		};
		static int RelationWith(const MWDVector3& P1, const MWDAABB3& AABB) {
			return RelationWith(AABB ,P1);
		};

		//Vector3W
		static FORCEINLINE void GetV3(const MWDVector3W& in, MWDVector3& out){
			out.x = in.x;
			out.y = in.y;
			out.z = in.z;
		};
		static MWDVector3W Vec3WMulMat3x3W(const MWDVector3W& v3w,const MWDMatrix3X3W& mat3x3w) {
			MWDVector3W vcResult;
			vcResult.x = v3w.x * mat3x3w._00 + v3w.y * mat3x3w._10 + v3w.z * mat3x3w._20 + v3w.w * mat3x3w._30;
			vcResult.y = v3w.x * mat3x3w._01 + v3w.y * mat3x3w._11 + v3w.z * mat3x3w._21 + v3w.w * mat3x3w._31;
			vcResult.z = v3w.x * mat3x3w._02 + v3w.y * mat3x3w._12 + v3w.z * mat3x3w._22 + v3w.w * mat3x3w._32;
			vcResult.w = v3w.x * mat3x3w._03 + v3w.y * mat3x3w._13 + v3w.z * mat3x3w._23 + v3w.w * mat3x3w._33;
			return vcResult;
		}
		
		//Quat
		static void QuatMulVec3  (const MWDQuat& q,const MWDVector3& v, MWDQuat& out) {
			MWDQuat& t = *new MWDQuat(v.x, v.y, v.z, 0);
			out.Set(q * t);
			MWDDelete(&t);
		};
		static void Rotate(const MWDVector3& v, const MWDQuat& q, MWDVector3& out) {
			MWDQuat qq = q;
			qq.Normalize();
			MWDQuat tmp ;
			MWD::QuatMulVec3(qq.GetConjugate(), v, tmp);
			MWDQuat r =  tmp * qq;
			out.Set(r.x, r.y, r.z);
		};
		static void CreateAxisAngle(MWDQuat& inout,const MWDVector3& Axis, MWDREAL fAngle) {
			MWDREAL fTemp = fAngle / 2.0f;
			MWDREAL Sin = SIN(fTemp);
			MWDREAL Cos = COS(fTemp);

			MWDVector3 N = Axis;
			N.Normalize();

			inout.w = Cos;
			inout.x = Sin * N.x;
			inout.y = Sin * N.y;
			inout.z = Sin * N.z;
		};
		static void  CreateEuler(MWDQuat& inout, MWDREAL fRoll, MWDREAL fPitch, MWDREAL fYaw) {
			MWDREAL cX, cY, cZ, sX, sY, sZ, cXcZ, sXsZ, cXsZ, sXcZ;

			fPitch *= 0.5f;
			fYaw *= 0.5f;
			fRoll *= 0.5f;

			cX = COS(fPitch);
			cY = COS(fYaw);
			cZ = COS(fRoll);

			sX = SIN(fPitch);
			sY = SIN(fYaw);
			sZ = SIN(fRoll);

			cXcZ = cX * cZ;
			sXsZ = sX * sZ;
			cXsZ = cX * sZ;
			sXcZ = sX * cZ;

			inout.w = cXcZ * cY + sXsZ * sY;
			inout.x = sXcZ * cY + cXsZ * sY;
			inout.y = cXcZ * sY - sXsZ * cY;
			inout.z = cXsZ * cY - sXcZ * sY;
		};
		static MWDREAL Dot(const MWDQuat& p,const MWDQuat& q){
			return (p.w * q.w + p.x * q.x + p.y * q.y + p.z * q.z);
		};
		static void Rotate(MWDQuat& inout,const MWDQuat& q1, const MWDQuat& q2) {
			MWDQuat m1 = q1;
			MWDQuat m2 =q2;
			m1.Normalize();
			m2.Normalize();
			MWDQuat t = (m1.GetConjugate()) * m2 * m1;
			inout.x = t.x; inout.y = t.y; inout.z = t.z; inout.w = t.w;
		};
		static void Slerp(MWDQuat& inout, const MWDQuat& q1, const MWDQuat& q2, MWDREAL t) {
			if (t <= 0)
			{
				(inout) = q1;
				return;
			}
			if (t >= 1)
			{
				(inout) = q2;
				return;
			}
			MWDQuat q1Temp =q1;
			MWDQuat q2Temp =q2;
			q1Temp.Normalize();
			q2Temp.Normalize();

			MWDREAL fCos = MWD::Dot(q1Temp, q2Temp);
			if (fCos < 0.0f)
			{
				q1Temp *= -1.0f;
				fCos = -fCos;
			}
			MWDREAL k1, k2;
			if (fCos > MWDREAL_1)
			{
				k1 = 1.0f - t;
				k2 = t;
			}
			else
			{
				MWDREAL fSin = SQRT(1.0f - fCos * fCos);
				MWDREAL fAngle = ATAN2(fSin, fCos);
				MWDREAL fInMWDin = 1.0f / fSin;
				k1 = SIN((1.0f - t) * fAngle) * fInMWDin;
				k2 = SIN(t * fAngle) * fInMWDin;
			}
			inout.x = k1 * q1Temp.x + k2 * q2Temp.x;
			inout.y = k1 * q1Temp.y + k2 * q2Temp.y;
			inout.z = k1 * q1Temp.z + k2 * q2Temp.z;
			inout.w = k1 * q1Temp.w + k2 * q2Temp.w;
		};
		static void TriangleSlerp(MWDQuat& inout, MWDREAL t1, MWDREAL t2, const MWDQuat& q1, const MWDQuat& q2, const MWDQuat& q3) {
			MWDQuat q1Temp =q1;
			MWDQuat q2Temp =q2;
			MWDQuat q3Temp =q3;
			q1Temp.Pow(t1);
			q2Temp.Pow(t2);
			q3Temp.Pow(1.0f - t1 - t2);
			inout.Set(q1Temp+ q2Temp+ q3Temp);
		};
		static void Slerp(MWDQuat& inout, const MWDQuat& q1, const MWDQuat& q2, const MWDQuat& s1, const MWDQuat& s2, MWDREAL t) {
			MWDREAL fSlerpT = 2.0f * t * (1.0f - t);
			MWDQuat SlerpP ;
			MWDQuat SlerpQ ;
			MWD::Slerp(SlerpP, q1, q2,t);
			MWD::Slerp(SlerpQ, s1, s2, t);
			MWD::Slerp(inout, SlerpP, SlerpQ,fSlerpT);
		};
		static void SlerpSValueOf(MWDQuat& inout, const MWDQuat& q1, const MWDQuat& q2, const MWDQuat& q3) {
			//MWDQuat& q2Inv = q2.GetInverse();
			MWDQuat K1 = q1 * q2;
			MWDQuat K2 = q3 * q2;
			K1.Ln();
			K2.Ln();
			MWDQuat K3 = (K1+K2) * (-0.25f);
			K3.Exp();
			MWDQuat tmp = q2 * K3;
			inout.Set(tmp);
		};

		//Matrix3x3
		//生成相机矩阵V
		static void CreateFromDirection(MWDMatrix3X3& out, const MWDVector3& Direction, const MWDVector3& Up = MWDVector3(0, 1, 0)) {
			MWDVector3 vcTemp, vcUp,Dir = Direction;
			Dir.Normalize();

			MWDREAL fDot = MWD::Dot(Up,Dir); 
			vcTemp = Dir * fDot;
			vcUp = Up - vcTemp;

			MWDREAL fL = vcUp.GetLength();

			if (fL < EPSILON_E4)
			{
				MWDVector3 vcY;
				vcY.Set(0.0f, 1.0f, 0.0f);

				vcTemp = Dir * Direction.y;
				vcUp = vcY - vcTemp;

				fL = vcUp.GetLength();

				if (fL < EPSILON_E4)
				{
					vcY.Set(0.0f, 0.0f, 1.0f);

					vcTemp = Dir * Dir.z;
					vcUp = vcY - vcTemp;


					fL = vcUp.GetLength();
					if (fL < EPSILON_E4) return;
				}
			}
			vcUp /= fL;
			MWDVector3 vcRight;
			Dir = MWD::Cross(vcRight,vcUp);
			MWD::CreateRot(out,vcRight, vcUp, Dir);
		};
		static void CreateRotX(MWDMatrix3X3& out, MWDREAL a) {
			MWDREAL fCos = COS(a);
			MWDREAL fSin = SIN(a);

			out._11 = fCos;
			out._12 = fSin;
			out._21 = -fSin;
			out._22 = fCos;

			out._00 = 1.0f;
			out._01 = out._02 = out._10 = out._20 = 0.0f;
		};
		static void CreateRotY(MWDMatrix3X3& out, MWDREAL a) {
			MWDREAL fCos = COS(a);
			MWDREAL fSin = SIN(a);

			out._00 = fCos;
			out._02 = -fSin;
			out._20 = fSin;
			out._22 = fCos;

			out._11 = 1.0f;
			out._01 = out._12 = out._10 = out._21 = 0.0f;
		};
		static void CreateRotZ(MWDMatrix3X3& out, MWDREAL a) {
			MWDREAL fCos = COS(a);
			MWDREAL fSin = SIN(a);

			out._00 = fCos;
			out._01 = fSin;
			out._10 = -fSin;
			out._11 = fCos;

			out._22 = 1.0f;
			out._02 = out._12 = out._20 = out._21 = 0.0f;
		};
		static void CreateEluer(MWDMatrix3X3& out, MWDREAL Roll, MWDREAL Pitch, MWDREAL Yaw) {
			MWDREAL SX, SY, SZ, CX, CY, CZ;

			SZ = SIN(Roll);
			CZ = COS(Roll);
			SY = SIN(Yaw);
			CY = COS(Yaw);
			SX = SIN(Pitch);
			CX = COS(Pitch);

			out._00 = CY * CZ + SX * SY * SZ;
			out._01 = CX * SZ;
			out._02 = -SY * CZ + SX * CY * SZ;
			out._10 = -CY * SZ + SX * SY * CZ;
			out._11 = CX * CZ;
			out._12 = SY * SZ + SX * CY * CZ; ;
			out._20 = CX * SY;
			out._21 = -SX;
			out._22 = CX * CY;
		};
		// -pi <= y z <= pi  -pi/2 < x < pi/2
		//转轴+旋转角度a
		static void CreateAxisAngle(MWDMatrix3X3& out, const MWDVector3& _vcAxis, MWDREAL a) {
			MWDVector3 vcAxis = _vcAxis;
			MWDREAL fCos = COS(a);
			MWDREAL fSin = SIN(a);
			MWDREAL fSum = 1.0f - fCos;

			if (vcAxis.GetSqrLength() != 1.0)
				vcAxis.Normalize();

			MWDREAL xx = vcAxis.x * vcAxis.x;
			MWDREAL yy = vcAxis.y * vcAxis.y;
			MWDREAL zz = vcAxis.z * vcAxis.z;

			MWDREAL xy = vcAxis.x * vcAxis.y;
			MWDREAL yz = vcAxis.y * vcAxis.z;
			MWDREAL xz = vcAxis.z * vcAxis.x;

			MWDREAL zs = vcAxis.z * fSin;
			MWDREAL ys = vcAxis.y * fSin;
			MWDREAL xs = vcAxis.x * fSin;

			out._00 = xx * fSum + fCos;
			out._01 = xy * fSum + zs;
			out._02 = xz * fSum - ys;

			out._10 = xy * fSum - zs;
			out._11 = yy * fSum + fCos;
			out._12 = yz * fSum + xs;

			out._20 = xz * fSum + ys;
			out._21 = yz * fSum - xs;
			out._22 = zz * fSum + fCos;
		};
		static void CreateRot(MWDMatrix3X3& out, const MWDVector3& U, const MWDVector3& V, const MWDVector3& N) {
			out._00 = U.x;
			out._01 = U.y;
			out._02 = U.z;

			out._10 = V.x;
			out._11 = V.y;
			out._12 = V.z;

			out._20 = N.x;
			out._21 = N.y;
			out._22 = N.z;
		};
		static void CreateScale(MWDMatrix3X3& out, MWDREAL fX, MWDREAL fY, MWDREAL fZ) {
			out.Identity();
			out._00 = fX;
			out._11 = fY;
			out._22 = fZ;
		};
		static void CreateScale(MWDMatrix3X3& out,const MWDVector3& Axis, MWDREAL fScale) {
			MWDVector3 NorAxis = Axis;
			NorAxis.Normalize();
			MWDREAL a = fScale - 1.0f;
			MWDREAL ax = a * NorAxis.x;
			MWDREAL ay = a * NorAxis.y;
			MWDREAL az = a * NorAxis.z;

			out.M[0][0] = ax * NorAxis.x - 1.0f;
			out.M[1][1] = ay * NorAxis.y - 1.0f;
			out.M[2][2] = az * NorAxis.z - 1.0f;

			out.M[0][1] = out.M[1][0] = ax * NorAxis.y;
			out.M[0][2] = out.M[2][0] = ax * NorAxis.z;
			out.M[1][2] = out.M[2][1] = ay * NorAxis.z;
		};
		//矩阵线性插值M2 * t + M1 * (1.0f - t)————————不准确
		static void LineInterpolation(MWDMatrix3X3& out, MWDREAL t, const MWDMatrix3X3& M1, const MWDMatrix3X3& M2) {
			out = M2 * t + M1 * (1.0f - t);
		};
		//球面线性插值
		static void Slerp(MWDMatrix3X3& out, MWDREAL t, const MWDMatrix3X3& M1, const MWDMatrix3X3& M2) {
			MWDMatrix3X3  M1Transpose, Temp;
			M1Transpose.TransposeOf(M1);
			Temp = M1Transpose * M2;
			MWDREAL fAnagle;
			MWDVector3 Axis;
			Temp.GetAxisAngle(Axis, fAnagle);
			Temp.Identity();
			MWD::CreateAxisAngle(Temp,Axis, fAnagle * t);
			out = M1 * Temp;
		};
		static MWDQuat GetQuat(const MWDMatrix3X3&mat) {
			MWDREAL x, y, z, w;
			MWDREAL fTemp[4];
			int iflag = 0;
			fTemp[0] = mat._00 + mat._11 + mat._22;
			fTemp[1] = mat._00 - mat._11 - mat._22;
			fTemp[2] = mat._11 - mat._00 - mat._22;
			fTemp[3] = mat._22 - mat._00 - mat._11;
			MWDREAL fBig = fTemp[0];
			for (int i = 1; i < 4; i++)
			{
				if (fBig < fTemp[i])
				{
					fBig = fTemp[i];
					iflag = i;
				}
			}
			fBig = SQRT(fBig + 1.0f) * 0.5f;
			MWDREAL fMult = 0.25f / fBig;
			switch (iflag)
			{
			case 0:
				w = fBig;
				x = (mat._12 - mat._21) * fMult;
				y = (mat._20 - mat._02) * fMult;
				z = (mat._01 - mat._10) * fMult;
				break;
			case 1:
				x = fBig;
				w = (mat._12 - mat._21) * fMult;
				y = (mat._01 + mat._10) * fMult;
				z = (mat._20 + mat._02) * fMult;
				break;
			case 2:
				y = fBig;
				w = (mat._20 - mat._02) * fMult;
				x = (mat._01 + mat._10) * fMult;
				z = (mat._12 + mat._21) * fMult;
				break;
			case 3:
				z = fBig;
				w = (mat._01 - mat._10) * fMult;
				x = (mat._20 + mat._02) * fMult;
				y = (mat._12 + mat._21) * fMult;
				break;
			}
			return MWDQuat(x, y, z, w);
		};
		static MWDVector3 Mat3x3MulVec3 (const MWDMatrix3X3& mat, const MWDVector3& vc) {
			return MWDVector3(
				mat._00 * vc.x + mat._01 * vc.y + mat._02 * vc.z,
				mat._10 * vc.x + mat._11 * vc.y + mat._12 * vc.z,
				mat._20 * vc.x + mat._21 * vc.y + mat._22 * vc.z);
		};

		//Matrix3x3W
		//void CreatePointLightPlaneShadow(const MWDVector3 & LightPoint,const MWDPlane3 &Plane);
		//void CreateInfiniteLightPlaneShadow(const MWDVector3 & LightDir,const MWDPlane3 &Plane);
		static MWDVector3W Mat3x3WMulVec3(const MWDMatrix3X3W& mat3x3w, const MWDVector3W& v3w) {
			MWDVector3W vcResult;
			vcResult.x = v3w.x * mat3x3w._00 + v3w.y * mat3x3w._01 + v3w.z * mat3x3w._02 + v3w.w * mat3x3w._03;
			vcResult.y = v3w.x * mat3x3w._10 + v3w.y * mat3x3w._11 + v3w.z * mat3x3w._12 + v3w.w * mat3x3w._13;
			vcResult.z = v3w.x * mat3x3w._20 + v3w.y * mat3x3w._21 + v3w.z * mat3x3w._22 + v3w.w * mat3x3w._23;
			vcResult.w = v3w.x * mat3x3w._30 + v3w.y * mat3x3w._31 + v3w.z * mat3x3w._32 + v3w.w * mat3x3w._33;
			return vcResult;
		}

		//Line3
		static void Transform(MWDLine3& out, const MWDLine3& in, const MWDMatrix3X3W& Mat) {
			MWDVector3 vec3 = Mat.Apply3X3(in.GetDir());
			MWDVector3 tmp;
			out.SetDir(vec3);
			tmp  = MWD::Vec3MulMat3x3W(in.GetOrig(),Mat);
			out.SetOrig(tmp);
		};
		static bool GetParameter(const MWDLine3& L,const MWDVector3& Point, MWDREAL& fLineParameter) {
			MWDVector3 Temp = Point -L.GetOrig();
			if (MWD::IsParallel(Temp,L.GetDir()))
			{
				fLineParameter = Temp.GetLength();
				return 1;
			}
			else
				return 0;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L, const MWDVector3& Point, MWDREAL& fLineParameter) {
			MWDVector3 Diff = Point -L.GetOrig();
			MWDVector3 m_Dir = L.GetDir();
			fLineParameter = MWD::Dot(m_Dir, Diff);

			Diff -= m_Dir * fLineParameter;

			return Diff.GetSqrLength();
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDLine3& L2, MWDREAL& fLine1Parameter, MWDREAL& fLine2Parameter) {
			MWDREAL SquareDis;
			MWDVector3 Diff =L1.GetOrig() - L2.GetOrig();
			MWDREAL a01 = -(MWD::Dot(L1.GetDir(), L2.GetDir()));
			MWDREAL b0 = MWD::Dot(L1.GetDir(), Diff);
			MWDREAL c = Diff.GetSqrLength();
			MWDREAL det = 1 - a01 * a01;

			if (ABS(det) >= EPSILON_E4)
			{
				MWDREAL b1 = -(MWD::Dot(L2.GetDir(), Diff));
				MWDREAL invDet = 1 / det;
				fLine1Parameter = (a01 * b1 - b0) * invDet;
				fLine2Parameter = (a01 * b0 - b1) * invDet;
				SquareDis = fLine1Parameter * (fLine1Parameter + a01 * fLine2Parameter + 2 * b0) + fLine2Parameter * (a01 * fLine1Parameter + fLine2Parameter + 2 * b1) + c;

			}
			else
			{
				fLine1Parameter = -b0;
				fLine2Parameter = 0;

				SquareDis = b0 * fLine1Parameter + c;
			}
			return SquareDis;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDRay3& Ray, MWDREAL& fLineParameter, MWDREAL& fRayParameter) {
			const MWDLine3 Line = Ray;
			MWDREAL sqrDist = SquaredDistance(Line,L1, fLineParameter, fRayParameter);

			if (fRayParameter < 0)
			{
				fRayParameter = 0;
				sqrDist = SquaredDistance(L1,Ray.GetOrig(), fLineParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDSegment3& Segment, MWDREAL& fLineParameter, MWDREAL& fSegmentParameter) {
			const MWDRay3 Ray = Segment;
			MWDREAL sqrDist = SquaredDistance(L1,Ray, fLineParameter, fSegmentParameter);
			MWDREAL fLen = Segment.GetLen();

			if (fSegmentParameter > fLen)
			{
				MWDVector3 End = Segment.GetEnd();
				fSegmentParameter = fLen;
				sqrDist = SquaredDistance(L1,End, fLineParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDTriangle3& Triangle, MWDREAL& fLineParameter, MWDREAL fTriangleParameter[3]) {
			MWDVector3 TrianglePoint[3];
			Triangle.GetPoint(TrianglePoint);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDSegment3 Edge;
			MWDREAL Tempt0, Tempt1;
			Edge.Set(TrianglePoint[0], TrianglePoint[1]);
			fSqrDistTmp = SquaredDistance(L1,Edge, Tempt0, Tempt1);

			if (fSqrDistTmp < fSqrDist)
			{


				fLineParameter = Tempt0;

				fTriangleParameter[1] = Tempt1 / Edge.GetLen();
				fTriangleParameter[0] = 1.0f - fTriangleParameter[1];
				fTriangleParameter[2] = 0.0f;

				fSqrDist = fSqrDistTmp;
			}


			Edge.Set(TrianglePoint[0], TrianglePoint[2]);
			fSqrDistTmp = SquaredDistance(L1,Edge, Tempt0, Tempt1);

			if (fSqrDistTmp < fSqrDist)
			{


				fLineParameter = Tempt0;

				fTriangleParameter[1] = 0.0f;
				fTriangleParameter[2] = Tempt1 / Edge.GetLen();
				fTriangleParameter[0] = 1.0f - fTriangleParameter[2];


				fSqrDist = fSqrDistTmp;
			}

			Edge.Set(TrianglePoint[1], TrianglePoint[2]);
			fSqrDistTmp = SquaredDistance(L1,Edge, Tempt0, Tempt1);

			if (fSqrDistTmp < fSqrDist)
			{


				fLineParameter = Tempt0;

				fTriangleParameter[1] = Tempt1 / Edge.GetLen();
				fTriangleParameter[2] = 1.0f - fTriangleParameter[1];
				fTriangleParameter[0] = 0.0f;


				fSqrDist = fSqrDistTmp;
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDRectangle3& Rectangle, MWDREAL& fLineParameter, MWDREAL fRectangleParameter[2]) {
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDVector3 r[4];

			MWDVector3 A[2];
			Rectangle.GetA(A);
			MWDREAL fA[2];
			Rectangle.GetfA(fA);


			MWDVector3 Temp0 = Rectangle.GetCenter() + A[0] * fA[0];
			MWDVector3 Temp1 = Rectangle.GetCenter() + A[0] * (-fA[0]);
			r[0] = Temp0 + A[1] * fA[1];
			r[1] = Temp0 + A[1] * (-fA[1]);
			r[2] = Temp1 + A[1] * (-fA[1]);
			r[3] = Temp1 + A[1] * fA[1];

			MWDSegment3 Edge;
			Edge.Set(r[0], r[1]);

			MWDREAL fT0, fT1;
			fSqrDistTmp = SquaredDistance(L1,Edge, fT0, fT1);

			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fLineParameter = fT0;
				fRectangleParameter[0] = fA[0];
				fRectangleParameter[1] = fT1 - fA[1];
			}
			Edge.Set(r[2], r[1]);

			fSqrDistTmp = SquaredDistance(L1,Edge, fT0, fT1);

			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fLineParameter = fT0;
				fRectangleParameter[0] = fA[1];
				fRectangleParameter[1] = fT1 - fA[0];
			}
			Edge.Set(r[3], r[2]);

			fSqrDistTmp = SquaredDistance(L1,Edge, fT0, fT1);

			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fLineParameter = fT0;
				fRectangleParameter[0] = -fA[0];
				fRectangleParameter[1] = fT1 - fA[1];
			}
			Edge.Set(r[3], r[0]);
			fSqrDistTmp = SquaredDistance(L1,Edge, fT0, fT1);

			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fLineParameter = fT0;
				fRectangleParameter[0] = -fA[1];
				fRectangleParameter[1] = fT1 - fA[0];
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDOBB3& OBB, MWDREAL& fLineParameter, MWDREAL fOBBParameter[3]) {
			return SquaredDistance(OBB ,L1, fOBBParameter, fLineParameter);
		};
		static MWDREAL Distance(const MWDLine3& L1, const MWDSphere3& Sphere, MWDREAL& fLineParameter, MWDVector3& SpherePoint) {
			return Distance(Sphere ,L1, SpherePoint, fLineParameter);
		};
		static MWDREAL Distance(const MWDLine3& L1, const MWDPlane3& Plane, MWDVector3& LinePoint, MWDVector3& PlanePoint) {
			return Distance(Plane ,L1, PlanePoint, LinePoint);
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDAABB3& AABB, MWDREAL& fLineParameter, MWDREAL fAABBParameter[3]) {
			return SquaredDistance(AABB ,L1, fAABBParameter, fLineParameter);
		};
		static MWDREAL SquaredDistance(const MWDLine3& L1, const MWDPolygon3& Polygon, MWDREAL& fLineParameter,int& IndexTriangle,MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Polygon ,L1, IndexTriangle, fTriangleParameter, fLineParameter);
		};
		static int RelationWith(const MWDLine3& L1, const MWDRectangle3& Rectangle, bool bCull, MWDREAL& fLineParameter, MWDREAL fRectangleParameter[2]) {
			MWDPlane3 Plane = Rectangle.GetPlane();
			if (bCull)
			{
				if (RelationWith(L1.GetOrig(),Plane) == MWDBACK)
					return MWDNOINTERSECT;
			}
			if (RelationWith(L1,Plane, bCull, fLineParameter) == MWDINTERSECT)
			{
				MWDVector3 Point = L1.GetParameterPoint(fLineParameter);
				if (GetParameter(Rectangle,Point, fRectangleParameter))
				{
					return MWDINTERSECT;
				}
			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDLine3& L1, const MWDSphere3& sphere, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			MWDVector3 Delta =L1.GetOrig() - sphere.m_Center;
			MWDREAL a0 = Delta.Dot(Delta) - sphere.m_fRadius * sphere.m_fRadius;
			MWDREAL a1 = Dot(Delta,L1.GetDir());
			MWDREAL Disc = a1 * a1 - a0;
			if (Disc < 0)
			{
				Quantity = 0;
			}
			else if (Disc >= EPSILON_E4)
			{
				MWDREAL root = SQRT(Disc);
				tFar = -a1 + root;
				tNear = -a1 - root;
				Quantity = 2;
			}
			else
			{
				tNear = -a1;
				tFar = -a1;
				Quantity = 1;
			}
			if (Quantity == 0)
				return MWDNOINTERSECT;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDLine3& L1, const MWDOBB3& OBB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			MWDREAL e, f, t1, t2, temp;
			MWDREAL	tmin = -MWDMAX_REAL,
				tmax = MWDMAX_REAL;
			MWDVector3 A[3];
			MWDREAL fA[3];
			MWDVector3 vcP = OBB.GetCenter() -L1.GetOrig();
			OBB.GetA(A);
			OBB.GetfA(fA);
			for (unsigned int i = 0; i < 3; i++)
			{
				e = A[i].Dot(vcP);
				f = A[i].Dot(L1.GetDir());
				if (ABS(f) > EPSILON_E4)
				{
					t1 = (e + fA[i]) / f;
					t2 = (e - fA[i]) / f;
					if (t1 > t2) { temp = t1; t1 = t2; t2 = temp; }
					if (t1 > tmin) tmin = t1;
					if (t2 < tmax) tmax = t2;
					if (tmin > tmax) return MWDNOINTERSECT;
					if (tmax < 0.0f) return MWDNOINTERSECT;
				}
				else if (((-e - fA[i]) > 0.0f) || ((-e + fA[i]) < 0.0f))
					return MWDNOINTERSECT;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDLine3& L1, const MWDAABB3& AABB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			MWDREAL t0, t1, tmp;
			tNear = -MWDMAX_REAL;
			tFar = MWDMAX_REAL;
			MWDVector3 MaxT;
			MWDVector3 Min = AABB.GetMinPoint();
			MWDVector3 Max = AABB.GetMaxPoint();
			for (int i = 0; i < 3; i++)
			{
				if (ABS(L1.GetDir().m[i]) < EPSILON_E4)
				{
					if ((L1.GetDir().m[i] < Min.m[i]) ||
						(L1.GetDir().m[i] > Max.m[i]))
						return MWDNOINTERSECT;
				}
				t0 = (Min.m[i] - L1.GetDir().m[i]) / L1.GetDir().m[i];
				t1 = (Max.m[i] - L1.GetDir().m[i]) / L1.GetDir().m[i];
				if (t0 > t1) { tmp = t0; t0 = t1; t1 = tmp; }
				if (t0 > tNear) tNear = t0;
				if (t1 < tFar)  tFar = t1;
				if (tNear > tFar) return MWDNOINTERSECT;
				if (tFar < 0) return MWDNOINTERSECT;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDLine3& L1, const MWDPolygon3& Polygon, MWDREAL& fLineParameter, bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3]) {
			return RelationWith(Polygon ,L1, bCull, iIndexTriangle, fTriangleParameter, fLineParameter);
		};
		static int RelationWith(const MWDLine3& L1, const MWDTriangle3& Triangle, bool bCull, MWDREAL& fLineParameter,MWDREAL fTriangleParameter[3]) {
			MWDVector3 pvec, tvec, qvec;
			MWDVector3 TrianglePoint[3];
			Triangle.GetPoint(TrianglePoint);
			MWDVector3 edge1 = TrianglePoint[1] - TrianglePoint[0];
			MWDVector3 edge2 = TrianglePoint[2] - TrianglePoint[0];

			pvec.Cross(L1.GetDir(), edge2);

			MWDREAL det = edge1.Dot(pvec);
			if ((bCull) && (det < EPSILON_E4))
				return MWDNOINTERSECT;
			else if ((det < EPSILON_E4) && (det > -EPSILON_E4))
				return MWDNOINTERSECT;

			MWDREAL f_det = 1.0f / det;
			tvec =L1.GetOrig() - TrianglePoint[0];
			MWDREAL u = (tvec.Dot(pvec)) * f_det;
			if (u < 0.0f || u > 1)
				return MWDNOINTERSECT;

			qvec.Cross(tvec, edge1);
			MWDVector3 tmp_dir = L1.GetDir();
			MWDREAL v = (tmp_dir.Dot(qvec)) * f_det;
			if (v < 0.0f || u + v > 1)
				return MWDNOINTERSECT;

			MWDREAL f = (edge2.Dot(qvec)) * f_det;

			fTriangleParameter[1] = u;
			fTriangleParameter[2] = v;
			fTriangleParameter[0] = 1.0f - u - v;
			fLineParameter = f;


			return MWDINTERSECT;
		};
		static int RelationWith(const MWDLine3& L1, const MWDPlane3& Plane, bool bCull, MWDREAL& fLineParameter) {
			MWDVector3 tmp = Plane.GetN();
			MWDREAL Vd = tmp.Dot(L1.GetDir());

			if (ABS(Vd) < EPSILON_E4)
			{
				return RelationWith(L1.GetOrig(),Plane);
			}

			if (bCull && (Vd > 0.0f))
				return MWDNOINTERSECT;
			MWDVector3 plane_Normal = Plane.GetN();
			MWDREAL Vo = -((plane_Normal.Dot(L1.GetOrig())) + Plane.GetfD());

			MWDREAL _t = Vo / Vd;


			fLineParameter = _t;

			return MWDINTERSECT;
		};


		//Ray3
		static bool GetParameter(const MWDRay3& L, const MWDVector3& Point, MWDREAL& fRayParameter) {
			if (!MWD::GetParameter(L,Point, fRayParameter))
				return 0;

			if (fRayParameter < 0)
				return 0;
			return 1;
		};
		static void Transform(MWDLine3& out, const MWDRay3& Ray, const MWDMatrix3X3W& Mat) {
			MWD::Transform(out,Ray, Mat);
		};
		#pragma region MyRegion
		static int RelationWith(const MWDRay3& R, const MWDTriangle3& Triangle, bool bCull, MWDREAL& fRayParameter,MWDREAL fTriangleParameter[3]) {
			if (RelationWith(R,Triangle, bCull, fRayParameter, fTriangleParameter) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (fRayParameter < 0.0f)
				return MWDNOINTERSECT;
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDPlane3& Plane, bool bCull, MWDREAL& fRayParameter) {
			int iFlag = RelationWith(R,Plane, bCull, fRayParameter);
			if (iFlag != MWDINTERSECT)
				return iFlag;
			if (fRayParameter < 0.0f)
			{
				iFlag = RelationWith(R.GetOrig(),Plane);
				return iFlag;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDRectangle3& Rectangle, bool bCull, MWDREAL& fRayParameter,MWDREAL fRectangleParameter[2]) {
			if (RelationWith(R,Rectangle, bCull, fRayParameter, fRectangleParameter) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (fRayParameter < 0.0f)
				return MWDNOINTERSECT;

			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDOBB3& OBB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(R,OBB, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tFar < 0.0f)
			{
				Quantity = 0;
				return MWDNOINTERSECT;
			}
			if (tNear < 0.0f)
			{
				Quantity = 1;
				tNear = tFar;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDAABB3& AABB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(R,AABB, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tFar < 0.0f)
			{
				Quantity = 0;
				return MWDNOINTERSECT;
			}
			if (tNear < 0.0f)
			{
				Quantity = 1;
				tNear = tFar;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDSphere3& sphere, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(R,sphere, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tFar < 0.0f)
			{
				Quantity = 0;
				return MWDNOINTERSECT;
			}
			if (tNear < 0.0f)
			{
				Quantity = 1;
				tNear = tFar;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRay3& R, const MWDPolygon3& Polygon, MWDREAL& fRayParameter,bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3]) {
			return RelationWith(Polygon,R, bCull, iIndexTriangle, fTriangleParameter, fRayParameter);
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDVector3& Point, MWDREAL& fLineParameter) {
			MWDREAL fSqDis = SquaredDistance(R,Point, fLineParameter);
			if (fLineParameter >= 0)
				return fSqDis;
			else
			{
				fLineParameter = 0;
				MWDVector3 Diff;
				Diff = Point - R.GetOrig();
				return Diff.GetSqrLength();
			}
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDLine3& Line, MWDREAL& fRayParameter, MWDREAL& fLineParameter) {
			return SquaredDistance(Line,R, fLineParameter, fRayParameter);
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDRay3& Ray, MWDREAL& fRay1Parameter, MWDREAL& fRay2Parameter) {
			const MWDLine3 Line = Ray;
			MWDREAL sqrDist = SquaredDistance(R,Line, fRay1Parameter, fRay2Parameter);
			if (fRay2Parameter < 0)
			{
				fRay2Parameter = 0;
				sqrDist = SquaredDistance(R,Ray.GetOrig(), fRay1Parameter);
			}

			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDSegment3& Segment, MWDREAL& fRayParameter, MWDREAL& fSegmentParameter) {
			const MWDRay3 Ray = Segment;
			MWDREAL sqrDist = SquaredDistance(R,Ray, fRayParameter, fSegmentParameter);
			MWDREAL fLen = Segment.GetLen();
			if (fSegmentParameter > fLen)
			{
				fSegmentParameter = fLen;
				MWDVector3 End = Segment.GetEnd();
				sqrDist = SquaredDistance(R,End, fRayParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDTriangle3& Triangle, MWDREAL& fRayParameter, MWDREAL fTriangleParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(R,Triangle, fRayParameter, fTriangleParameter);
			if (fRayParameter < 0)
			{
				fRayParameter = 0;
				sqrDist = SquaredDistance(R.GetOrig(),Triangle, fTriangleParameter);
			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDRectangle3& Rectangle, MWDREAL& fRayParameter, MWDREAL fRectangleParameter[2]) {
			MWDREAL sqrDist = SquaredDistance(R,Rectangle, fRayParameter, fRectangleParameter);
			if (fRayParameter < 0)
			{
				fRayParameter = 0;
				sqrDist = SquaredDistance(R.GetOrig(),Rectangle, fRectangleParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDOBB3& OBB, MWDREAL& fRayParameter, MWDREAL fOBBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(R,OBB, fRayParameter, fOBBParameter);
			if (fRayParameter < 0)
			{
				fRayParameter = 0;
				sqrDist = SquaredDistance(OBB,R.GetOrig(), fOBBParameter);
			}
			return sqrDist;
		};
		static MWDREAL Distance(const MWDRay3& R, const MWDSphere3& Sphere, MWDREAL& fRayParameter, MWDVector3& SpherePoint) {
			return Distance(Sphere,R, SpherePoint, fRayParameter);
		};
		static MWDREAL Distance(const MWDRay3& R, const MWDPlane3& Plane, MWDVector3& RayPoint, MWDVector3& PlanePoint) {
			return Distance(Plane,R, PlanePoint, RayPoint);
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDAABB3& AABB, MWDREAL& fRayParameter, MWDREAL fAABBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(R,AABB, fRayParameter, fAABBParameter);
			if (fRayParameter < 0)
			{
				fRayParameter = 0;
				sqrDist = SquaredDistance(AABB,R.GetOrig(), fAABBParameter);

			}


			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRay3& R, const MWDPolygon3& Polygon, MWDREAL& fRayParameter,int& IndexTriangle,MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Polygon,R, IndexTriangle, fTriangleParameter, fRayParameter);
		};
#pragma endregion

		//Segment3
		static bool GetParameter(const MWDSegment3& L, const MWDVector3& Point, MWDREAL& fSegmentParameter) {
			if (!MWD::GetParameter(L,Point, fSegmentParameter))
				return 0;
			if (fSegmentParameter >L.GetLen())
				return 0;
			return 1;
		};
		static void Transform( MWDSegment3& L, const MWDSegment3& Segment, const MWDMatrix3X3W& Mat) {
			MWDVector3 v3 = Mat * Segment.GetOrig();
			L.SetOrig(v3);
			MWDVector3 end = Mat * Segment.GetEnd();
			L.SetEnd(end);
			MWDVector3 Dir = Mat.Apply3X3(Segment.GetDir());
			L.SetDir(Dir);
			L.SetLen(Segment.GetLen());
		};
		#pragma region MyRegion
		static int RelationWith(const MWDSegment3& S, const MWDTriangle3& Triangle, bool bCull, MWDREAL& fSegmentParameter,MWDREAL fTriangleParameter[3]) {
			if (RelationWith(S,Triangle, bCull, fSegmentParameter, fTriangleParameter) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (fSegmentParameter >S.GetLen())
				return MWDNOINTERSECT;
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDPlane3& Plane, bool bCull, MWDREAL& fSegmentParameter) {
			int iFlag = RelationWith(S,Plane, bCull, fSegmentParameter);
			if (iFlag != MWDINTERSECT)
				return iFlag;
			if (fSegmentParameter >S.GetLen())
			{
				return RelationWith(S.GetOrig(),Plane);
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDRectangle3& Rectangle, bool bCull, MWDREAL& fSegmentParameter,MWDREAL fRectangleParameter[2]) {
			if (RelationWith(S,Rectangle, bCull, fSegmentParameter, fRectangleParameter) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (fSegmentParameter >S.GetLen())
				return MWDNOINTERSECT;
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDOBB3& OBB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(S,OBB, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tNear >S.GetLen())
			{
				Quantity = 0;
				if (RelationWith(S.GetOrig(),OBB) != MWDOUT)
					return MWDIN;
				return MWDNOINTERSECT;
			}

			if (tFar >S.GetLen())
			{
				Quantity = 1;
				tFar = tNear;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDAABB3& AABB, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(S,AABB, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tNear >S.GetLen())
			{
				Quantity = 0;
				if (RelationWith(S.GetOrig(),AABB) != MWDOUT)
					return MWDIN;
				return MWDNOINTERSECT;
			}

			if (tFar > S.GetLen())
			{
				Quantity = 1;
				tFar = tNear;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDSphere3& sphere, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			if (RelationWith(S,sphere, Quantity, tNear, tFar) == MWDNOINTERSECT)
				return MWDNOINTERSECT;
			if (tNear >S.GetLen())
			{
				Quantity = 0;
				if (RelationWith(S.GetOrig(),sphere) != MWDOUT)
					return MWDIN;
				return MWDNOINTERSECT;
			}

			if (tFar > S.GetLen())
			{
				Quantity = 1;
				tFar = tNear;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDSegment3& S, const MWDPolygon3& Polygon, MWDREAL& fSegmentParameter,bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3]) {
			return RelationWith(Polygon,S, bCull, iIndexTriangle, fTriangleParameter, fSegmentParameter);
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDVector3& Point, MWDREAL& fSegmentParameter) {
			MWDREAL SqDis = SquaredDistance(S,Point, fSegmentParameter);
			if (fSegmentParameter > S.GetLen())
			{
				fSegmentParameter = S.GetLen();
				MWDVector3 Diff;
				Diff = Point - S.GetEnd();
				return Diff.GetSqrLength();
			}
			else
			{
				return SqDis;
			}
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDLine3& Line, MWDREAL& fSegmentParameter, MWDREAL& fLineParameter) {
			return SquaredDistance(Line,S, fLineParameter, fSegmentParameter);
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDRay3& Ray, MWDREAL& fSegmentParameter, MWDREAL& fRayParameter) {
			return SquaredDistance(Ray,S, fRayParameter, fSegmentParameter);
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDSegment3& Segment, MWDREAL& fSegment1Parameter, MWDREAL& fSegment2Parameter) {
			const MWDRay3& Ray = Segment;
			MWDREAL sqrDist = SquaredDistance(S,Ray, fSegment1Parameter, fSegment2Parameter);
			MWDREAL fLen = Segment.GetLen();
			if (fSegment2Parameter > fLen)
			{
				fSegment2Parameter = fLen;
				MWDVector3 End = Segment.GetEnd();
				sqrDist = SquaredDistance(S,End, fSegment1Parameter);
			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDTriangle3& Triangle, MWDREAL& fSegmentParameter, MWDREAL fTriangleParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(S,Triangle, fSegmentParameter, fTriangleParameter);

			if (fSegmentParameter > S.GetLen())
			{
				fSegmentParameter = S.GetLen();
				sqrDist = SquaredDistance(S.GetEnd(),Triangle, fTriangleParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDRectangle3& Rectangle, MWDREAL& fSegmentParameter, MWDREAL fRectangleParameter[2]) {
			MWDREAL sqrDist = SquaredDistance(S,Rectangle, fSegmentParameter, fRectangleParameter);

			if (fSegmentParameter > S.GetLen())
			{
				fSegmentParameter = S.GetLen();
				sqrDist = SquaredDistance(S.GetEnd(),Rectangle, fRectangleParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDOBB3& OBB, MWDREAL& fSegmentParameter, MWDREAL fOBBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(S,OBB, fSegmentParameter, fOBBParameter);
			if (fSegmentParameter > S.GetLen())
			{
				fSegmentParameter = S.GetLen();
				sqrDist = SquaredDistance(OBB,S.GetEnd(), fOBBParameter);

			}


			return sqrDist;
		};
		static MWDREAL Distance(const MWDSegment3& S, const MWDSphere3& Sphere, MWDREAL& fSegmentParameter, MWDVector3& SpherePoint) {
			return Distance(Sphere,S, SpherePoint, fSegmentParameter);
		};
		static MWDREAL Distance(const MWDSegment3& S, const MWDPlane3& Plane, MWDVector3& SegmentPoint, MWDVector3& PlanePoint) {
			return Distance(Plane,S, PlanePoint, SegmentPoint);
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDAABB3& AABB, MWDREAL& fSegmentParameter, MWDREAL fAABBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(S,AABB, fSegmentParameter, fAABBParameter);
			if (fSegmentParameter > S.GetLen())
			{
				fSegmentParameter = S.GetLen();
				sqrDist = SquaredDistance(AABB,S.GetEnd(), fAABBParameter);

			}
			return sqrDist;
		};
		static MWDREAL SquaredDistance(const MWDSegment3& S, const MWDPolygon3& Polygon, MWDREAL& fSegmentParameter,int& IndexTriangle,MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Polygon,S, IndexTriangle, fTriangleParameter, fSegmentParameter);
		};
#pragma endregion

		//Plane3
		static void Transform(MWDPlane3& inout,const MWDPlane3& Plane, const MWDMatrix3X3W& Mat) {
			MWDVector3W Temp(inout.m_N.x, inout.m_N.y, inout.m_N.z, inout.m_fD);
			MWDMatrix3X3W Inv;
			Inv.Inverse(Mat);
			MWDMatrix3X3W Transpose;
			Transpose.Transpose(Inv);
			MWDVector3W Resoult = MWD::Vec3WMulMat3x3W(Temp, Transpose); 
			inout.Set(MWDVector3(Resoult.x, Resoult.y, Resoult.z), Resoult.w);
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDVector3& Point, MWDVector3& PlanePoint) {
			MWDVector3 pt = Point, plp = PlanePoint;
			MWDVector3 P_Nor = P.GetN();
			MWDREAL Dist = ABS((P_Nor.Dot(pt)) -P.m_fD);
			P_Nor.Negate();
			MWDLine3 Line(pt, P_Nor);
			PlanePoint = Line.GetParameterPoint(Dist);

			return Dist;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDSphere3& Sphere, MWDVector3& SpherePoint) {
			return Distance(Sphere,P, SpherePoint);
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDLine3& Line, MWDVector3& PlanePoint, MWDVector3& LinePoint) {
			MWDVector3 dir = Line.GetDir();
			MWDREAL fDot = dir.Dot(P.GetN());
			if (ABS(fDot) < EPSILON_E4)
			{
				LinePoint = Line.GetOrig();
				return Distance(LinePoint,P, PlanePoint);
			}
			else
				return 0;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDRay3& Ray, MWDVector3& PlanePoint, MWDVector3& RayPoint) {
			MWDVector3 dir = Ray.GetDir();
			MWDREAL fDot = dir.Dot(P.GetN());
			RayPoint = Ray.GetOrig();

			MWDREAL f = RayPoint.Dot(P.GetN()) + P.GetfD();
			if (fDot * f > 0)
				return Distance(RayPoint,P, PlanePoint);
			else
				return 0;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDSegment3& Segment, MWDVector3& PlanePoint, MWDVector3& SegmentPoint) {
			MWDVector3 SegmentOrig = Segment.GetOrig();
			MWDVector3 SegmentEnd = Segment.GetEnd();
			MWDREAL f0 = SegmentOrig.Dot(P.m_N) + P.m_fD;
			MWDREAL f1 = SegmentEnd.Dot(P.m_N) + P.m_fD;

			if (f0 * f1 > 0)
			{
				MWDVector3 PPoint;
				MWDREAL dist = Distance(SegmentOrig,P, PPoint);
				SegmentPoint = SegmentOrig;
				PlanePoint = PPoint;
				MWDREAL distTemp = Distance(SegmentEnd,P, PPoint);
				if (dist > distTemp)
				{
					dist = distTemp;
					SegmentPoint = SegmentEnd;
					PlanePoint = PPoint;
				}
				return dist;
			}
			else
				return 0;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDPlane3& Plane, MWDVector3& Plane1Point, MWDVector3& Plane2Point) {
			if (IsParallel(P.m_N,Plane.m_N))
			{
				Plane1Point = P.GetPoint();
				return Distance(Plane1Point,Plane, Plane2Point);
			}
			else
				return 0;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDTriangle3& Triangle, MWDVector3& PlanePoint, MWDVector3& TrianglePoint) {
			MWDVector3 Point[3];
			Triangle.GetPoint(Point);

			MWDREAL dist[3], distTemp;
			MWDVector3 PlanePointTemp[3];
			dist[0] = Distance(Point[0],P, PlanePointTemp[0]);
			dist[1] = Distance(Point[1],P, PlanePointTemp[1]);
			dist[2] = Distance(Point[2],P, PlanePointTemp[2]);
			distTemp = dist[0];
			PlanePoint = PlanePointTemp[0];
			TrianglePoint = Point[0];
			if (distTemp > dist[1])
			{
				distTemp = dist[1];
				PlanePoint = PlanePointTemp[1];
				TrianglePoint = Point[1];
			}
			if (distTemp > dist[2])
			{
				distTemp = dist[2];
				PlanePoint = PlanePointTemp[2];
				TrianglePoint = Point[2];
			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDRectangle3& Rectangle, MWDVector3& PlanePoint, MWDVector3& RectanglePoint) {
			MWDVector3 Point[4];
			Rectangle.GetPoint(Point);

			MWDREAL dist[4], distTemp;
			MWDVector3 PlanePointTemp[4];
			for (int i = 0; i < 4; i++)
				dist[i] = Distance(Point[i],P, PlanePointTemp[i]);

			distTemp = dist[0];
			PlanePoint = PlanePointTemp[0];
			RectanglePoint = Point[0];
			for (int i = 1; i < 4; i++)
			{
				if (distTemp > dist[i])
				{
					distTemp = dist[i];
					PlanePoint = PlanePointTemp[i];
					RectanglePoint = Point[i];
				}
			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDOBB3& OBB, MWDVector3& PlanePoint, MWDVector3& OBBPoint) {
			MWDVector3 Point[8];
			GetPoint(OBB,Point);


			MWDREAL dist[4], distTemp;
			MWDVector3 PlanePointTemp[8];
			for (int i = 0; i < 8; i++)
				dist[i] = Distance(Point[i],P, PlanePointTemp[i]);

			distTemp = dist[0];
			PlanePoint = PlanePointTemp[0];
			OBBPoint = Point[0];
			for (int i = 1; i < 8; i++)
			{
				if (distTemp > dist[i])
				{
					distTemp = dist[i];
					PlanePoint = PlanePointTemp[i];
					OBBPoint = Point[i];
				}
			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDAABB3& AABB, MWDVector3& PlanePoint, MWDVector3& AABBPoint) {
			MWDVector3 Point[8];
			GetPoint(AABB,Point);



			MWDREAL dist[4], distTemp;
			MWDVector3 PlanePointTemp[8];
			for (int i = 0; i < 8; i++)
				dist[i] = Distance(Point[i],P, PlanePointTemp[i]);

			distTemp = dist[0];
			PlanePoint = PlanePointTemp[0];
			AABBPoint = Point[0];
			for (int i = 1; i < 8; i++)
			{
				if (distTemp > dist[i])
				{
					distTemp = dist[i];
					PlanePoint = PlanePointTemp[i];
					AABBPoint = Point[i];
				}
			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPlane3& P, const MWDPolygon3& Polygon, MWDVector3& PlanePoint, int& IndexTriangle,MWDVector3& TrianglePoint) {
			return Distance(Polygon,P, IndexTriangle, TrianglePoint, PlanePoint);
		};
		static int RelationWith(const MWDPlane3& P, const MWDVector3& Point) {
			MWDVector3 tmp = Point;
			MWDREAL f = (tmp.Dot(P.m_N)) + P.m_fD;

			if (f > EPSILON_E4) return MWDFRONT;
			if (f < -EPSILON_E4) return MWDBACK;
			return MWDON;
		};
		static int RelationWith(const MWDPlane3& P, const MWDLine3& Line, bool bCull, MWDREAL& fLineParameter) {
			return RelationWith(Line,P, bCull, fLineParameter);
		};
		static int RelationWith(const MWDPlane3& P, const MWDRay3& Ray, bool bCull, MWDREAL& fRayParameter) {
			return RelationWith(Ray,P, bCull, fRayParameter);
		};
		static int RelationWith(const MWDPlane3& P, const MWDSegment3& Segment, bool bCull, MWDREAL& fSegmentParameter) {
			return RelationWith(Segment,P, bCull, fSegmentParameter);
		};
		static int RelationWith(const MWDPlane3& P, const MWDOBB3& OBB) {
			return RelationWith(OBB,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDAABB3& AABB) {
			return RelationWith(AABB,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDSphere3& Sphere) {
			return RelationWith(Sphere,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDTriangle3& Triangle) {
			return RelationWith(Triangle,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDTriangle3& Triangle, MWDSegment3& Segment) {
			return RelationWith(Triangle,P, Segment);
		};
		static int RelationWith(const MWDPlane3& P, const MWDPlane3& Plane) {
			MWDVector3 vcCross;
			MWDREAL     fSqrLength;

			vcCross.Cross(P.m_N, Plane.m_N);
			fSqrLength = vcCross.GetSqrLength();


			if (fSqrLength < EPSILON_E4)
			{
				//return Plane.m_Point.RelationWith(*this);
				return MWDNOINTERSECT;
			}
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDPlane3& P, const MWDPlane3& Plane, MWDLine3& Line) {
			MWDVector3 vcCross;
			MWDREAL     fSqrLength;


			vcCross.Cross(P.m_N, Plane.m_N);
			fSqrLength = vcCross.GetSqrLength();


			if (fSqrLength < EPSILON_E4)
			{
				return MWDNOINTERSECT;
				//return Plane.m_Point.RelationWith(*this);
			}
			MWDREAL fN00 = P.m_N.GetSqrLength();
			MWDVector3 tmp_nor = P.m_N,t1;
			MWDREAL fN01 = tmp_nor.Dot(Plane.m_N);
			MWDREAL fN11 = Plane.m_N.GetSqrLength();
			MWDREAL fDet = fN00 * fN11 - fN01 * fN01;

			if (ABS(fDet) < EPSILON_E4)
				return MWDNOINTERSECT;

			MWDREAL fInvDet = 1.0f / fDet;
			MWDREAL fC0 = (fN11 * P.m_fD - fN01 * Plane.m_fD) * fInvDet;
			MWDREAL fC1 = (fN00 * Plane.m_fD - fN01 * P.m_fD) * fInvDet;
			t1 = tmp_nor * fC0 + Plane.m_N * fC1;
			Line.Set(t1, vcCross);
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDPlane3& P, const MWDRectangle3& Rectangle) {
			return RelationWith(Rectangle,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDRectangle3& Rectangle, MWDSegment3& Segment) {
			return RelationWith(Rectangle,P, Segment);
		};
		static int RelationWith(const MWDPlane3& P, const MWDPolygon3& Polygon) {
			return RelationWith(Polygon,P);
		};
		static int RelationWith(const MWDPlane3& P, const MWDPolygon3& Polygon, MWDSegment3& Segment) {
			return RelationWith(Polygon,P, Segment);
		};

		//未实现Cylinder3
		static int RelationWith(const MWDPlane3& P, const MWDCylinder3& Cylinder3) {
			return MWDINTERSECT;
		};


		//Triangle3
		static bool GetParameter(const MWDTriangle3& tri,const MWDVector3& Point, MWDREAL fTriangleParameter[3]) {
			MWDVector3 v01, v02, v0p;
			MWDVector3 vp1, vp2;
			v01 = tri. m_V[1] - tri.m_V[0];
			v02 = tri.m_V[2] - tri.m_V[0];
			v0p = Point - tri.m_V[0];
			vp1 = tri.m_V[1] - Point;
			vp2 = tri.m_V[2] - Point;

			MWDVector3 S[4];

			S[0] = MWD::Cross(v01, v02);
			S[1] = MWD::Cross(v01, v0p);
			S[2] = MWD::Cross(v0p, v02);
			S[3] = MWD::Cross(vp1, vp2);

			MWDREAL fS[4];
			for (int i = 0; i < 4; i++)
			{
				fS[i] = S[i].GetLength();
			}
			for (int i = 0; i < 3; i++)
			{
				fTriangleParameter[i] = fS[i + 1] / fS[0];
			}
			MWDREAL fSum = 0.0f;
			for (int i = 0; i < 3; i++)
			{
				fSum += fTriangleParameter[i];
				if (fTriangleParameter[i] < 0.0f || fTriangleParameter[i] > 1.0f)
					return 0;
			}
			if (fSum > 1.0f)
				return 0;
			return 1;
		};
		static void Transform(MWDTriangle3& Tri, const MWDTriangle3& Triangle, const MWDMatrix3X3W& Mat) {
			Tri.m_V[0] = MWD::Vec3MulMat3x3W(Triangle.m_V[0],Mat);
			Tri.m_V[1] = MWD::Vec3MulMat3x3W(Triangle.m_V[1],Mat);
			Tri.m_V[2] = MWD::Vec3MulMat3x3W(Triangle.m_V[2],Mat);
			Tri.Set(Tri.m_V);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDVector3& Point, MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Point,T, fTriangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDTriangle3& Triangle,MWDREAL fTriangle1Parameter[3],MWDREAL fTriangle2Parameter[3]) {
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDSegment3 Edge;
			MWDREAL Tempt0, Temp[3];
			MWDREAL fR0, fS0, fT0, fR1, fS1, fT1;
			MWDVector3 v0 = T.m_V[0], v1 = T.m_V[1],v2 = T.m_V[2];
			Edge.Set(v0 ,v1 );
			fSqrDistTmp = SquaredDistance(Edge,Triangle, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{

				fS0 = Tempt0 / Edge.GetLen();
				fR0 = 1.0f - fS0;
				fT0 = 0.0f;
				fR1 = Temp[0];
				fS1 = Temp[1];
				fT1 = Temp[2];
				fSqrDist = fSqrDistTmp;
			}


			Edge.Set(v0, v2);
			fSqrDistTmp = SquaredDistance(Edge,Triangle, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{


				fS0 = 0.0f;
				fT0 = Tempt0 / Edge.GetLen();
				fR0 = 1.0f - fT0;

				fR1 = Temp[0];
				fS1 = Temp[1];
				fT1 = Temp[2];

				fSqrDist = fSqrDistTmp;
			}

			Edge.Set(v1, v2);
			fSqrDistTmp = SquaredDistance(Edge,Triangle, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{

				fS0 = Tempt0 / Edge.GetLen();
				fT0 = 1.0f - fS0;
				fR0 = 0.0f;

				fR1 = Temp[0];
				fS1 = Temp[1];
				fT1 = Temp[2];

				fSqrDist = fSqrDistTmp;
			}

			MWDVector3 TV0 = Triangle.m_V[0], TV1 = Triangle.m_V[1], TV2 = Triangle.m_V[3];

			Edge.Set(TV0, TV1);
			fSqrDistTmp = SquaredDistance(Edge,T, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{

				fS1 = Tempt0 / Edge.GetLen();
				fR1 = 1.0f - fS0;
				fT1 = 0.0f;
				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];
				fSqrDist = fSqrDistTmp;
			}


			Edge.Set(TV0, TV2);
			fSqrDistTmp = SquaredDistance(Edge,T, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{


				fS1 = 0.0f;
				fT1 = Tempt0 / Edge.GetLen();
				fR1 = 1.0f - fT0;

				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];

				fSqrDist = fSqrDistTmp;
			}

			Edge.Set(TV1, TV2);
			fSqrDistTmp = SquaredDistance(Edge,T, Tempt0, Temp);

			if (fSqrDistTmp < fSqrDist)
			{

				fS1 = Tempt0 / Edge.GetLen();
				fT1 = 1.0f - fS0;
				fR1 = 0.0f;

				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];

				fSqrDist = fSqrDistTmp;
			}

			fTriangle1Parameter[0] = fR0;
			fTriangle1Parameter[1] = fS0;
			fTriangle1Parameter[2] = fT0;

			fTriangle2Parameter[0] = fR1;
			fTriangle2Parameter[1] = fS1;
			fTriangle2Parameter[2] = fT1;
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDRectangle3& Rectangle,MWDREAL fTriangleParameter[3],MWDREAL fRectangleParameter[2]) {
			MWDVector3 RectanglePoint[4];
			Rectangle.GetPoint(RectanglePoint);
			MWDSegment3 Edge;
			//MWDVector3 A[2];
			//Rectangle.GetA(A);
			MWDREAL fA[2];
			Rectangle.GetfA(fA);
			MWDREAL t, Temp[3];
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL fR0, fS0, fT0, fT1, fS1;
			Edge.Set(RectanglePoint[1], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,T, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;

				fS1 = t - fA[0];
				fT1 = fA[1];
				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];

			}
			Edge.Set(RectanglePoint[3], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,T, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = fA[0];
				fT1 = t - fA[1];
				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];
			}
			Edge.Set(RectanglePoint[2], RectanglePoint[3]);
			fSqrDistTmp = SquaredDistance(Edge,T, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = t - fA[0];
				fT1 = -fA[1];
				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];
			}
			Edge.Set(RectanglePoint[2], RectanglePoint[1]);
			fSqrDistTmp = SquaredDistance(Edge,T, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = -fA[0];
				fT1 = t - fA[1];
				fR0 = Temp[0];
				fS0 = Temp[1];
				fT0 = Temp[2];
			}

			MWDVector3 TV0 = T.m_V[0], TV1 = T.m_V[1], TV2 = T.m_V[2];

			Edge.Set(TV0, TV1);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{


				fS0 = t / Edge.GetLen();
				fR0 = 1.0f - fS0;
				fT0 = 0.0f;
				fS1 = Temp[0];
				fT1 = Temp[1];
				fSqrDist = fSqrDistTmp;
			}

			Edge.Set(TV0, TV2);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{


				fS0 = 0.0f;
				fT0 = t / Edge.GetLen();
				fR0 = 1.0f - fT0;

				fS1 = Temp[0];
				fT1 = Temp[1];
				fSqrDist = fSqrDistTmp;
			}

			Edge.Set(TV1, TV2);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, Temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fS0 = t / Edge.GetLen();
				fT0 = 1.0f - fS0;
				fR0 = 0.0f;

				fS1 = Temp[0];
				fT1 = Temp[1];
				fSqrDist = fSqrDistTmp;
			}
			fTriangleParameter[0] = fR0;
			fTriangleParameter[1] = fS0;
			fTriangleParameter[2] = fT0;
			fRectangleParameter[0] = fS1;
			fRectangleParameter[2] = fT1;
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDLine3& Line, MWDREAL fTriangleParameter[3], MWDREAL& fLineParameter) {
			return SquaredDistance(Line,T, fLineParameter, fTriangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDRay3& Ray, MWDREAL fTriangleParameter[3], MWDREAL& fRayParameter) {
			return SquaredDistance(Ray,T, fRayParameter, fTriangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDSegment3& Segment, MWDREAL fTriangleParameter[3], MWDREAL& fSegmentParameter) {
			return SquaredDistance(Segment,T, fSegmentParameter, fTriangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDOBB3& OBB, MWDREAL TriangleParameter[3], MWDREAL OBBParameter[3]) {
			return SquaredDistance(OBB,T, OBBParameter, TriangleParameter);
		};
		static MWDREAL Distance(const MWDTriangle3& T, const MWDSphere3& Sphere, MWDREAL fTriangleParameter[3], MWDVector3& SpherePoint) {
			return Distance(Sphere,T, SpherePoint, fTriangleParameter);
		};
		static MWDREAL Distance(const MWDTriangle3& T, const MWDPlane3& Plane, MWDVector3& TrianglePoint, MWDVector3& PlanePoint) {
			return Distance(Plane,T, PlanePoint, TrianglePoint);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDAABB3& AABB, MWDREAL TriangleParameter[3], MWDREAL AABBParameter[3]) {
			return SquaredDistance(AABB,T, AABBParameter, TriangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDTriangle3& T, const MWDPolygon3& Polygon,MWDREAL fTriangle1Parameter[3],int& IndexTriangle,MWDREAL fTriangle2Parameter[3]) {
			return SquaredDistance(Polygon,T, IndexTriangle, fTriangle2Parameter, fTriangle1Parameter);
		};
		static int RelationWith(const MWDTriangle3& T, const MWDLine3& Line, bool bCull, MWDREAL fTriangleParameter[3],MWDREAL& fLineParameter) {
			return RelationWith(Line,T, bCull, fLineParameter, fTriangleParameter);
		};
		static int RelationWith(const MWDTriangle3& T, const MWDRay3& Ray, bool bCull, MWDREAL fTriangleParameter[3],MWDREAL& fRayParameter) {
			return RelationWith(Ray,T, bCull, fRayParameter, fTriangleParameter);
		};
		static int RelationWith(const MWDTriangle3& T, const MWDSegment3& Segment, bool bCull, MWDREAL fTriangleParameter[3],MWDREAL& fSegmentParameter) {
			return RelationWith(Segment,T, bCull, fSegmentParameter, fTriangleParameter);
		};
		static int RelationWith(const MWDTriangle3& T, const MWDPlane3& Plane) {
			int iFrontNum = 0;
			int iBackNum = 0;

			for (int i = 0; i < 3; i++)
			{
				MWDVector3 tmp = T.m_V[i];
				int iFlag = RelationWith(tmp,Plane);
				if (iFlag == MWDFRONT)
				{
					iFrontNum++;
				}
				else if (iFlag == MWDBACK)
				{
					iBackNum++;
				}
				if (iFrontNum > 0 && iBackNum > 0)
					return MWDINTERSECT;

			}
			if (iFrontNum == 0 && iBackNum == 0)
			{
				return MWDON;
			}
			else if (iFrontNum == 0)
				return MWDBACK;
			else if (iBackNum == 0)
				return MWDFRONT;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDPlane3& Plane, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			int OnNum = 0, BackNum = 0, FrontNum = 0;
			MWDVector3 Orig, End;
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 Vi = T.m_V[i], Vj = T.m_V[j];
				SegmentTemp.Set(Vi, Vj);
				MWDREAL t;
				int iFlag = RelationWith(SegmentTemp,Plane, 0, t);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(t);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(t);
						Segment.Set(Orig, End);
						return MWDINTERSECT;

					}
				}
				else if (iFlag == MWDON)
				{
					OnNum++;

				}
				else if (iFlag == MWDBACK)
				{
					BackNum++;

				}
				else if (iFlag == MWDFRONT)
				{
					FrontNum++;
				}

			}
			if (OnNum == 3)
				return MWDON;
			if (BackNum == 3)
				return MWDBACK;
			if (FrontNum == 3)
				return MWDFRONT;
			//not control all path
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDTriangle3& Triangle) {
			MWDSegment3 Segment;
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 Vi = T.m_V[i], Vj = T.m_V[j];
				Segment.Set(Vi,Vj);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[3];
				if (RelationWith(Segment,Triangle, 0, fSegmentParameter, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 TVi = Triangle.m_V[i], TVj = Triangle.m_V[j];
				Segment.Set(TVi, TVj);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[3];
				if (RelationWith(Segment,T, 0, fSegmentParameter, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDTriangle3& Triangle, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			MWDVector3 Orig, End;
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 Vi = T.m_V[i], Vj = T.m_V[j];
				SegmentTemp.Set(Vi, Vj);
				MWDREAL fTriangleParameter[3];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,Triangle, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}

			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 TVi = Triangle.m_V[i], TVj = Triangle.m_V[j];
				SegmentTemp.Set(TVi, TVj);
				MWDREAL fTriangleParameter[3];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,T, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}
			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDRectangle3& Rectangle) {
			MWDSegment3 Segment;
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 Vi = T.m_V[i], Vj = T.m_V[j];
				Segment.Set(Vi, Vj);
				MWDREAL fSegmentParameter;
				MWDREAL RectangleParameter[2];
				if (RelationWith(Segment,Rectangle, 0, fSegmentParameter, RectangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			MWDVector3 Point[4];
			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				Segment.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL RectangleParameter[2];
				if (RelationWith(Segment,T, 0, fSegmentParameter, RectangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			return MWDNOINTERSECT;

		};
		static int RelationWith(const MWDTriangle3& T, const MWDRectangle3& Rectangle, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			MWDVector3 Orig, End;
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				MWDVector3 Vi = T.m_V[i], Vj = T.m_V[j];
				SegmentTemp.Set(Vi, Vj);
				MWDREAL fTriangleParameter[2];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,Rectangle, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}

			}

			MWDVector3 Point[4];
			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL fTriangleParameter[3];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,T, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}
			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDAABB3& AABB) {
			MWDPlane3 Plane = T.GetPlane();
			if (RelationWith(Plane,AABB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[3];
			T.GetPoint(Point);
			for (int i = 0; i < 3; i++)
			{
				int iFlag = RelationWith(Point[i],AABB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDOBB3& OBB) {
			MWDPlane3 Plane = T.GetPlane();
			if (RelationWith(Plane,OBB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[3];
			T.GetPoint(Point);
			for (int i = 0; i < 3; i++)
			{
				int iFlag = RelationWith(Point[i],OBB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDTriangle3& T, const MWDSphere3& Sphere) {
			MWDPlane3 Plane = T.GetPlane();
			if (RelationWith(Plane,Sphere) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[3];
			T.GetPoint(Point);
			for (int i = 0; i < 3; i++)
			{
				int iFlag = RelationWith(Point[i],Sphere);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};

		//Rect3
		static bool GetParameter(const MWDRectangle3& Rect3,const MWDVector3& Point, MWDREAL fRectangleParameter[2]) {
			MWDVector3 A2, p = Point;
			MWDRectangle3 rect3 = Rect3;
			A2.Cross(rect3.m_A[0], rect3.m_A[1]);
			MWDMatrix3X3W m;
			m.CreateInWorldObject(rect3.m_A[0], rect3.m_A[1], A2, rect3.m_Center);
			MWDVector3 Temp;
			Temp = MWD::Vec3MulMat3x3W(p,m);
			fRectangleParameter[0] = Temp.x;
			fRectangleParameter[1] = Temp.y;
			if (ABS(Temp.z) > EPSILON_E4 || ABS(Temp.x) > rect3.m_fA[0] || ABS(Temp.y) > rect3.m_fA[1])
				return 0;
			return 1;
		};
		static void Transform(MWDRectangle3& Rect3, const MWDRectangle3& Rectangle,const MWDMatrix3X3W& Mat) {
			Rect3.m_A[0] = Mat.Apply3X3(Rectangle.m_A[0]);
			Rect3.m_A[1] = Mat.Apply3X3(Rectangle.m_A[1]);
			Rect3.m_A[0].Normalize();
			Rect3.m_A[1].Normalize();
			Rect3.m_Center = MWD::Vec3MulMat3x3W(Rectangle.m_Center , Mat) ;
			Rect3.m_fA[0] = Rectangle.m_fA[0];
			Rect3.m_fA[1] = Rectangle.m_fA[1];

			MWDVector3 N;
			N.Cross(Rect3.m_A[0], Rect3.m_A[1]);
			MWDPlane3(N, Rect3.m_Center);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDVector3& Point, MWDREAL fRectangleParameter[2]) {
			return SquaredDistance(Point,R, fRectangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDRectangle3& Rectangle,MWDREAL fRectangle1Parameter[2], MWDREAL fRectangle2Parameter[2]) {
			MWDREAL t, temp[2];
			MWDSegment3 Edge;
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDVector3 RectanglePoint[4];
			Rectangle.GetPoint(RectanglePoint);
			Edge.Set(RectanglePoint[1], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,R, t, temp);
			MWDREAL fS0, fT0, fS1, fT1;
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = t - Rectangle.m_fA[0];
				fT1 = Rectangle.m_fA[1];
				fS0 = temp[0];
				fT0 = temp[1];
			}

			Edge.Set(RectanglePoint[3], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,R, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = Rectangle.m_fA[0];
				fT1 = t - Rectangle.m_fA[1];
				fS0 = temp[0];
				fT0 = temp[1];
			}

			Edge.Set(RectanglePoint[2], RectanglePoint[3]);
			fSqrDistTmp = SquaredDistance(Edge,R, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = t - Rectangle.m_fA[0];
				fT1 = -Rectangle.m_fA[1];
				fS0 = temp[0];
				fT0 = temp[1];
			}

			Edge.Set(RectanglePoint[2], RectanglePoint[1]);
			fSqrDistTmp = SquaredDistance(Edge,R, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS1 = -Rectangle.m_fA[0];
				fT1 = t - Rectangle.m_fA[1];
				fS0 = temp[0];
				fT0 = temp[1];
			}





			R.GetPoint(RectanglePoint);
			Edge.Set(RectanglePoint[1], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS0 = t - Rectangle.m_fA[0];
				fT0 = Rectangle.m_fA[1];
				fS1 = temp[0];
				fT1 = temp[1];
			}

			Edge.Set(RectanglePoint[3], RectanglePoint[0]);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS0 = Rectangle.m_fA[0];
				fT0 = t - Rectangle.m_fA[1];
				fS1 = temp[0];
				fT1 = temp[1];
			}

			Edge.Set(RectanglePoint[2], RectanglePoint[3]);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS0 = t - Rectangle.m_fA[0];
				fT0 = -Rectangle.m_fA[1];
				fS1 = temp[0];
				fT1 = temp[1];
			}

			Edge.Set(RectanglePoint[2], RectanglePoint[1]);
			fSqrDistTmp = SquaredDistance(Edge,Rectangle, t, temp);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				fS0 = -Rectangle.m_fA[0];
				fT0 = t - Rectangle.m_fA[1];
				fS1 = temp[0];
				fT1 = temp[1];
			}
			fRectangle1Parameter[0] = fS0;
			fRectangle1Parameter[1] = fT0;
			fRectangle2Parameter[0] = fS1;
			fRectangle2Parameter[1] = fT1;
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDTriangle3& Triangle, MWDREAL fRectangleParameter[2], MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Triangle,R, fTriangleParameter, fRectangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDLine3& Line, MWDREAL fRectangleParameter[2], MWDREAL& fLineParameter) {
			return SquaredDistance(Line,R, fLineParameter, fRectangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDRay3& Ray, MWDREAL fRectangleParameter[2], MWDREAL& fRayParameter) {
			return SquaredDistance(Ray,R, fRayParameter, fRectangleParameter);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDSegment3& Segment, MWDREAL fRectangleParameter[2], MWDREAL& fSegmentParameter) {
			return SquaredDistance(Segment,R, fSegmentParameter, fRectangleParameter);

		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDOBB3& OBB, MWDREAL RectangleParameter[2], MWDREAL OBBParameter[3]) {
			return SquaredDistance(OBB,R, OBBParameter, RectangleParameter);

		};
		static MWDREAL Distance(const MWDRectangle3& R, const MWDSphere3& Sphere, MWDREAL fRectangleParameter[2], MWDVector3& SpherePoint) {
			return Distance(Sphere,R, SpherePoint, fRectangleParameter);
		};
		static MWDREAL Distance(const MWDRectangle3& R, const MWDPlane3& Plane, MWDVector3& RectanglePoint, MWDVector3& PlanePoint) {
			return Distance(Plane,R, PlanePoint, RectanglePoint);
		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDAABB3& AABB, MWDREAL RectangleParameter[2], MWDREAL AABBParameter[3]) {
			return 	SquaredDistance(AABB,R, RectangleParameter, AABBParameter);

		};
		static MWDREAL SquaredDistance(const MWDRectangle3& R, const MWDPolygon3& Polygon,MWDREAL fRectangleParameter[2],int& IndexTriangle,MWDREAL fTriangleParameter[3]) {
			return SquaredDistance(Polygon,R, IndexTriangle, fTriangleParameter, fRectangleParameter);
		};
		static int RelationWith(const MWDRectangle3& R, const MWDLine3& Line, bool bCull, MWDREAL fRectangleParameter[2], MWDREAL& fLineParameter) {
			return RelationWith(Line,R, 1, fLineParameter, fRectangleParameter);

		};
		static int RelationWith(const MWDRectangle3& R, const MWDRay3& Ray, bool bCull, MWDREAL fRectangleParameter[2], MWDREAL& fRayParameter) {
			return RelationWith(Ray,R, 1, fRayParameter, fRectangleParameter);

		};
		static int RelationWith(const MWDRectangle3& R, const MWDSegment3& Segment, bool bCull, MWDREAL fRectangleParameter[2], MWDREAL& fSegmentParameter) {
			return RelationWith(Segment,R, 1, fSegmentParameter, fRectangleParameter);

		};
		static int RelationWith(const MWDRectangle3& R, const MWDPlane3& Plane) {
			int iFrontNum = 0;
			int iBackNum = 0;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int iFlag = RelationWith(Point[i],Plane);
				if (iFlag == MWDFRONT)
				{
					iFrontNum++;
				}
				else if (iFlag == MWDBACK)
				{
					iBackNum++;
				}


			}
			if (iFrontNum == 0 && iBackNum == 0)
			{
				return MWDON;
			}
			else if (iFrontNum == 0)
				return MWDBACK;
			else if (iBackNum == 0)
				return MWDFRONT;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDPlane3& Plane, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			int OnNum = 0, BackNum = 0, FrontNum = 0;
			MWDVector3 Orig, End;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL t;
				int iFlag = RelationWith(SegmentTemp,Plane, 0, t);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(t);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(t);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}
				else if (iFlag == MWDON)
				{
					OnNum++;

				}
				else if (iFlag == MWDBACK)
				{
					BackNum++;

				}
				else if (iFlag == MWDFRONT)
				{
					FrontNum++;
				}

			}
			if (OnNum == 4)
				return MWDON;
			if (BackNum == 4)
				return MWDBACK;
			if (FrontNum == 4)
				return MWDFRONT;
			//not control all path
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDTriangle3& Triangle) {
			return RelationWith(Triangle,R);
		};
		static int RelationWith(const MWDRectangle3& R, const MWDTriangle3& Triangle, MWDSegment3& Segment) {
			return RelationWith(Triangle,R, Segment);
		};
		static int RelationWith(const MWDRectangle3& R, const MWDRectangle3& Rectangle) {
			MWDVector3 Point[4];
			R.GetPoint(Point);

			MWDSegment3 Segment;
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				Segment.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL RectangleParameter[2];
				if (RelationWith(Segment,Rectangle, 0, fSegmentParameter, RectangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				Segment.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL RectangleParameter[2];
				if (RelationWith(Segment,R, 0, fSegmentParameter, RectangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDRectangle3& Rectangle, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			MWDVector3 Orig, End;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL fRectangleParameter[2];
				int iFlag = RelationWith(SegmentTemp,Rectangle, 0, fSegmentParameter, fRectangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}


			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL fRectangleParameter[2];
				int iFlag = RelationWith(SegmentTemp,R, 0, fSegmentParameter, fRectangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}
			if (!InNum)
				return MWDNOINTERSECT;
			//not control all path
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDAABB3& AABB) {
			MWDPlane3 Plane = R.GetPlane();
			if (RelationWith(Plane,AABB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int iFlag = RelationWith(Point[i],AABB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDOBB3& OBB) {
			MWDPlane3 Plane = R.GetPlane();
			if (RelationWith(Plane,OBB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int iFlag = RelationWith(Point[i],OBB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDRectangle3& R, const MWDSphere3& Sphere) {
			MWDPlane3 Plane = R.GetPlane();
			if (RelationWith(Plane,Sphere) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;
			MWDVector3 Point[4];
			R.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int iFlag = RelationWith(Point[i],Sphere);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};

		//AABB3
		static int RelationWith(const MWDAABB3& AABB1,const MWDAABB3& AABB2) {
			MWDAABB3 aabb1 = AABB1, aabb2 = AABB2;
			if ((aabb1.m_Min.x > aabb2.m_Max.x) || (aabb2.m_Min.x > aabb1.m_Max.x))
				return MWDNOINTERSECT;
			if ((aabb1.m_Min.y > aabb2.m_Max.y) || (aabb2.m_Min.y > aabb1.m_Max.y))
				return MWDNOINTERSECT;
			if ((aabb1.m_Min.z > aabb2.m_Max.z) || (aabb2.m_Min.z > aabb1.m_Max.z))
				return MWDNOINTERSECT;
			return MWDINTERSECT;
		};
		static bool GetIntersect(MWDAABB3& InAABB, MWDAABB3& AABB, MWDAABB3& OutAABB) {
			if (RelationWith(InAABB,AABB) == MWDINTERSECT)
			{
				MWDArrayOrder<MWDREAL> XArray;
				XArray.AddElement(InAABB.GetMinPoint().x);
				XArray.AddElement(InAABB.GetMaxPoint().x);
				XArray.AddElement(AABB.GetMinPoint().x);
				XArray.AddElement(AABB.GetMaxPoint().x);

				MWDArrayOrder<MWDREAL> YArray;
				YArray.AddElement(InAABB.GetMinPoint().y);
				YArray.AddElement(InAABB.GetMaxPoint().y);
				YArray.AddElement(AABB.GetMinPoint().y);
				YArray.AddElement(AABB.GetMaxPoint().y);

				MWDArrayOrder<MWDREAL> ZArray;
				ZArray.AddElement(InAABB.GetMinPoint().z);
				ZArray.AddElement(InAABB.GetMaxPoint().z);
				ZArray.AddElement(AABB.GetMinPoint().z);
				ZArray.AddElement(AABB.GetMaxPoint().z);

				OutAABB.Set(MWDVector3(XArray[2], YArray[2], ZArray[2]), MWDVector3(XArray[1], YArray[1], ZArray[1]));
				return true;
			}
			return false;
		};
		static void GetPlane(const MWDAABB3& InAABB,MWDPlane3 pPlanes[6]) {
			MWDVector3 vcN;
			MWDAABB3 in = InAABB;
			vcN = in.m_A[0];
			pPlanes[0].Set(vcN, in.m_Max);

			vcN = in.m_A[0] * (-1.0f);
			pPlanes[1].Set(vcN, in.m_Min);

			vcN = in.m_A[2] * (-1.0f);
			pPlanes[2].Set(vcN, in.m_Min);

			vcN = in.m_A[2];
			pPlanes[3].Set(vcN, in.m_Max);

			vcN = in.m_A[1];
			pPlanes[4].Set(vcN, in.m_Max);

			vcN = in.m_A[1] * (-1.0f);
			pPlanes[5].Set(vcN, in.m_Min);
		};
		static void GetPoint(const MWDAABB3& InAABB, MWDVector3 Point[8]) {
			Point[0].Set(InAABB.m_Center.x + InAABB.m_fA[0], InAABB.m_Center.y + InAABB.m_fA[1], InAABB.m_Center.z + InAABB.m_fA[2]);
			Point[1].Set(InAABB.m_Center.x - InAABB.m_fA[0], InAABB.m_Center.y + InAABB.m_fA[1], InAABB.m_Center.z + InAABB.m_fA[2]);
			Point[2].Set(InAABB.m_Center.x - InAABB.m_fA[0], InAABB.m_Center.y - InAABB.m_fA[1], InAABB.m_Center.z + InAABB.m_fA[2]);
			Point[3].Set(InAABB.m_Center.x + InAABB.m_fA[0], InAABB.m_Center.y - InAABB.m_fA[1], InAABB.m_Center.z + InAABB.m_fA[2]);
			Point[4].Set(InAABB.m_Center.x + InAABB.m_fA[0], InAABB.m_Center.y + InAABB.m_fA[1], InAABB.m_Center.z - InAABB.m_fA[2]);
			Point[5].Set(InAABB.m_Center.x - InAABB.m_fA[0], InAABB.m_Center.y + InAABB.m_fA[1], InAABB.m_Center.z - InAABB.m_fA[2]);
			Point[6].Set(InAABB.m_Center.x - InAABB.m_fA[0], InAABB.m_Center.y - InAABB.m_fA[1], InAABB.m_Center.z - InAABB.m_fA[2]);
			Point[7].Set(InAABB.m_Center.x + InAABB.m_fA[0], InAABB.m_Center.y - InAABB.m_fA[1], InAABB.m_Center.z - InAABB.m_fA[2]);

		};
		static void GetRectangle(const MWDAABB3& InAABB, MWDRectangle3 Rectangle[6]) {
			MWDVector3 Center[6];
			Center[0].Set(InAABB.m_Center.x + InAABB.m_fA[0], InAABB.m_Center.y, InAABB.m_Center.z);
			Center[1].Set(InAABB.m_Center.x - InAABB.m_fA[0], InAABB.m_Center.y, InAABB.m_Center.z);
			Center[2].Set(InAABB.m_Center.x, InAABB.m_Center.y + InAABB.m_fA[1], InAABB.m_Center.z);
			Center[3].Set(InAABB.m_Center.x, InAABB.m_Center.y - InAABB.m_fA[1], InAABB.m_Center.z);
			Center[4].Set(InAABB.m_Center.x, InAABB.m_Center.y, InAABB.m_Center.z + InAABB.m_fA[2]);
			Center[5].Set(InAABB.m_Center.x, InAABB.m_Center.y, InAABB.m_Center.z - InAABB.m_fA[2]);
			//right
			Rectangle[0].Set(Center[0], InAABB.m_A[1], InAABB.m_A[2], InAABB.m_fA[1], InAABB.m_fA[2]);
			//left
			Rectangle[1].Set(Center[1], InAABB.m_A[1], InAABB.m_A[2] * (-1), InAABB.m_fA[1], InAABB.m_fA[2]);

			//up
			Rectangle[2].Set(Center[4], InAABB.m_A[2], InAABB.m_A[0], InAABB.m_fA[2], InAABB.m_fA[1]);
			//down
			Rectangle[3].Set(Center[5], InAABB.m_A[2], InAABB.m_A[0] * (-1), InAABB.m_fA[2], InAABB.m_fA[0]);

			//front
			Rectangle[4].Set(Center[2], InAABB.m_A[1], InAABB.m_A[0] * (-1), InAABB.m_fA[1], InAABB.m_fA[0]);
			//back    
			Rectangle[5].Set(Center[3], InAABB.m_A[1], InAABB.m_A[0], InAABB.m_fA[1], InAABB.m_fA[0]);

		};
		static bool GetParameter(const MWDAABB3& InAABB, const MWDVector3& Point, MWDREAL fAABBParameter[3]) {
			for (int i = 0; i < 3; i++)
			{
				fAABBParameter[i] = Point.m[i] - InAABB.m_Center.m[i];
			}
			for (int i = 0; i < 3; i++)
				if (ABS(fAABBParameter[i]) > InAABB.m_fA[i])
					return 0;
			return 1;
		};
		static void Transform(MWDAABB3& InAABB, const MWDAABB3& AABB, const MWDMatrix3X3W& m, bool bHasProject = false) {
			MWDAABB3 inaabb = InAABB,aabb = AABB;
			if (bHasProject == false)
			{
				inaabb.m_Max = inaabb.m_Min = m.GetTranslation();
				if (m._00 > 0.0f)
				{
					inaabb.m_Min.x += m._00 * AABB.m_Min.x;
					inaabb.m_Max.x += m._00 * AABB.m_Max.x;

				}
				else
				{
					inaabb.m_Min.x += m._00 * AABB.m_Max.x;
					inaabb.m_Max.x += m._00 * AABB.m_Min.x;
				}

				if (m._01 > 0.0f)
				{
					inaabb.m_Min.y += m._01 * AABB.m_Min.x;
					inaabb.m_Max.y += m._01 * AABB.m_Max.x;

				}
				else
				{
					inaabb.m_Min.y += m._01 * AABB.m_Max.x;
					inaabb.m_Max.y += m._01 * AABB.m_Min.x;
				}

				if (m._02 > 0.0f)
				{
					inaabb.m_Min.z += m._02 * AABB.m_Min.x;
					inaabb.m_Max.z += m._02 * AABB.m_Max.x;

				}
				else
				{
					inaabb.m_Min.z += m._02 * AABB.m_Max.x;
					inaabb.m_Max.z += m._02 * AABB.m_Min.x;
				}

				if (m._10 > 0.0f)
				{
					inaabb.m_Min.x += m._10 * AABB.m_Min.y;
					inaabb.m_Max.x += m._10 * AABB.m_Max.y;

				}
				else
				{
					inaabb.m_Min.x += m._10 * AABB.m_Max.y;
					inaabb.m_Max.x += m._10 * AABB.m_Min.y;
				}

				if (m._11 > 0.0f)
				{
					inaabb.m_Min.y += m._11 * AABB.m_Min.y;
					inaabb.m_Max.y += m._11 * AABB.m_Max.y;

				}
				else
				{
					inaabb.m_Min.y += m._11 * AABB.m_Max.y;
					inaabb.m_Max.y += m._11 * AABB.m_Min.y;
				}

				if (m._12 > 0.0f)
				{
					inaabb.m_Min.z += m._12 * AABB.m_Min.y;
					inaabb.m_Max.z += m._12 * AABB.m_Max.y;

				}
				else
				{
					inaabb.m_Min.z += m._12 * AABB.m_Max.y;
					inaabb.m_Max.z += m._12 * AABB.m_Min.y;
				}

				if (m._20 > 0.0f)
				{
					inaabb.m_Min.x += m._20 * AABB.m_Min.z;
					inaabb.m_Max.x += m._20 * AABB.m_Max.z;

				}
				else
				{
					inaabb.m_Min.x += m._20 * AABB.m_Max.z;
					inaabb.m_Max.x += m._20 * AABB.m_Min.z;
				}

				if (m._21 > 0.0f)
				{
					inaabb.m_Min.y += m._21 * AABB.m_Min.z;
					inaabb.m_Max.y += m._21 * AABB.m_Max.z;

				}
				else
				{
					inaabb.m_Min.y += m._21 * AABB.m_Max.z;
					inaabb.m_Max.y += m._21 * AABB.m_Min.z;
				}

				if (m._22 > 0.0f)
				{
					inaabb.m_Min.z += m._22 * AABB.m_Min.z;
					inaabb.m_Max.z += m._22 * AABB.m_Max.z;

				}
				else
				{
					inaabb.m_Min.z += m._22 * AABB.m_Max.z;
					inaabb.m_Max.z += m._22 * AABB.m_Min.z;
				}
			}
			else
			{
				
				MWDVector3 Point[8];
				GetPoint(aabb,Point);

				InAABB.m_Max = MWDVector3(-MWDMAX_REAL, -MWDMAX_REAL, -MWDMAX_REAL);
				InAABB.m_Min = MWDVector3(MWDMAX_REAL, MWDMAX_REAL, MWDMAX_REAL);
				for (unsigned int i = 0; i < 8; i++)
				{
					MWDVector3 New =MWD::Vec3MulMat3x3W(Point[i] , m) ;

					InAABB.m_Min.x = Min(InAABB.m_Min.x, New.x);
					InAABB.m_Min.y = Min(InAABB.m_Min.y, New.y);
					InAABB.m_Min.z = Min(InAABB.m_Min.z, New.z);
					InAABB.m_Max.x = Max(InAABB.m_Max.x, New.x);
					InAABB.m_Max.y = Max(InAABB.m_Max.y, New.y);
					InAABB.m_Max.z = Max(InAABB.m_Max.z, New.z);
				}
			}
			InAABB.Set(InAABB.m_Max, InAABB.m_Min);
		};
		static MWDAABB3 MergAABB(const MWDAABB3& InAABB,const MWDAABB3& AABB) {
			MWDAABB3 Temp,inaabb = InAABB;
			Temp.m_Max = inaabb.m_Max;
			Temp.m_Min = inaabb.m_Min;
			if (Temp.m_Min.x > AABB.m_Min.x)
			{
				Temp.m_Min.x = AABB.m_Min.x;
			}
			if (Temp.m_Min.y > AABB.m_Min.y)
			{
				Temp.m_Min.y = AABB.m_Min.y;
			}
			if (Temp.m_Min.z > AABB.m_Min.z)
			{
				Temp.m_Min.z = AABB.m_Min.z;
			}

			if (Temp.m_Max.x < AABB.m_Max.x)
			{
				Temp.m_Max.x = AABB.m_Max.x;
			}
			if (Temp.m_Max.y < AABB.m_Max.y)
			{
				Temp.m_Max.y = AABB.m_Max.y;
			}
			if (Temp.m_Max.z < AABB.m_Max.z)
			{
				Temp.m_Max.z = AABB.m_Max.z;
			}
			Temp.Set(Temp.m_Max, Temp.m_Min);
			return Temp;
		};
		static void GetQuadAABB(const MWDAABB3& InAABB, MWDAABB3 AABB[4]) {
			MWDAABB3 inaabb = InAABB;
			MWDVector3 Point[8];
			GetPoint(inaabb,Point);
			MWDVector3 Mid[4];
			Mid[0] = (inaabb.m_Center + Point[0]) * 0.5;
			Mid[1] = (inaabb.m_Center + Point[1]) * 0.5;
			Mid[2] = (inaabb.m_Center + Point[4]) * 0.5;
			Mid[3] = (inaabb.m_Center + Point[5]) * 0.5;
			MWDREAL fA[3] = { inaabb.m_fA[0] * 0.5f ,inaabb.m_fA[1],inaabb.m_fA[2] * 0.5f };
			for (unsigned int i = 0; i < 4; i++)
			{
				Mid[i].y = inaabb.m_Center.y;
				AABB[i].Set(Mid[i], fA);
			}
		};
		static void GetOctAABB(const MWDAABB3& InAABB, MWDAABB3 AABB[8]) {
			MWDAABB3 inaabb = InAABB;
			MWDVector3 Point[8];
			GetPoint(inaabb,Point);
			MWDVector3 Mid[8];

			MWDREAL fA[3] = { inaabb.m_fA[0] * 0.5f , inaabb.m_fA[1] * 0.5f ,inaabb.m_fA[2] * 0.5f };
			for (unsigned int i = 0; i < 8; i++)
			{
				Mid[i] = (inaabb.m_Center + Point[i]) * 0.5;
				AABB[i].Set(Mid[i], fA);
			}
		};
		static MWDAABB3 GetMin(const MWDAABB3& InAABB, const MWDAABB3& AABB)  {
			MWDAABB3 inaabb = InAABB;
			MWDAABB3 Temp;
			Temp.m_Max = inaabb.m_Max;
			Temp.m_Min = inaabb.m_Min;
			if (Temp.m_Min.x < AABB.m_Min.x)
			{
				Temp.m_Min.x = AABB.m_Min.x;
			}
			if (Temp.m_Min.y < AABB.m_Min.y)
			{
				Temp.m_Min.y = AABB.m_Min.y;
			}
			if (Temp.m_Min.z < AABB.m_Min.z)
			{
				Temp.m_Min.z = AABB.m_Min.z;
			}

			if (Temp.m_Max.x > AABB.m_Max.x)
			{
				Temp.m_Max.x = AABB.m_Max.x;
			}
			if (Temp.m_Max.y > AABB.m_Max.y)
			{
				Temp.m_Max.y = AABB.m_Max.y;
			}
			if (Temp.m_Max.z > AABB.m_Max.z)
			{
				Temp.m_Max.z = AABB.m_Max.z;
			}
			Temp.Set(Temp.m_Max, Temp.m_Min);
			return Temp;
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDVector3& Point, MWDREAL fAABBParameter[3]) {
			MWDVector3 Diff = Point - AABB1.m_Center;

			MWDREAL fSqrDistance = 0.0, fDelta;

			fAABBParameter[0] = Diff.x;
			if (fAABBParameter[0] < -AABB1.m_fA[0])
			{
				fDelta = fAABBParameter[0] + AABB1.m_fA[0];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[0] = -AABB1.m_fA[0];
			}
			else if (fAABBParameter[0] > AABB1.m_fA[0])
			{
				fDelta = fAABBParameter[0] - AABB1.m_fA[0];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[0] = AABB1.m_fA[0];
			}

			fAABBParameter[1] = Diff.y;
			if (fAABBParameter[1] < -AABB1.m_fA[1])
			{
				fDelta = fAABBParameter[1] + AABB1.m_fA[1];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[1] = -AABB1.m_fA[1];
			}
			else if (fAABBParameter[1] > AABB1.m_fA[1])
			{
				fDelta = fAABBParameter[1] - AABB1.m_fA[1];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[1] = AABB1.m_fA[1];
			}

			fAABBParameter[2] = Diff.z;
			if (fAABBParameter[2] < -AABB1.m_fA[2])
			{
				fDelta = fAABBParameter[2] + AABB1.m_fA[2];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[2] = -AABB1.m_fA[2];
			}
			else if (fAABBParameter[2] > AABB1.m_fA[2])
			{
				fDelta = fAABBParameter[2] - AABB1.m_fA[2];
				fSqrDistance += fDelta * fDelta;
				fAABBParameter[2] = AABB1.m_fA[2];
			}
			return fSqrDistance;
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDLine3& Line, MWDREAL fAABBParameter[3], MWDREAL& fLineParameter) {
			MWDRectangle3 Rectangle[6];
			GetRectangle(AABB1,Rectangle);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL t, fAABB[3];
			for (int i = 0; i < 6; i++)
			{
				fSqrDistTmp = SquaredDistance(Line, AABB1, t, fAABB);
				if (fSqrDist < fSqrDistTmp)
				{
					fSqrDist = fSqrDistTmp;
					fLineParameter = t;
					for (int j = 0; j < 3; j++)
					{
						fAABBParameter[j] = fAABB[j];
					}
				}
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDRay3& Ray, MWDREAL fAABBParameter[3], MWDREAL& fRayParameter) {
			return SquaredDistance(Ray,AABB1, fRayParameter, fAABBParameter);
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDSegment3& Segment, MWDREAL fAABBParameter[3], MWDREAL& fSegmentParameter) {
			return SquaredDistance(Segment, AABB1, fSegmentParameter, fAABBParameter);
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDTriangle3& Triangle, MWDREAL AABBParameter[3], MWDREAL TriangleParameter[3]) {
			MWDRectangle3 Rectangle[6];
			GetRectangle(AABB1,Rectangle);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL RectP[2], TriP[3];

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[0], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = AABB1.m_fA[0];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[1], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = -AABB1.m_fA[0];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = -RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[2], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = RectP[1];
				AABBParameter[1] = AABB1.m_fA[1];
				AABBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[3], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = -RectP[1];
				AABBParameter[1] = -AABB1.m_fA[1];
				AABBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[4], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = -RectP[1];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = AABB1.m_fA[2];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[5], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				AABBParameter[0] = RectP[1];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = -AABB1.m_fA[2];
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDRectangle3& Rectangle, MWDREAL AABBParameter[3], MWDREAL RectangleParameter[2]) {
			MWDRectangle3 RectangleTemp[6];
			GetRectangle(AABB1,RectangleTemp);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL RectP[2], RectP1[2];

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[0], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = AABB1.m_fA[0];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[1], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = -AABB1.m_fA[0];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = -RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[2], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = RectP[1];
				AABBParameter[1] = AABB1.m_fA[1];
				AABBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[3], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = -RectP[1];
				AABBParameter[1] = -AABB1.m_fA[1];
				AABBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[4], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = -RectP[1];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = AABB1.m_fA[2];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[5], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				AABBParameter[0] = RectP[1];
				AABBParameter[1] = RectP[0];
				AABBParameter[2] = -AABB1.m_fA[2];
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDAABB3& AABB, MWDREAL AABB1Parameter[3], MWDREAL AABB2Parameter[3]) {
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDVector3 Point[8];
			GetPoint(AABB1,Point);
			MWDREAL Temp[3];
			for (int i = 0; i < 8; i++)
			{
				fSqrDistTmp = SquaredDistance(AABB1,Point[i], Temp);
				if (fSqrDistTmp < fSqrDist)
				{
					GetParameter(AABB1,Point[i], AABB1Parameter);
					for (int j = 0; j < 3; j++)
					{
						AABB2Parameter[j] = Temp[j];
					}
				}
			}
			return fSqrDist;
		};
		static MWDREAL Distance(const MWDAABB3& AABB1, const MWDSphere3& Sphere, MWDREAL fAABBParameter[3], MWDVector3& SpherePoint) {
			return Distance(Sphere,AABB1, SpherePoint, fAABBParameter);
		};
		static MWDREAL Distance(const MWDAABB3& AABB1, const MWDPlane3& Plane, MWDVector3& AABBPoint, MWDVector3& PlanePoint) {
			return Distance(Plane,AABB1, PlanePoint, AABBPoint);
		};
		static MWDREAL SquaredDistance(const MWDAABB3& AABB1, const MWDPolygon3& Polygon, MWDREAL AABBParameter[3], int& IndexTriangle, MWDREAL TriangleParameter[3]) {
			return SquaredDistance(Polygon,AABB1, IndexTriangle, TriangleParameter, AABBParameter);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDVector3& Point) {
			MWDVector3 PointTemp = Point - AABB1.m_Center;
			MWDREAL fDiffX = ABS(PointTemp.x) - AABB1.m_fA[0];
			MWDREAL fDiffY = ABS(PointTemp.y) - AABB1.m_fA[1];
			MWDREAL fDiffZ = ABS(PointTemp.z) - AABB1.m_fA[2];

			if (fDiffX > EPSILON_E4 || fDiffY > EPSILON_E4 || fDiffZ > EPSILON_E4)
				return MWDOUT;

			if (fDiffX < -EPSILON_E4 || fDiffY < -EPSILON_E4 || fDiffZ < -EPSILON_E4)
				return MWDIN;

			return MWDON;
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDLine3& Line, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Line,AABB1, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDRay3& Ray, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Ray,AABB1, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDSegment3& Segment, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Segment,AABB1, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDPlane3& Plane) {
			MWDVector3 N = Plane.GetN();
			MWDREAL fD = Plane.GetfD();
			MWDVector3 MinTemp, MaxTemp;
			// x 
			if (N.x >= 0.0f)
			{
				MinTemp.x = AABB1.m_Min.x;
				MaxTemp.x = AABB1.m_Max.x;
			}
			else
			{
				MinTemp.x = AABB1.m_Max.x;
				MaxTemp.x = AABB1.m_Min.x;
			}
			// y 
			if (N.y >= 0.0f)
			{
				MinTemp.y = AABB1.m_Min.y;
				MaxTemp.y = AABB1.m_Max.y;
			}
			else
			{
				MinTemp.y = AABB1.m_Max.y;
				MaxTemp.y = AABB1.m_Min.y;
			}
			// z 
			if (N.z >= 0.0f)
			{
				MinTemp.z = AABB1.m_Min.z;
				MaxTemp.z = AABB1.m_Max.z;
			}
			else
			{
				MinTemp.z = AABB1.m_Max.z;
				MaxTemp.z = AABB1.m_Min.z;
			}

			if ((N.Dot(MinTemp) + fD) > 0.0f)
				return MWDFRONT;
			else if ((N.Dot(MaxTemp) + fD) < 0.0f)
				return MWDBACK;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDTriangle3& Triangle) {
			return RelationWith(Triangle,AABB1);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDRectangle3& Rectangle) {
			return RelationWith(Rectangle,AABB1);
		};
		static int RelationWith(const MWDAABB3& AABB1, const MWDSphere3& Sphere) {
			MWDREAL fAABBParameter[3];
			MWDREAL Dist = SquaredDistance(Sphere.m_Center,AABB1, fAABBParameter);
			Dist = SQRT(Dist);
			if (Dist - Sphere.m_fRadius < 0.0f)
				return MWDINTERSECT;

			return MWDNOINTERSECT;
		};

		//OBB3
		static void CreateOBB( MWDAABB3& inout, const MWDVector3*  pPointArray, unsigned int uiPointNum) {
			if (!pPointArray || !uiPointNum)
				return;
			MWDVector3 Sum = pPointArray[0];
			for (unsigned int i = 1; i < uiPointNum; i++)
			{
				Sum += pPointArray[i];
			}
			inout.m_Center = Sum / (uiPointNum * 1.0f);
			MWDMatrix3X3 MatSum, MatTemp;
			MatSum.SetZero();

			for (unsigned int i = 0; i < uiPointNum; i++)
			{
				MWDVector3 Diff = pPointArray[i] - inout.m_Center;
				MatTemp.CreateFromTwoVector(Diff, Diff);
				MatSum += MatTemp;
			}
			MWDREAL InvNun = 1.0f / uiPointNum;
			MatSum *= InvNun;
			MWDREAL EigenValue[3];
			MWDVector3 vec[3] = {inout.m_A[0],inout.m_A[1] ,inout.m_A[2] };
			MatSum.GetEigenSystem(EigenValue, vec);

			MWDVector3 Min, Max;
			MWDVector3 Diff = pPointArray[0] - inout.m_Center;
			for (int j = 0; j < 3; j++)
			{
				MWDVector3 tmp = inout.m_A[j];
				Min.m[j] = tmp.Dot(Diff);
				Min.m[j] = tmp.Dot(Diff);
				Max.m[j] = Min.m[j];
			}
			for (unsigned int i = 1; i < uiPointNum; i++)
			{
				Diff = pPointArray[i] - inout.m_Center;
				for (int j = 0; j < 3; j++)
				{
					MWDVector3 tmp = inout.m_A[j];
					MWDREAL temp = tmp.Dot(Diff);
					if (temp < Min.m[j])
					{
						Min.m[j] = temp;
					}
					else if (temp > Max.m[j])
					{
						Max.m[j] = temp;
					}
				}
			}

			for (int j = 0; j < 3; j++)
			{
				inout.m_Center += inout.m_A[j] * (0.5f * (Min.m[j] + Max.m[j]));
				inout.m_fA[j] = 0.5f * (Max.m[j] - Min.m[j]);

			}
		};
		static MWDOBB3 MergOBB(const MWDOBB3& InOBB,const MWDOBB3& OBB) {
			MWDOBB3 Temp,inobb = InOBB;
			Temp.m_Center = (OBB.m_Center + inobb.m_Center) * 0.5f;
			MWDMatrix3X3 m1, m2;
			MWD::CreateRot(m1, inobb.m_A[0], inobb.m_A[1], inobb.m_A[2]);
			MWD::CreateRot(m2,OBB.m_A[0], OBB.m_A[1], OBB.m_A[2]);

			MWDQuat q, q1, q2;
			q1 = MWD::GetQuat(m1);
			q2 = MWD::GetQuat(m2);
			if (q1.Dot(q2) < 0)
			{
				q2 *= -1.0f;
			}
			q = q1 + q2;
			q.Normalize();

			MWDMatrix3X3 Mat;
			Mat = q.GetMatrix();
			Mat.GetUVN(Temp.m_A);

			//m_Center = (OBB1.m_Center + OBB2.m_Center) / 2.0f;

			MWDVector3 Point[8];

			MWD::GetPoint(inobb,Point);
			MWDVector3 Diff;
			Diff = Point[0] - Temp.m_Center;
			MWDVector3 Min = MWDVector3(Diff.Dot(Temp.m_A[0]), Diff.Dot(Temp.m_A[1]), Diff.Dot(Temp.m_A[2]));//= Point[0];
			MWDVector3 Max = Min;

			for (int i = 1; i < 8; i++)
			{
				Diff = Point[i] - Temp.m_Center;
				for (int j = 0; j < 3; j++)
				{
					MWDREAL fDot = Diff.Dot(Temp.m_A[j]);
					if (fDot > Max.m[j])
					{
						Max.m[j] = fDot;
					}
					else if (fDot < Min.m[j])
					{
						Min.m[j] = fDot;
					}
				}

			}
			MWD::GetPoint(OBB,Point);
			for (int i = 1; i < 8; i++)
			{
				Diff = Point[i] - Temp.m_Center;
				for (int j = 0; j < 3; j++)
				{
					MWDREAL fDot = Diff.Dot(Temp.m_A[j]);
					if (fDot > Max.m[j])
					{
						Max.m[j] = fDot;
					}
					else if (fDot < Min.m[j])
					{
						Min.m[j] = fDot;
					}
				}

			}
			for (int i = 0; i < 3; i++)
			{
				Temp.m_Center += Temp.m_A[i] * ((Min.m[i] + Max.m[i]) * 0.5f);
				Temp.m_fA[i] = 0.5f * (Max.m[i] - Min.m[i]);
			}
			return Temp;
		};
		static void GetPlane(const MWDOBB3& InOBB, MWDPlane3 pPlanes[6]) {
			MWDOBB3 inobb = InOBB;
			MWDVector3 Max, Min;
			Max = inobb.m_Center + inobb.m_A[0] * inobb.m_fA[0] + inobb.m_A[1] * inobb.m_fA[1] + inobb.m_A[2] * inobb.m_fA[2];
			Min = inobb.m_Center + inobb.m_A[0] * (-inobb.m_fA[0]) + inobb.m_A[1] * (-inobb.m_fA[1]) + inobb.m_A[2] * (-inobb.m_fA[2]);

			MWDVector3 vcN;

			vcN = inobb.m_A[0];
			pPlanes[0].Set(vcN, Max);

			vcN = inobb.m_A[0] * (-1.0f);
			pPlanes[1].Set(vcN, Min);

			vcN = inobb.m_A[2] * (-1.0f);
			pPlanes[2].Set(vcN, Min);

			vcN = inobb.m_A[2];
			pPlanes[3].Set(vcN, Max);

			vcN = inobb.m_A[1];
			pPlanes[4].Set(vcN, Max);

			vcN = inobb.m_A[1] * (-1.0f);
			pPlanes[5].Set(vcN, Min);
		};
		static void GetPoint(const MWDOBB3& InOBB, MWDVector3 Point[8]) {
			MWDOBB3 inobb = InOBB;
			MWDVector3 Temp[6];
			Temp[0] = inobb. m_A[0] * inobb.m_fA[0];
			Temp[1] = inobb.m_A[0] * (-inobb.m_fA[0]);

			Temp[2] = inobb.m_A[1] * inobb.m_fA[1];
			Temp[3] = inobb.m_A[1] * (-inobb.m_fA[1]);

			Temp[4] = inobb.m_A[2] * inobb.m_fA[2];
			Temp[5] = inobb.m_A[2] * (-inobb.m_fA[2]);

			MWDVector3 Temp1[6];
			Temp1[0] = inobb.m_Center + Temp[0];
			Temp1[1] = inobb.m_Center + Temp[1];

			Temp1[2] = Temp1[0] + Temp[2];
			Temp1[3] = Temp1[1] + Temp[2];
			Temp1[4] = Temp1[1] + Temp[3];
			Temp1[5] = Temp1[0] + Temp[3];

			Point[0] = Temp1[2] + Temp[4];
			Point[1] = Temp1[3] + Temp[4];
			Point[2] = Temp1[4] + Temp[4];
			Point[3] = Temp1[5] + Temp[4];

			Point[4] = Temp1[2] + Temp[5];
			Point[5] = Temp1[3] + Temp[5];
			Point[6] = Temp1[4] + Temp[5];
			Point[7] = Temp1[5] + Temp[5];
		};
		static void GetRectangle(const MWDOBB3& InOBB, MWDRectangle3 Rectangle[6]) {
			MWDOBB3 inobb = InOBB;
			MWDVector3 Center[6];
			Center[0] = inobb.m_Center + inobb.m_A[0] * inobb.m_fA[0];
			Center[1] = inobb.m_Center + inobb.m_A[0] * (-inobb.m_fA[0]);
			Center[2] = inobb.m_Center + inobb.m_A[1] * inobb.m_fA[1];
			Center[3] = inobb.m_Center + inobb.m_A[1] * (-inobb.m_fA[1]);
			Center[4] = inobb.m_Center + inobb.m_A[2] * inobb.m_fA[2];
			Center[5] = inobb.m_Center + inobb.m_A[2] * (-inobb.m_fA[2]);
			//right
			Rectangle[0].Set(Center[0], inobb.m_A[1], inobb.m_A[2], inobb.m_fA[1], inobb.m_fA[2]);
			//left
			Rectangle[1].Set(Center[1], inobb.m_A[1], inobb.m_A[2] * (-1), inobb.m_fA[1], inobb.m_fA[2]);

			//up
			Rectangle[2].Set(Center[4], inobb.m_A[2], inobb.m_A[0], inobb.m_fA[2], inobb.m_fA[1]);
			//down
			Rectangle[3].Set(Center[5], inobb.m_A[2], inobb.m_A[0] * (-1), inobb.m_fA[2], inobb.m_fA[0]);

			//front
			Rectangle[4].Set(Center[2], inobb.m_A[1], inobb.m_A[0] * (-1), inobb.m_fA[1], inobb.m_fA[0]);
			//back    
			Rectangle[5].Set(Center[3], inobb.m_A[1], inobb.m_A[0], inobb.m_fA[1], inobb.m_fA[0]);
		};
		static void GetTransform(const MWDOBB3& InOBB, MWDMatrix3X3W& m) {
			MWDOBB3 inobb = InOBB;
			m.CreateInWorldObject(inobb.m_A[0], inobb.m_A[1], inobb.m_A[2], inobb.m_Center);
		};
		static bool GetParameter(const MWDOBB3& InOBB, const MWDVector3& Point, MWDREAL fOBBParameter[3]) {
			MWDOBB3 inobb = InOBB;
			MWDMatrix3X3W m;
			GetTransform(inobb,m);
			MWDVector3 Temp =MWD::Vec3MulMat3x3W(Point ,m) ;
			fOBBParameter[0] = Temp.x;
			fOBBParameter[1] = Temp.y;
			fOBBParameter[2] = Temp.z;
			if (ABS(Temp.x) > inobb.m_fA[0] || ABS(Temp.y) > inobb.m_fA[1] || ABS(Temp.z) > inobb.m_fA[2])
				return 0;
			else
				return 1;
		};
		static void Transform( MWDOBB3& InOBB, const MWDOBB3& OBB, const MWDMatrix3X3W& Mat) {
			MWDOBB3 inobb = InOBB;
			InOBB.m_Center =MWD::Vec3MulMat3x3W(OBB.m_Center , Mat) ;
			InOBB.m_A[0] = Mat.Apply3X3(OBB.m_A[0]);
			InOBB.m_A[1] = Mat.Apply3X3(OBB.m_A[1]);
			InOBB.m_A[2] = Mat.Apply3X3(OBB.m_A[2]);

			InOBB.m_fA[0] = OBB.m_fA[0] * InOBB.m_A[0].GetLength();
			InOBB.m_fA[1] = OBB.m_fA[1] * InOBB.m_A[1].GetLength();
			InOBB.m_fA[2] = OBB.m_fA[2] * InOBB.m_A[2].GetLength();

			InOBB.m_A[0].Normalize();
			InOBB.m_A[1].Normalize();
			InOBB.m_A[2].Normalize();
		};
		static MWDAABB3 GetAABB(const MWDOBB3& InOBB ) {
			MWDOBB3 inobb = InOBB;
			MWDVector3 Point[8];
			MWD::GetPoint(inobb,Point);
			MWDAABB3 Temp;
			Temp.CreateAABB(Point, 8);
			return Temp;
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDVector3& Point, MWDREAL fOBBParameter[3]) {
			MWDVector3 Diff = Point - OBB3.m_Center;


			MWDREAL fSqrDistance = 0.0f, fDelta;

			fOBBParameter[0] = Diff.Dot(OBB3.m_A[0]);
			if (fOBBParameter[0] < -OBB3.m_fA[0])
			{
				fDelta = fOBBParameter[0] + OBB3.m_fA[0];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[0] = -OBB3.m_fA[0];
			}
			else if (fOBBParameter[0] > OBB3.m_fA[0])
			{
				fDelta = fOBBParameter[0] - OBB3.m_fA[0];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[0] = OBB3.m_fA[0];
			}

			fOBBParameter[1] = Diff.Dot(OBB3.m_A[1]);
			if (fOBBParameter[1] < -OBB3.m_fA[1])
			{
				fDelta = fOBBParameter[1] + OBB3.m_fA[1];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[1] = -OBB3.m_fA[1];
			}
			else if (fOBBParameter[1] > OBB3.m_fA[1])
			{
				fDelta = fOBBParameter[1] - OBB3.m_fA[1];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[1] = OBB3.m_fA[1];
			}

			fOBBParameter[2] = Diff.Dot(OBB3.m_A[2]);
			if (fOBBParameter[2] < -OBB3.m_fA[2])
			{
				fDelta = fOBBParameter[2] + OBB3.m_fA[2];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[2] = -OBB3.m_fA[2];
			}
			else if (fOBBParameter[2] > OBB3.m_fA[2])
			{
				fDelta = fOBBParameter[2] - OBB3.m_fA[2];
				fSqrDistance += fDelta * fDelta;
				fOBBParameter[2] = OBB3.m_fA[2];
			}
			return fSqrDistance;
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDLine3& Line, MWDREAL fOBBParameter[3], MWDREAL& fLineParameter) {
			MWDRectangle3 Rectangle[6];
			GetRectangle(OBB3,Rectangle);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL t, fAABB[3];
			for (int i = 0; i < 6; i++)
			{
				fSqrDistTmp = SquaredDistance(Line, OBB3, t, fAABB);
				if (fSqrDist < fSqrDistTmp)
				{
					fSqrDist = fSqrDistTmp;
					fLineParameter = t;
					for (int j = 0; j < 3; j++)
					{
						fOBBParameter[j] = fAABB[j];
					}

				}

			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDRay3& Ray, MWDREAL fOBBParameter[3], MWDREAL& fRayParameter) {
			return SquaredDistance(Ray, OBB3, fRayParameter, fOBBParameter);
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDSegment3& Segment, MWDREAL fOBBParameter[3], MWDREAL& fSegmentParameter) {
			return SquaredDistance(Segment, OBB3, fSegmentParameter, fOBBParameter);
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDTriangle3& Triangle, MWDREAL OBBParameter[3],MWDREAL TriangleParameter[3]) {
			MWDRectangle3 Rectangle[6];
			GetRectangle(OBB3,Rectangle);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL RectP[2], TriP[3];

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[0], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = OBB3.m_fA[0];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[1], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = -OBB3.m_fA[0];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = -RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[2], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = RectP[1];
				OBBParameter[1] = OBB3.m_fA[1];
				OBBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[3], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = -RectP[1];
				OBBParameter[1] = -OBB3.m_fA[1];
				OBBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[4], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = -RectP[1];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = OBB3.m_fA[2];
			}

			fSqrDistTmp = SquaredDistance(Triangle,Rectangle[5], TriP, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				TriangleParameter[0] = TriP[0];
				TriangleParameter[1] = TriP[1];
				TriangleParameter[2] = TriP[2];
				OBBParameter[0] = RectP[1];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = -OBB3.m_fA[2];
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDRectangle3& Rectangle, MWDREAL OBBParameter[3],MWDREAL RectangleParameter[2]) {
			MWDRectangle3 RectangleTemp[6];
			GetRectangle(OBB3,RectangleTemp);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL RectP[2], RectP1[2];

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[0], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = OBB3.m_fA[0];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[1], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = -OBB3.m_fA[0];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = -RectP[1];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[2], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = RectP[1];
				OBBParameter[1] = OBB3.m_fA[1];
				OBBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[3], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = -RectP[1];
				OBBParameter[1] = -OBB3.m_fA[1];
				OBBParameter[2] = RectP[0];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[4], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = -RectP[1];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = OBB3.m_fA[2];
			}

			fSqrDistTmp = SquaredDistance(Rectangle,RectangleTemp[5], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				RectangleParameter[0] = RectP1[0];
				RectangleParameter[1] = RectP1[1];

				OBBParameter[0] = RectP[1];
				OBBParameter[1] = RectP[0];
				OBBParameter[2] = -OBB3.m_fA[2];
			}
			return fSqrDist;
		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDOBB3& OBB, MWDREAL OBB1Parameter[3],MWDREAL OBB2Parameter[3]) {
			MWDRectangle3 RectangleTemp[6];
			GetRectangle(OBB3,RectangleTemp);
			MWDREAL fSqrDist = MWDMAX_REAL;
			MWDREAL fSqrDistTmp;
			MWDREAL RectP[2], RectP1[3];

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[0], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = OBB3.m_fA[0];
				OBB1Parameter[1] = RectP[0];
				OBB1Parameter[2] = RectP[1];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[1], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = -OBB3.m_fA[0];
				OBB1Parameter[1] = RectP[0];
				OBB1Parameter[2] = -RectP[1];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[2], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = RectP[1];
				OBB1Parameter[1] = OBB3.m_fA[1];
				OBB1Parameter[2] = RectP[0];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[3], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = -RectP[1];
				OBB1Parameter[1] = -OBB3.m_fA[1];
				OBB1Parameter[2] = RectP[0];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[4], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = -RectP[1];
				OBB1Parameter[1] = RectP[0];
				OBB1Parameter[2] = OBB3.m_fA[2];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}

			fSqrDistTmp = SquaredDistance(OBB,RectangleTemp[5], RectP1, RectP);
			if (fSqrDistTmp < fSqrDist)
			{
				fSqrDist = fSqrDistTmp;
				OBB1Parameter[0] = RectP[1];
				OBB1Parameter[1] = RectP[0];
				OBB1Parameter[2] = -OBB3.m_fA[2];

				OBB2Parameter[0] = RectP1[0];
				OBB2Parameter[1] = RectP1[0];
				OBB2Parameter[2] = RectP1[1];
			}
			return fSqrDist;
		};
		static MWDREAL Distance(const MWDOBB3& OBB3, const MWDSphere3& Sphere, MWDREAL fOBBParameter[3], MWDVector3& SpherePoint) {
			return Distance(Sphere, OBB3, SpherePoint, fOBBParameter);

		};
		static MWDREAL Distance(const MWDOBB3& OBB3, const MWDPlane3& Plane, MWDVector3& OBBPoint, MWDVector3& PlanePoint) {
			return Distance(Plane, OBB3, PlanePoint, OBBPoint);

		};
		static MWDREAL SquaredDistance(const MWDOBB3& OBB3, const MWDPolygon3& Polygon,MWDREAL OBBParameter[3], int& IndexTriangle,MWDREAL TriangleParameter[3]) {
			return SquaredDistance(Polygon, OBB3, IndexTriangle, TriangleParameter, OBBParameter);

		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDVector3& Point) {
			return RelationWith(Point, OBB3);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDLine3& Line, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Line, OBB3, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDRay3& Ray, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Ray, OBB3, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDSegment3& Segment, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Segment, OBB3, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDPlane3& Plane) {
			MWDVector3 N = Plane.GetN();
			MWDREAL fD = Plane.GetfD();
			MWDREAL fRadius = ABS(OBB3.m_fA[0] * (N.Dot(OBB3.m_A[0])))
				+ ABS(OBB3.m_fA[1] * (N.Dot(OBB3.m_A[1])))
				+ ABS(OBB3.m_fA[2] * (N.Dot(OBB3.m_A[2])));


			MWDREAL fTest = N.Dot(OBB3.m_Center) + fD;


			if (fTest > fRadius)
				return MWDFRONT;
			else if (fTest < -fRadius)
				return MWDBACK;
			else
				return MWDINTERSECT;

		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDTriangle3& Triangle) {
			return RelationWith(Triangle, OBB3);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDRectangle3& Rectangle) {
			return RelationWith(Rectangle, OBB3);
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDOBB3& OBB) {
			MWDREAL T[3];


			MWDVector3 vcD = OBB.m_Center - OBB3. m_Center;

			MWDREAL matM[3][3];
			MWDREAL ra,
				rb,
				t;

			MWDVector3 MA0 = OBB3.m_A[0], MA1 = OBB3.m_A[1], MA2 = OBB3.m_A[2];

			matM[0][0] = MA0.Dot(OBB.m_A[0]);
			matM[0][1] = MA0.Dot(OBB.m_A[1]);
			matM[0][2] = MA0.Dot(OBB.m_A[2]);
			ra = OBB3.m_fA[0];
			rb = OBB.m_fA[0] * ABS(matM[0][0]) +
				OBB.m_fA[1] * ABS(matM[0][1]) +
				OBB.m_fA[2] * ABS(matM[0][2]);

			T[0] = vcD.Dot(OBB3.m_A[0]);
			t = ABS(T[0]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			matM[1][0] = MA1.Dot(OBB.m_A[0]);
			matM[1][1] = MA1.Dot(OBB.m_A[1]);
			matM[1][2] = MA1.Dot(OBB.m_A[2]);
			ra = OBB3.m_fA[1];
			rb = OBB.m_fA[0] * ABS(matM[1][0]) +
				OBB.m_fA[1] * ABS(matM[1][1]) +
				OBB.m_fA[2] * ABS(matM[1][2]);
			T[1] = vcD.Dot(OBB3.m_A[1]);
			t = ABS(T[1]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			matM[2][0] = MA2.Dot(OBB.m_A[0]);
			matM[2][1] = MA2.Dot(OBB.m_A[1]);
			matM[2][2] = MA2.Dot(OBB.m_A[2]);
			ra = OBB3.m_fA[2];
			rb = OBB.m_fA[0] * ABS(matM[2][0]) +
				OBB.m_fA[1] * ABS(matM[2][1]) +
				OBB.m_fA[2] * ABS(matM[2][2]);
			T[2] = vcD.Dot(OBB3.m_A[2]);
			t = ABS(T[2]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[0][0]) +
				OBB3.m_fA[1] * ABS(matM[1][0]) +
				OBB3.m_fA[2] * ABS(matM[2][0]);
			rb = OBB.m_fA[0];
			t = ABS(T[0] * matM[0][0] + T[1] * matM[1][0] + T[2] * matM[2][0]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[0][1]) +
				OBB3.m_fA[1] * ABS(matM[1][1]) +
				OBB3.m_fA[2] * ABS(matM[2][1]);
			rb = OBB.m_fA[1];
			t = ABS(T[0] * matM[0][1] + T[1] * matM[1][1] + T[2] * matM[2][1]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[0][2]) +
				OBB3.m_fA[1] * ABS(matM[1][2]) +
				OBB3.m_fA[2] * ABS(matM[2][2]);
			rb = OBB.m_fA[2];
			t = ABS(T[0] * matM[0][2] + T[1] * matM[1][2] + T[2] * matM[2][2]);
			if (t > (ra + rb))
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[1] * ABS(matM[2][0]) + OBB3.m_fA[2] * ABS(matM[1][0]);
			rb = OBB.m_fA[1] * ABS(matM[0][2]) + OBB.m_fA[2] * ABS(matM[0][1]);
			t = ABS(T[2] * matM[1][0] - T[1] * matM[2][0]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[1] * ABS(matM[2][1]) + OBB3.m_fA[2] * ABS(matM[1][1]);
			rb = OBB.m_fA[0] * ABS(matM[0][2]) + OBB.m_fA[2] * ABS(matM[0][0]);
			t = ABS(T[2] * matM[1][1] - T[1] * matM[2][1]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[1] * ABS(matM[2][2]) + OBB3.m_fA[2] * ABS(matM[1][2]);
			rb = OBB.m_fA[0] * ABS(matM[0][1]) + OBB.m_fA[1] * ABS(matM[0][0]);
			t = ABS(T[2] * matM[1][2] - T[1] * matM[2][2]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[2][0]) + OBB3.m_fA[2] * ABS(matM[0][0]);
			rb = OBB.m_fA[1] * ABS(matM[1][2]) + OBB.m_fA[2] * ABS(matM[1][1]);
			t = ABS(T[0] * matM[2][0] - T[2] * matM[0][0]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[2][1]) + OBB3.m_fA[2] * ABS(matM[0][1]);
			rb = OBB.m_fA[0] * ABS(matM[1][2]) + OBB.m_fA[2] * ABS(matM[1][0]);
			t = ABS(T[0] * matM[2][1] - T[2] * matM[0][1]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[2][2]) + OBB3.m_fA[2] * ABS(matM[0][2]);
			rb = OBB.m_fA[0] * ABS(matM[1][1]) + OBB.m_fA[1] * ABS(matM[1][0]);
			t = ABS(T[0] * matM[2][2] - T[2] * matM[0][2]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[1][0]) + OBB3.m_fA[1] * ABS(matM[0][0]);
			rb = OBB.m_fA[1] * ABS(matM[2][2]) + OBB.m_fA[2] * ABS(matM[2][1]);
			t = ABS(T[1] * matM[0][0] - T[0] * matM[1][0]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[1][1]) + OBB3.m_fA[1] * ABS(matM[0][1]);
			rb = OBB.m_fA[0] * ABS(matM[2][2]) + OBB.m_fA[2] * ABS(matM[2][0]);
			t = ABS(T[1] * matM[0][1] - T[0] * matM[1][1]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			ra = OBB3.m_fA[0] * ABS(matM[1][2]) + OBB3.m_fA[1] * ABS(matM[0][2]);
			rb = OBB.m_fA[0] * ABS(matM[2][1]) + OBB.m_fA[1] * ABS(matM[2][0]);
			t = ABS(T[1] * matM[0][2] - T[0] * matM[1][2]);
			if (t > ra + rb)
				return MWDNOINTERSECT;


			return MWDINTERSECT;
		};
		static int RelationWith(const MWDOBB3& OBB3, const MWDSphere3& Sphere) {
			MWDREAL fOBBParameter[3];
			MWDREAL Dist = SquaredDistance(Sphere.m_Center, OBB3, fOBBParameter);
			Dist = SQRT(Dist);
			if (Dist - Sphere.m_fRadius < 0.0f)
				return MWDINTERSECT;

			return MWDNOINTERSECT;
		};

		//Sphere3
		static void Transform( MWDSphere3& inSph,const MWDSphere3& Sphere, const MWDMatrix3X3W& Mat) {
			inSph.m_Center =MWD::Vec3MulMat3x3W(Sphere.m_Center , Mat) ;
			MWDVector3 V0(1.0f, 0.0f, 0.0f), V1(0.0f, 1.0f, 0.0f), V2(0.0f, 0.0f, 1.0f);
			MWDVector3 Temp0 = Sphere.m_Center + V0 * Sphere.m_fRadius;
			MWDVector3 Temp1 = Sphere.m_Center + V1 * Sphere.m_fRadius;
			MWDVector3 Temp2 = Sphere.m_Center + V2 * Sphere.m_fRadius;

			Temp0 = MWD::Vec3MulMat3x3W(Temp0, Mat);
			Temp1 = MWD::Vec3MulMat3x3W(Temp1, Mat);
			Temp2 = MWD::Vec3MulMat3x3W(Temp2, Mat);

			MWDREAL fRadiusTemp;

			inSph.m_fRadius = (Temp0 - inSph.m_Center).GetLength();

			fRadiusTemp = (Temp1 - inSph.m_Center).GetLength();

			if (inSph.m_fRadius < fRadiusTemp)
				inSph.m_fRadius = fRadiusTemp;

			fRadiusTemp = (Temp2 - inSph.m_Center).GetLength();

			if (inSph.m_fRadius < fRadiusTemp)
				inSph.m_fRadius = fRadiusTemp;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDVector3& Point, MWDVector3& SpherePoint) {
			MWDREAL sqrDist = SquaredDistance(Point,S.m_Center);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;
			MWDLine3 Line(S.m_Center, Point);

			SpherePoint = Line.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDLine3& Line, MWDVector3& SpherePoint, MWDREAL& fLineParameter) {
			MWDREAL sqrDist = SquaredDistance(Line,S.m_Center, fLineParameter);
			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, Line.GetParameterPoint(fLineParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDRay3& Ray, MWDVector3& SpherePoint, MWDREAL& fRayParameter) {
			MWDREAL sqrDist = SquaredDistance(Ray,S.m_Center, fRayParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, Ray.GetParameterPoint(fRayParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDSegment3& Segment, MWDVector3& SpherePoint, MWDREAL& fSegmentParameter) {
			MWDREAL sqrDist = SquaredDistance(Segment,S.m_Center, fSegmentParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, Segment.GetParameterPoint(fSegmentParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDOBB3& OBB, MWDVector3& SpherePoint, MWDREAL fOBBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(OBB,S.m_Center, fOBBParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, OBB.GetParameterPoint(fOBBParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDPlane3& Plane, MWDVector3& SpherePoint) {
			MWDVector3 PlanePoint;
			MWDREAL sqrDist = Distance(Plane,S.m_Center, PlanePoint);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, PlanePoint);

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDRectangle3& Rectangle, MWDVector3& SpherePoint, MWDREAL fRectangleParameter[2]) {
			MWDREAL sqrDist = SquaredDistance(Rectangle,S.m_Center, fRectangleParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, Rectangle.GetParameterPoint(fRectangleParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDTriangle3 Triangle, MWDVector3& SpherePoint, MWDREAL fTriangleParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(Triangle,S.m_Center, fTriangleParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, Triangle.GetParameterPoint(fTriangleParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDAABB3& AABB, MWDVector3& SpherePoint, MWDREAL fAABBParameter[3]) {
			MWDREAL sqrDist = SquaredDistance(AABB,S.m_Center, fAABBParameter);

			sqrDist = SQRT(sqrDist);
			sqrDist = sqrDist - S.m_fRadius;

			MWDLine3 LineTemp(S.m_Center, AABB.GetParameterPoint(fAABBParameter));

			SpherePoint = LineTemp.GetParameterPoint(S.m_fRadius);
			return sqrDist;
		};
		static MWDREAL Distance(const MWDSphere3& S, const MWDPolygon3& Polygon, MWDVector3& SpherePoint, int& IndexTriangle,MWDREAL fTriangleParameter[3]) {
			return Distance(Polygon,S, IndexTriangle, fTriangleParameter, SpherePoint);
		};
		static int RelationWith(const MWDSphere3& S, const MWDVector3& Point) {
			return RelationWith(Point,S);
		};
		static int RelationWith(const MWDSphere3& S, const MWDLine3& Line, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Line,S, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDSphere3& S, const MWDRay3& Ray, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Ray,S, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDSphere3& S, const MWDSegment3& Segment, unsigned int& Quantity, MWDREAL& tNear, MWDREAL& tFar) {
			return RelationWith(Segment,S, Quantity, tNear, tFar);
		};
		static int RelationWith(const MWDSphere3& S, const MWDPlane3& Plane) {
			MWDVector3 N = Plane.GetN();
			MWDREAL fD = Plane.GetfD();
			MWDVector3 center = S.m_Center;
			MWDREAL test =center .Dot(N) + fD;
			if (test > S.m_fRadius)
				return MWDFRONT;
			else if (test < -S.m_fRadius)
				return MWDBACK;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDSphere3& S, const MWDTriangle3 Triangle) {
			return RelationWith(Triangle,S);
		};
		static int RelationWith(const MWDSphere3& S, const MWDRectangle3& Rectangle) {
			return RelationWith(Rectangle,S);
		};
		static int RelationWith(const MWDSphere3& S, const MWDOBB3& OBB) {

			return RelationWith(OBB,S);
		};
		static int RelationWith(const MWDSphere3& S, const MWDSphere3& Sphere) {
			MWDREAL Sum = S.m_fRadius + Sphere.m_fRadius;

			Sum *= Sum;

			MWDVector3 Sub = S.m_Center - Sphere.m_Center;

			if (Sub.GetSqrLength() > Sum)
				return MWDNOINTERSECT;

			return MWDINTERSECT;
		};

		//Polygon3
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol,const MWDVector3& Point, int& IndexTriangle, MWDREAL fTriangleParameter[3]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fParaTemp[3];
				MWDREAL dist = SquaredDistance(TriangleTemp,Point, fParaTemp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
						fTriangleParameter[j] = fParaTemp[j];
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDTriangle3& Triangle,int& IndexTriangle,MWDREAL fTriangle1Parameter[3],MWDREAL fTriangle2Parameter[3]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2Temp[3];
				MWDREAL dist = SquaredDistance(TriangleTemp,Triangle, fPara1Temp, fPara2Temp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangle1Parameter[j] = fPara1Temp[j];
						fTriangle2Parameter[j] = fPara2Temp[j];
					}
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDRectangle3& Rectangle,int& IndexTriangle,MWDREAL fTriangleParameter[3],MWDREAL fRectangleParameter[2]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2Temp[2];
				MWDREAL dist = SquaredDistance(TriangleTemp,Rectangle, fPara1Temp, fPara2Temp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangleParameter[j] = fPara1Temp[j];

					}
					for (int k = 0; k < 2; k++)
					{
						fRectangleParameter[k] = fPara2Temp[k];
					}
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDLine3& Line, int& IndexTriangle,MWDREAL fTriangleParameter[3], MWDREAL& fLineParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2;
				MWDREAL dist = SquaredDistance(TriangleTemp,Line, fPara1Temp, fPara2);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangleParameter[j] = fPara1Temp[j];
					}
					fLineParameter = fPara2;
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDRay3& Ray, int& IndexTriangle,MWDREAL fTriangleParameter[3], MWDREAL& fRayParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2;
				MWDREAL dist = SquaredDistance(TriangleTemp,Ray, fPara1Temp, fPara2);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangleParameter[j] = fPara1Temp[j];
					}
					fRayParameter = fPara2;
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDSegment3& Segment, int& IndexTriangle,MWDREAL fTriangleParameter[3], MWDREAL& fSegmentParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2;
				MWDREAL dist = SquaredDistance(TriangleTemp,Segment, fPara1Temp, fPara2);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangleParameter[j] = fPara1Temp[j];
					}
					fSegmentParameter = fPara2;
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDOBB3& OBB, int& IndexTriangle,MWDREAL TriangleParameter[3], MWDREAL OBBParameter[3]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2Temp[3];
				MWDREAL dist = SquaredDistance(TriangleTemp,OBB, fPara1Temp, fPara2Temp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						TriangleParameter[j] = fPara1Temp[j];
						OBBParameter[j] = fPara2Temp[j];
					}
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPolygon3& Pol, const MWDSphere3& Sphere, int& IndexTriangle,MWDREAL fTriangleParameter[3], MWDVector3& SpherePoint) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDVector3 Point;
				MWDREAL dist = Distance(TriangleTemp,Sphere, fPara1Temp, Point);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						fTriangleParameter[j] = fPara1Temp[j];

					}
					SpherePoint = Point;
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL Distance(const MWDPolygon3& Pol, const MWDPlane3& Plane, int& IndexTriangle,MWDVector3& TrianglePoint, MWDVector3& PlanePoint) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDVector3 Point1, Point2;
				MWDREAL dist = Distance(TriangleTemp,Plane, Point1, Point2);
				if (distTemp < dist)
				{
					distTemp = dist;

					TrianglePoint = Point1;
					PlanePoint = Point2;
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDAABB3& AABB, int& IndexTriangle,MWDREAL TriangleParameter[3], MWDREAL AABBParameter[3]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2Temp[3];
				MWDREAL dist = SquaredDistance(TriangleTemp,AABB, fPara1Temp, fPara2Temp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						TriangleParameter[j] = fPara1Temp[j];
						AABBParameter[j] = fPara2Temp[j];
					}
					IndexTriangle = i;
				}

			}
			return distTemp;
		};
		static MWDREAL SquaredDistance(const MWDPolygon3& Pol, const MWDPolygon3& Polygon, int& Index1Triangle,MWDREAL Triangle1Parameter[3], int& Index2Triangle,MWDREAL Triangle2Parameter[3]) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				MWDREAL fPara1Temp[3];
				MWDREAL fPara2Temp[3];
				int k;
				MWDREAL dist = SquaredDistance(TriangleTemp,Polygon, fPara1Temp, k, fPara2Temp);
				if (distTemp < dist)
				{
					distTemp = dist;
					for (int j = 0; j < 3; j++)
					{
						Triangle1Parameter[j] = fPara1Temp[j];
						Triangle2Parameter[j] = fPara2Temp[j];
					}
					Index1Triangle = i;
					Index2Triangle = k;
				}

			}
			return distTemp;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDLine3& Line, bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3],MWDREAL& fLineParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				int iFlag = RelationWith(TriangleTemp,Line, bCull, fTriangleParameter, fLineParameter);
				if (iFlag == MWDINTERSECT)
					return MWDINTERSECT;

			}

			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDRay3& Ray, bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3],MWDREAL& fRayParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				int iFlag = RelationWith(TriangleTemp,Ray, bCull, fTriangleParameter, fRayParameter);
				iIndexTriangle = i;
				if (iFlag == MWDINTERSECT)
					return MWDINTERSECT;

			}

			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDSegment3& Segment, bool bCull, int& iIndexTriangle, MWDREAL fTriangleParameter[3],MWDREAL& fSegmentParameter) {
			int iTriangleNum = Pol.m_IndexNum / 3;
			MWDTriangle3 TriangleTemp;
			MWDREAL distTemp = MWDMAX_REAL;

			for (int i = 0; i < iTriangleNum; i++)
			{
				TriangleTemp.Set(Pol.m_pPoint[Pol.m_pIndex[i]], Pol.m_pPoint[Pol.m_pIndex[i + 1]], Pol.m_pPoint[Pol.m_pIndex[i + 2]]);
				int iFlag = RelationWith(TriangleTemp,Segment, bCull, fTriangleParameter, fSegmentParameter);
				iIndexTriangle = i;
				if (iFlag == MWDINTERSECT)
					return MWDINTERSECT;

			}

			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDPlane3& Plane) {
			MWDSegment3 Segment;
			int iBackNum = 0, iFrontNum = 0;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{

				int iFlag = RelationWith(Pol.m_pPoint[i],Plane);
				if (iFlag == MWDBACK)
					iBackNum++;
				else if (iFlag == MWDFRONT)
					iFrontNum++;

				if (iBackNum > 0 && iFrontNum > 0)
					return MWDINTERSECT;

			}
			if (iBackNum == 0 && iFrontNum == 0)
				return MWDON;
			else if (iBackNum == 0)
				return MWDFRONT;
			else if (iFrontNum == 0)
				return MWDBACK;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDPlane3& Plane, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			int OnNum = 0, BackNum = 0, FrontNum = 0;
			MWDVector3 Orig, End;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
					j = 0;
				SegmentTemp.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
				MWDREAL t;
				int iFlag = RelationWith(SegmentTemp,Plane, 0, t);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(t);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(t);
						Segment.Set(Orig, End);
						return MWDINTERSECT;

					}
				}
				else if (iFlag == MWDON)
				{
					OnNum++;

				}
				else if (iFlag == MWDBACK)
				{
					BackNum++;

				}
				else if (iFlag == MWDFRONT)
				{
					FrontNum++;
				}

			}
			if (OnNum == 3)
				return MWDON;
			if (BackNum == 3)
				return MWDBACK;
			if (FrontNum == 3)
				return MWDFRONT;
			//not control all path
			return MWDINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDTriangle3& Triangle) {
			MWDSegment3 Segment;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
					j = 0;
				Segment.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[3];
				if (RelationWith(Segment,Triangle, 0, fSegmentParameter, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			MWDVector3 Point[3];
			Triangle.GetPoint(Point);
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				Segment.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[3];
				int Index;
				if (RelationWith(Segment, Pol, fSegmentParameter, 0, Index, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDTriangle3& Triangle, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			MWDVector3 Orig, End;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
					j = 0;
				Segment.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
				MWDREAL fTriangleParameter[3];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,Triangle, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}
			MWDVector3 Point[3];
			Triangle.GetPoint(Point);
			for (int i = 0; i < 3; i++)
			{
				int j = i + 1;
				if (j == 3)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL fTriangleParameter[3];
				int Index;
				int iFlag = RelationWith(SegmentTemp,Pol, fSegmentParameter, 0, Index, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDRectangle3& Rectangle) {
			MWDSegment3 Segment;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
					j = 0;
				Segment.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[2];
				if (RelationWith(Segment,Rectangle, 0, fSegmentParameter, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			MWDVector3 Point[4];
			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				Segment.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL TriangleParameter[3];
				int Index;
				if (RelationWith(Segment, Pol, fSegmentParameter, 0, Index, TriangleParameter) == MWDINTERSECT)
					return MWDINTERSECT;

			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDRectangle3& Rectangle, MWDSegment3& Segment) {
			MWDSegment3 SegmentTemp;
			int InNum = 0;
			MWDVector3 Orig, End;
			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
					j = 0;
				Segment.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
				MWDREAL fTriangleParameter[2];
				MWDREAL fSegmentParameter;
				int iFlag = RelationWith(SegmentTemp,Rectangle, 0, fSegmentParameter, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}


			}
			MWDVector3 Point[4];
			Rectangle.GetPoint(Point);
			for (int i = 0; i < 4; i++)
			{
				int j = i + 1;
				if (j == 4)
					j = 0;
				SegmentTemp.Set(Point[i], Point[j]);
				MWDREAL fSegmentParameter;
				MWDREAL fTriangleParameter[3];
				int Index;
				int iFlag = RelationWith(SegmentTemp, Pol, fSegmentParameter, 0, Index, fTriangleParameter);
				if (iFlag == MWDINTERSECT)
				{
					InNum++;
					if (InNum == 1)
					{
						Orig = SegmentTemp.GetParameterPoint(fSegmentParameter);

					}
					else if (InNum == 2)
					{
						End = SegmentTemp.GetParameterPoint(fSegmentParameter);
						Segment.Set(Orig, End);
						return MWDINTERSECT;
					}
				}
			}
			return MWDNOINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDAABB3& AABB) {
			MWDPlane3 Plane = Pol.GetPlane();
			if (RelationWith(Plane,AABB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;

			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int iFlag = RelationWith(Pol.m_pPoint[i],AABB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDOBB3& OBB) {
			MWDPlane3 Plane = Pol.GetPlane();
			if (RelationWith(Plane,OBB) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;

			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int iFlag = RelationWith(Pol.m_pPoint[i],OBB);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;
		};
		static int RelationWith(const MWDPolygon3& Pol, const MWDSphere3& Sphere) {
			MWDPlane3 Plane = Pol.GetPlane();
			if (RelationWith(Plane,Sphere) != MWDINTERSECT)
				return MWDNOINTERSECT;
			int InNum = 0, OutNum = 0;

			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int iFlag = RelationWith(Pol.m_pPoint[i],Sphere);
				if (iFlag == MWDIN || iFlag == MWDON)
				{
					InNum++;

				}
				else if (iFlag == MWDOUT)
				{

					OutNum++;
				}
				if (InNum > 0 && OutNum > 0)
					return MWDINTERSECT;
			}
			if (InNum == 0)
				return MWDOUT;
			if (OutNum == 0)
				return MWDIN;
			else
				return MWDINTERSECT;

		};
		static void	Clip(const MWDPolygon3& Pol, const MWDPlane3& Plane,MWDPolygon3& pFront,MWDPolygon3& pBack) {
			int			nNumFront = 0,
				nNumBack = 0,
				nLoop = 0,
				nCurrent = 0;

			MWDVector3* pvcFront = new MWDVector3[Pol.m_PointNum * 3];
			MWDVector3* pvcBack = new MWDVector3[Pol.m_PointNum * 3];

			for (unsigned int i = 0; i < Pol.m_PointNum; i++)
			{
				int j = i + 1;
				if (j == Pol.m_PointNum)
				{
					j = 0;
				}
				int iFlagi = RelationWith(Plane,Pol.m_pPoint[i]);
				int iFlagj = RelationWith(Plane,Pol.m_pPoint[j]);
				if (iFlagi == MWDON)
				{
					pvcFront[nNumFront] = Pol.m_pPoint[i];
					nNumFront++;
					pvcBack[nNumBack] = Pol.m_pPoint[i];
					nNumBack++;
				}
				else
				{
					MWDSegment3 Segment;
					Segment.Set(Pol.m_pPoint[i], Pol.m_pPoint[j]);
					MWDREAL t;
					if (RelationWith(Segment,Plane, 0, t) == MWDINTERSECT && iFlagj != MWDON)
					{
						MWDVector3 Hit = Segment.GetParameterPoint(t);
						pvcFront[nNumFront] = Hit;
						nNumFront++;
						pvcBack[nNumBack] = Hit;
						nNumBack++;
					}

					if (iFlagi == MWDFRONT)
					{

						pvcFront[nNumFront++] = Pol.m_pPoint[i];
					}
					else if (iFlagi == MWDBACK)
					{

						pvcBack[nNumBack++] = Pol.m_pPoint[i];
					}

				}
			}



			pFront.Set(pvcFront, nNumFront);
			if (pFront.m_N.Dot(Pol.m_N) < 0.0f)
				pFront.SwapFaces();
			pBack.Set(pvcBack, nNumBack);
			if (pBack.m_N.Dot(Pol.m_N) < 0.0f)
				pBack.SwapFaces();

			MWDMAC_DELETEA(pvcFront);
			MWDMAC_DELETEA(pvcBack);
		};
		static void	Clip(MWDPolygon3& Pol, const MWDAABB3& ABBB) {
			MWDPlane3 Plane[6];
			GetPlane(ABBB,Plane);
			MWDPolygon3 Front, Back;
			for (int i = 0; i < 6; i++)
			{
				if (RelationWith(Pol,Plane[i]) == MWDINTERSECT)
				{
					Clip(Pol,Plane[i], Front, Back);
					Pol.CopyFrom(Back);
				}
			}
		};

		//Cylinder3

		//










	};
	



}