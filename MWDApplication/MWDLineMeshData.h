#pragma once
#include "MWDMeshData.h"
namespace MWDEngine {

	//只负责实现相关方法，不维护新数据
	class MWDLineMeshData :public MWDMeshData
	{
		DECLARE_CLASS_FUNCTION(MWDLineMeshData)
		DECLARE_RTTI(MWDLineMeshData, MWDMeshData)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDLineMeshData)
	public:
		MWDLineMeshData() {};
		virtual ~MWDLineMeshData() {};
		enum	//Line Type
		{
			LT_OPEN,
			LT_CLOSE,
			LT_SEGMENT,
			LT_MAX
		};
		bool CreateIndex(unsigned int uiLineType) {
			if (m_pVertexBuffer == NULL)
				return 0;
			unsigned int Num = m_pVertexBuffer->GetVertexNum();
			if (Num < 2)
				return 0;
			if (uiLineType == LT_OPEN)
			{
				Num = (Num - 1) * 2;

			}
			else if (uiLineType == LT_CLOSE)
			{
				Num = Num * 2;
			}
			else if (uiLineType == LT_SEGMENT)
			{
				if (Num % 2)
				{
					return 0;
				}

			}
			else
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

			if (uiLineType == LT_OPEN)
			{
				for (unsigned int i = 0; i < Num / 2; i++)
				{
					pIndexBuffer[2 * i] = i;
					pIndexBuffer[2 * i + 1] = i + 1;
				}
			}
			else if (uiLineType == LT_CLOSE)
			{
				unsigned int i;
				for (i = 0; i < Num / 2; i++)
				{
					pIndexBuffer[2 * i] = i;
					pIndexBuffer[2 * i + 1] = i + 1;
				}
				//i已经递增，最后一个索引应该是0，所以修改 2 * ( i - 1) + 1 =  2 * i - 1
				pIndexBuffer[2 * i - 1] = 0;
			}
			else if (uiLineType == LT_SEGMENT)
			{
				for (unsigned int i = 0; i < Num; i++)
				{
					pIndexBuffer[i] = i;

				}
			}
			if (!m_pIndexBuffer)
			{
				m_pIndexBuffer =new MWDIndexBuffer();
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
				return m_pVertexBuffer->GetVertexNum() / 2;
			}
			else
				return m_pIndexBuffer->GetNum() / 2;
		};
		virtual unsigned int GetMeshDataType() { return MDT_LINE; }

	};
	DECLARE_Ptr(MWDLineMeshData);
	MWDTYPE_MARCO(MWDLineMeshData);
}

