#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDAABB3.h"
namespace MWDEngine {
	class MWDMATH_API MWDSphere3
	{
	public:
		MWDVector3 m_Center;		
		MWDREAL m_fRadius;		
		MWDSphere3() {
			
		};
		~MWDSphere3() {
			m_fRadius = 0.0f;
		};
		MWDSphere3(const MWDVector3& Center, MWDREAL fRadius) {
			Set(Center, fRadius);
		};
		void CreateSphere(const MWDVector3* pPointArray, unsigned int uiPointNum) {
			if (!pPointArray || !uiPointNum)
				return;
			MWDVector3 Sum;
			for (unsigned int i = 0; i < uiPointNum; i++)
				Sum += pPointArray[i];
			m_Center = Sum / (uiPointNum * 1.0f);
			MWDREAL radiusSqr = 0.0f;
			for (unsigned int i = 0; i < uiPointNum; i++)
			{
				MWDVector3 diff = pPointArray[i] - m_Center;
				MWDREAL Temp = diff.GetSqrLength();
				if (Temp > radiusSqr)
					radiusSqr = Temp;

			}
			m_fRadius = SQRT(radiusSqr);
		};
		MWDSphere3 MergSpheres(const MWDSphere3& Sphere)const {
			MWDSphere3 Temp;
			MWDVector3 CenterDiff = m_Center - Sphere.m_Center;
			MWDREAL fRadiusDiff = m_fRadius - Sphere.m_fRadius;
			MWDREAL fRadiusDiffSqr = fRadiusDiff * fRadiusDiff;
			MWDREAL Lsqr = CenterDiff.GetSqrLength();
			if (fRadiusDiffSqr >= Lsqr)
			{
				if (fRadiusDiff >= 0.0f)
				{
					Temp.m_Center = m_Center;
					Temp.m_fRadius = m_fRadius;
				}
				else
				{
					Temp.m_Center = Sphere.m_Center;
					Temp.m_fRadius = Sphere.m_fRadius;
				}


			}
			else
			{
				MWDREAL L = SQRT(Lsqr);
				MWDREAL t = (L + m_fRadius - Sphere.m_fRadius) / (2.0f * L);
				Temp.m_Center = Sphere.m_Center + CenterDiff * t;
				Temp.m_fRadius = (L + m_fRadius + Sphere.m_fRadius) / 2.0f;
			}

			return Temp;
		};
		MWDAABB3 GetAABB()const {
			MWDVector3 Max = m_Center + MWDVector3(m_fRadius, m_fRadius, m_fRadius);
			MWDVector3 Min = m_Center + MWDVector3(-m_fRadius, -m_fRadius, -m_fRadius);
			return MWDAABB3(Max, Min);
		};
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void Set(const MWDVector3& Center, MWDREAL fRadius) {
			m_Center = Center;
			m_fRadius = fRadius;
		};

	};

}
