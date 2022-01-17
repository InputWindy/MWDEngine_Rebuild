#pragma once
#include "MWDMath.h"
#include "MWDTriangle3.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class MWDMATH_API MWDOBB3
	{
	public:
		MWDVector3	m_A[3];		
		MWDVector3	m_Center;	
		MWDREAL		m_fA[3];	
	public:


		MWDOBB3() {
			
		};
		~MWDOBB3() {

		};
		MWDOBB3(const MWDVector3 A[3], MWDREAL fA[3], const MWDVector3& Center) {
			Set(A, fA, Center);
		};
		MWDOBB3(const MWDVector3& A0, const MWDVector3& A1, const MWDVector3& A2,MWDREAL fA0, MWDREAL fA1, MWDREAL fA2,const MWDVector3& Center) {
			Set(A0, A1, A2, fA0, fA1, fA2, Center);
		};
		
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const MWDVector3 A[3], MWDREAL fA[3], const MWDVector3& Center) {
			m_A[0] = A[0];
			m_A[1] = A[1];
			m_A[2] = A[2];
			m_A[0].Normalize();
			m_A[1].Normalize();
			m_A[2].Normalize();
			m_fA[0] = ABS(fA[0]);
			m_fA[1] = ABS(fA[1]);
			m_fA[2] = ABS(fA[2]);
			m_Center = Center;
		};
		FORCEINLINE void Set(const MWDVector3& A0, const MWDVector3& A1, const MWDVector3& A2,MWDREAL fA0, MWDREAL fA1, MWDREAL fA2,const MWDVector3& Center) {
			m_A[0] = A0;
			m_A[1] = A1;
			m_A[2] = A2;

			m_A[0].Normalize();
			m_A[1].Normalize();
			m_A[2].Normalize();

			m_fA[0] = ABS(fA0);
			m_fA[1] = ABS(fA1);
			m_fA[2] = ABS(fA2);
			m_Center = Center;
		};

		FORCEINLINE void GetA(MWDVector3 A[3])const {
			for (int i = 0; i < 3; i++)
			{
				A[i] = m_A[i];
			}
		};
		FORCEINLINE void GetfA(MWDREAL fA[3])const {
			for (int i = 0; i < 3; i++)
			{
				fA[i] = m_fA[i];
			}
		};
		FORCEINLINE const MWDVector3& GetCenter()const {
			return m_Center;
		};
		
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fOBBParameter[3])const {
			return (m_Center + m_A[0] * fOBBParameter[0] + m_A[1] * fOBBParameter[1] + m_A[2] * fOBBParameter[2]);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fOBBParameter0, MWDREAL fOBBParameter1, MWDREAL fOBBParameter2)const {
			return (m_Center + m_A[0] * fOBBParameter0 + m_A[1] * fOBBParameter1 + m_A[2] * fOBBParameter2);
		};

	};

}
