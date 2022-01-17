#pragma once
#include "MWDMath.h"
#include "MWDTriangle3.h"
#include "../MWDDataStruct/MWDArray.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class MWDMATH_API MWDAABB3
	{
	public:
		static inline const MWDVector3	m_A[3] = {};
		MWDVector3	m_Center;		
		//Èý¸öÖáÏò
		MWDREAL		m_fA[3];					
		MWDVector3	m_Max;					
		MWDVector3	m_Min;						
	public:

		MWDAABB3() {
			m_fA[0] = m_fA[1] = m_fA[2] = 0.0f;
		};
		~MWDAABB3() {

		};

		MWDAABB3(const MWDVector3& Max, const MWDVector3& Min) {
			Set(Max, Min);
		};
		MWDAABB3(const MWDVector3& Center, MWDREAL fA0, MWDREAL fA1, MWDREAL fA2) {
			Set(Center, fA0, fA1, fA2);
		};
		MWDAABB3(const MWDVector3& Center, MWDREAL fA[3]) {
			Set(Center, fA);
		};
		
		void CreateAABB(const MWDVector3* const pPointArray, unsigned int uiPointNum) {
			if (!pPointArray || !uiPointNum)
				return;
			m_Min = pPointArray[0];
			m_Max = pPointArray[0];
			for (unsigned int i = 1; i < uiPointNum; i++)
			{
				if (pPointArray[i].x < m_Min.x)
				{
					m_Min.x = pPointArray[i].x;
				}
				else if (pPointArray[i].x > m_Max.x)
				{
					m_Max.x = pPointArray[i].x;
				}

				if (pPointArray[i].y < m_Min.y)
				{
					m_Min.y = pPointArray[i].y;
				}
				else if (pPointArray[i].y > m_Max.y)
				{
					m_Max.y = pPointArray[i].y;
				}

				if (pPointArray[i].z < m_Min.z)
				{
					m_Min.z = pPointArray[i].z;
				}
				else if (pPointArray[i].z > m_Max.z)
				{
					m_Max.z = pPointArray[i].z;
				}
			}
			m_Center = (m_Min + m_Max) / 2.0f;

			MWDVector3 Temp = (m_Max - m_Min) / 2.0f;

			m_fA[0] = Temp.x;
			m_fA[1] = Temp.y;
			m_fA[2] = Temp.z;
		};
		/*********************************** FORCEINLINE *************************************/
		
		FORCEINLINE void Set(const MWDVector3& Max, const MWDVector3& Min) {
			m_Max = Max;
			m_Min = Min;
			m_Center = (Max + Min) / 2.0f;
			MWDVector3 Temp = (Max - Min) / 2.0f;
			for (int i = 0; i < 3; i++)
				m_fA[i] = Temp.m[i];
		};
		FORCEINLINE void Set(const MWDVector3& Center, MWDREAL fA0, MWDREAL fA1, MWDREAL fA2) {
			m_fA[0] = ABS(fA0);
			m_fA[1] = ABS(fA1);
			m_fA[2] = ABS(fA2);

			m_Center = Center;

			m_Max.Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1], m_Center.z + m_fA[2]);
			m_Min.Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1], m_Center.z - m_fA[2]);

		};
		FORCEINLINE void Set(const MWDVector3& Center, MWDREAL fA[3]) {
			m_fA[0] = ABS(fA[0]);
			m_fA[1] = ABS(fA[1]);
			m_fA[2] = ABS(fA[2]);

			m_Center = Center;

			m_Max.Set(m_Center.x + m_fA[0], m_Center.y + m_fA[1], m_Center.z + m_fA[2]);
			m_Min.Set(m_Center.x - m_fA[0], m_Center.y - m_fA[1], m_Center.z - m_fA[2]);

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
		FORCEINLINE const MWDVector3& GetMaxPoint()const {
			return m_Max;
		};
		FORCEINLINE const MWDVector3& GetMinPoint()const {
			return m_Min;
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fAABBParameter[3])const {
			return MWDVector3(m_Center.x + fAABBParameter[0], m_Center.y + fAABBParameter[1], m_Center.z + fAABBParameter[2]);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fAABBParameter0, MWDREAL fAABBParameter1, MWDREAL fAABBParameter2)const {
			return MWDVector3(m_Center.x + fAABBParameter0, m_Center.y + fAABBParameter1, m_Center.z + fAABBParameter2);
		};
	};

}