#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDPlane3.h"

/********************************************************************************************


__________________
|        |        |
|    A[0]|        |
|		  |        |
|        |_A[1]___|
|                 |
|                 |
|_________________|


********************************************************************************************/

namespace MWDEngine {
	class MWDMATH_API MWDRectangle3 :public MWDPlane3
	{
	public:
		MWDVector3 m_A[2];
		MWDREAL m_fA[2];
		MWDVector3 m_Center;
	public:
		MWDRectangle3() {
			
		};
		~MWDRectangle3() {

		};
		MWDRectangle3(const MWDVector3& Center, const MWDVector3& A0, const MWDVector3& A1, MWDREAL fA0, MWDREAL fA1) {
			Set(Center, A0, A1, fA0, fA1);
		};
		MWDRectangle3(const MWDVector3& Center, const MWDVector3 A[2], const MWDREAL fA[2]) {
			Set(Center, A, fA);
		};
		
		/*************************************FORCEINLINE************************************************/
		FORCEINLINE void Set(const MWDVector3& Center, const MWDVector3& A0, const MWDVector3& A1, MWDREAL fA0, MWDREAL fA1) {
			m_Center = Center;
			m_A[0] = A0;
			m_A[1] = A1;
			m_A[0].Normalize();
			m_A[1].Normalize();
			m_fA[0] = ABS(fA0);
			m_fA[1] = ABS(fA1);
			MWDVector3 N;
			N.Cross(A0, A1);
			MWDPlane3::Set(N, Center);
		};
		FORCEINLINE void Set(const MWDVector3& Center, const MWDVector3 A[2], const MWDREAL fA[2]) {
			Set(Center, A[0], A[1], fA[0], fA[1]);
		};
		FORCEINLINE void GetA(MWDVector3 A[2])const {
			A[0] = m_A[0];
			A[1] = m_A[1];
		};
		FORCEINLINE void GetfA(MWDREAL fA[2])const {
			fA[0] = m_fA[0];
			fA[1] = m_fA[1];
		};
		FORCEINLINE const MWDVector3& GetCenter()const {
			return m_Center;
		};
		FORCEINLINE void GetPoint(MWDVector3 Point[4])const {
			MWDVector3 temp0 = m_Center + m_A[0] * m_fA[0];
			MWDVector3 temp1 = m_Center + m_A[0] * (-m_fA[0]);
			Point[0] = temp0 + m_A[1] * m_fA[1];
			Point[1] = temp1 + m_A[1] * m_fA[1];
			Point[2] = temp1 + m_A[1] * (-m_fA[1]);
			Point[3] = temp0 + m_A[1] * (-m_fA[1]);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fRectangleParameter[2])const {
			return (m_Center + m_A[0] * fRectangleParameter[0] + m_A[1] * fRectangleParameter[1]);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fRectangleParameter0, MWDREAL fRectangleParameter1)const {
			return (m_Center + m_A[0] * fRectangleParameter0 + m_A[1] * fRectangleParameter1);
		};
		
	};

}