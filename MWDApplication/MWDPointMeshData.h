#pragma once
#include "MWDMeshData.h"
namespace MWDEngine {
	//只维护index生成算法(一个顶点就是一个Mesh)
	class MWDPointMeshData : public MWDMeshData
	{
		DECLARE_CLASS_FUNCTION(MWDPointMeshData)
		DECLARE_RTTI(MWDPointMeshData, MWDMeshData)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDPointMeshData)
	public:
		MWDPointMeshData() {};
		MWDPointMeshData(const MWDVector3& Point, MWDREAL fSize) {
			MWDDataBuffer* pVertex = new MWDDataBuffer();

			pVertex->SetData(&Point, 1, MWDDataBuffer::DataType_FLOAT32_3);

			MWDDataBuffer* PSize = new MWDDataBuffer;
			PSize->SetData(&fSize, 1, MWDDataBuffer::DataType_FLOAT32_1);

			m_pVertexBuffer = new MWDVertexBuffer(true);
			m_pVertexBuffer->SetData(pVertex, MWDVertexFormat::VF_POSITION);
			m_pVertexBuffer->SetData(PSize, MWDVertexFormat::VF_PSIZE);

			bool bResult = CreateIndex();
			MWDMAC_ASSERT(bResult);
		};
		virtual ~MWDPointMeshData() {};
		bool CreateIndex() {
			if (m_pVertexBuffer == NULL)
				return 0;
			unsigned int Num = m_pVertexBuffer->GetVertexNum();
			if (Num < 1)
				return 0;
			MWDDataBuffer* pIndex = NULL;
			pIndex = new MWDDataBuffer();
			if (!pIndex)
				return 0;
			if (!pIndex->CreateEmptyBuffer(Num, MWDDataBuffer::DataType_USHORT))
				return 0;

			MWDUSHORT_INDEX* pIndexBuffer = (MWDUSHORT_INDEX*)pIndex->GetData();
			if (!pIndexBuffer)
				return 0;
			for (unsigned int i = 0; i < Num; i++)
			{
				pIndexBuffer[i] = i;
			}

			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer = new MWDIndexBuffer();
				if (!m_pIndexBuffer)
				{
					return 0;
				}
			}

			m_pIndexBuffer->SetData(pIndex);
			return 1;
		};
		virtual unsigned int GetTotleNum()const {
			if (!m_pIndexBuffer && !m_pVertexBuffer)
			{
				return 0;
			}
			else if (!m_pIndexBuffer)
			{
				return m_pVertexBuffer->GetVertexNum();
			}
			else
			{
				return m_pIndexBuffer->GetNum();
			}
		};
		virtual unsigned int GetMeshDataType() { return MDT_POINT; }

	};
	DECLARE_Ptr(MWDPointSet);
	MWDTYPE_MARCO(MWDPointSet);
}

