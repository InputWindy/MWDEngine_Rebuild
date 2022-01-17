#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDMatrix3X3W.h"
namespace MWDEngine {
	class MWDMATH_API MWDCurve3 : public MWDMemObject
	{
	public:
		enum //CURVE TYPE
		{
			CT_SEGMENT,
			CT_CONTROL,
			CT_MAX
		};
		MWDCurve3() {
			
		};
		virtual ~MWDCurve3() {
			
		};

		virtual unsigned int GetCurveType() = 0;
		virtual void GetFrenetFrameBNT(MWDREAL t, MWDVector3& B, MWDVector3& N, MWDVector3& T) = 0;
		
		void	GetFixedUpBNT(MWDREAL t, MWDVector3& B, MWDVector3& N, MWDVector3& T) {
			MWDMatrix3X3W Mat;
			MWDVector3 Point(0, 0, 0);
			MWDVector3 FD = GetFirstDerivative(t);
			FD.Normalize();
			Mat.CreateFromLookAt(Point, FD);
			B.x = Mat._00;
			B.y = Mat._10;
			B.z = Mat._20;

			N.x = Mat._01;
			N.y = Mat._11;
			N.z = Mat._21;

			T.x = Mat._02;
			T.y = Mat._12;
			T.z = Mat._22;
		};
		
		virtual MWDVector3	GetFirstDerivative(MWDREAL t) = 0;
		virtual MWDVector3	GetSecondDerivative(MWDREAL t) = 0;
		virtual MWDVector3	GetThirdDerivative(MWDREAL t) = 0;
		
		virtual MWDREAL		GetLength(MWDREAL t1, MWDREAL t2, unsigned int uiIterations) = 0;
		
		virtual MWDVector3	GetPoint(MWDREAL t) = 0;
		
		virtual MWDREAL		GetTotalLength(unsigned int uiIterations) = 0;
		
		virtual MWDREAL GetTime(MWDREAL fLength, unsigned int iIterations = 32,
			MWDREAL fTolerance = EPSILON_E4) = 0;
		
		virtual MWDREAL GetCurvature(MWDREAL fTime) = 0;
		
		virtual MWDREAL GetTorsion(MWDREAL fTime) = 0;

	};

}
