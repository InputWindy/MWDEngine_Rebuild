#pragma once
#include "MWDMath.h"
#include "MWDPlane3.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class MWDMATH_API MWDPolygon3 : public MWDPlane3, public MWDMemObject
	{
	public:
		unsigned int	m_PointNum;		
		unsigned int	m_IndexNum;		
		bool			m_Flag;			
		MWDVector3* m_pPoint;		
		MWDUSHORT_INDEX* m_pIndex;		
	public:
		MWDPolygon3() {
			m_PointNum = 0;		
			m_IndexNum = 0;		
			m_Flag = 0;			
			m_pPoint = NULL;		
			m_pIndex = NULL;
		};
		~MWDPolygon3() {
			MWDMAC_DELETEA(m_pPoint);
			MWDMAC_DELETEA(m_pIndex);
		};

		MWDPolygon3(const MWDVector3* pPoint, unsigned int PointNum) {
			Set(pPoint, PointNum);
		};
		
		bool CopyFrom(const MWDPolygon3& Polygon) {
			if (!Polygon.m_pPoint || !Polygon.m_PointNum || !Polygon.m_pIndex || !Polygon.m_IndexNum)
				return 0;
			m_PointNum = Polygon.m_PointNum;
			m_IndexNum = Polygon.m_IndexNum;
			m_Flag = Polygon.m_Flag;
			MWDMAC_DELETEA(m_pPoint);
			MWDMAC_DELETEA(m_pIndex);
			m_pPoint = new MWDVector3[m_PointNum];
			if (!m_pPoint)
				return 0;
			m_pIndex = new MWDUSHORT_INDEX[m_IndexNum];
			if (!m_pIndex)
				return 0;
			MWDMemcpy(m_pPoint, Polygon.m_pPoint, sizeof(MWDVector3) * m_PointNum);
			MWDMemcpy(m_pIndex, Polygon.m_pIndex, sizeof(MWDUSHORT_INDEX) * m_IndexNum);
			return 1;
		};
		
		void SwapFaces() {
			MWDUSHORT_INDEX* pIndis = new MWDUSHORT_INDEX[m_IndexNum];


			for (unsigned int i = 0; i < m_IndexNum; i++)
				pIndis[m_IndexNum - i - 1] = m_pIndex[i];

			m_Flag = !m_Flag;

			MWDPlane3::Set(m_N * (-1.0f), m_fD);

			MWDMAC_DELETEA(m_pIndex);
			m_pIndex = pIndis;
		};
		/*********************************** FORCEINLINE *************************************/
		FORCEINLINE void Set(const MWDVector3* pPoint, unsigned int PointNum) {
			if (!pPoint || !PointNum)
				return;

			m_PointNum = PointNum;
			m_IndexNum = 3 * (m_PointNum - 2);

			MWDMAC_DELETEA(m_pPoint);
			MWDMAC_DELETEA(m_pIndex);
			m_pPoint = new MWDVector3[m_PointNum];
			m_pIndex = new MWDUSHORT_INDEX[m_IndexNum];
			MWDMemcpy(m_pPoint, pPoint, sizeof(MWDVector3) * PointNum);

			m_Flag = 1;

			unsigned int I0, I1, I2;
			for (unsigned int nLoop = 0; nLoop < (m_PointNum - 2); nLoop++)
			{
				if (nLoop == 0) { I0 = 0; I1 = 1; I2 = 2; }
				else { I1 = I2; I2++; }

				m_pIndex[(nLoop * 3)] = I0;
				m_pIndex[(nLoop * 3) + 1] = I1;
				m_pIndex[(nLoop * 3) + 2] = I2;
			}


			MWDVector3 vcEdge0 = m_pPoint[1] - m_pPoint[0];
			MWDVector3 vcEdge1;
			for (unsigned int i = 2; i < PointNum; i++)
			{
				vcEdge1 = m_pPoint[i] - m_pPoint[0];

				vcEdge0.Normalize();
				vcEdge1.Normalize();

				MWDREAL dot = vcEdge0.Dot(vcEdge1);
				if (ABS(dot) < MWDREAL_1)
					break;
			} // for

			MWDVector3 N;
			N.Cross(vcEdge0, vcEdge1);
			MWDPlane3::Set(N, m_pPoint[0]);
		};
		FORCEINLINE unsigned int GetPointNum()const {
			return m_PointNum;
		};
		FORCEINLINE unsigned int GetIndexNum()const {
			return m_IndexNum;
		};
		FORCEINLINE const MWDVector3* const GetpPoint()const {
			return m_pPoint;
		};
		FORCEINLINE const MWDUSHORT_INDEX* const GetpIndex()const {
			return m_pIndex;
		};
		FORCEINLINE unsigned int GetFlag()const {
			return m_Flag;
		};
		
	};

}
