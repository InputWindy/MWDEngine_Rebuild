#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class  MWDMATH_API MWDPlane3
	{
	public:
		MWDVector3	m_N;       //
		MWDREAL		m_fD;       // (ax+by+cz+d=0)
	public:
		MWDPlane3() {
			m_fD = 0.0f;
		};
		MWDPlane3(const MWDVector3& N, const MWDVector3& P) {
			Set(N, P);
		};
		MWDPlane3(const MWDVector3& P0, const MWDVector3& P1, const MWDVector3& P2) {
			Set(P0, P1, P2);
		};
		MWDPlane3(const MWDVector3 Point[3]) {
			Set(Point[0], Point[1], Point[2]);
		};
		MWDPlane3(const MWDVector3& N, MWDREAL fD) {
			Set(N, fD);
		};
		
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE const MWDVector3& GetN()const {
			return m_N;
		};
		FORCEINLINE MWDVector3 GetPoint()const {
			if (ABS(m_N.z) < EPSILON_E4)
			{
				return MWDVector3(0.0f, 0.0f, 0.0f);
			}
			return MWDVector3(0.0f, 0.0f, -m_fD / m_N.z);
		};
		FORCEINLINE MWDREAL GetfD()const {
			return m_fD;
		};
		FORCEINLINE void  Set(const MWDVector3& N, const MWDVector3& P) {
			m_N = N;
			m_N.Normalize();
			m_fD = -(m_N.Dot(P));
		};
		FORCEINLINE void  Set(const MWDVector3& N, MWDREAL fD) {
			MWDREAL Len = N.GetLength();
			m_N = N / Len;
			m_fD = fD / Len;
		};
		FORCEINLINE void  Set(const MWDVector3& P0, const MWDVector3& P1, const MWDVector3& P2) {
			MWDVector3 vcEdge1 = P1 - P0;
			MWDVector3 vcEdge2 = P2 - P0;

			m_N.Cross(vcEdge1, vcEdge2);
			m_N.Normalize();
			m_fD = -(m_N.Dot(P0));
		};
		FORCEINLINE void  Set(const MWDVector3 Point[3]) {
			Set(Point[0], Point[1], Point[2]);
		};
		FORCEINLINE MWDPlane3 GetPlane()const {
			return *this;
		};
		MWDVector3 ReflectDir(const MWDVector3& Dir)const {
			MWDVector3 TempN = m_N * (-1.0f),dir = Dir;
			return Dir - m_N * dir.Dot(m_N) * 2.0f;
		};
		
	}; 

}
