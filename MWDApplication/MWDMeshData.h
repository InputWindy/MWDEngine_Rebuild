#pragma once
#include "../../Vertex&Index/MWDVertexBuffer.h"
#include "../../Vertex&Index/MWDIndexBuffer.h"
namespace MWDEngine{
	//MeshData维护IndexBuffer和VertexBuffer
	//IndexBuffer和VertexBuffer的数据都可以从Obj文件解析获得
	//IndexBuffer长度要比顶点数量多
	//渲染Mesh的时候要先LoadDataToVBO(),LoadDataToIBO();
	class  MWDMeshData : public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDMeshData, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMeshData)
	public:
		enum //MeshData Type
		{
			MDT_POINT,
			MDT_LINE,
			MDT_TRIANGLE,
			MDT_MAX
		};
		virtual ~MWDMeshData() {
			m_pVertexBuffer = NULL;
			m_pIndexBuffer = NULL;
		};
		MWDMeshData() {
			m_pVertexBuffer = NULL;
			m_pIndexBuffer = NULL;
		};
		bool SetIndexBuffer(MWDIndexBuffer* pIndexBuffer) {
			if (!pIndexBuffer)
				return 0;

			if (pIndexBuffer->GetNum())
			{
				m_pIndexBuffer = pIndexBuffer;
				return 1;
			}
			else
			{
				return 0;
			}
		};
		FORCEINLINE MWDIndexBuffer* GetIndexBuffer()const {
			return m_pIndexBuffer;
		};

		bool SetVertexBuffer(MWDVertexBuffer* pVertexBuffer) {
			if (!pVertexBuffer)
				return 0;
			if (pVertexBuffer->GetVertexNum())
			{
				m_pVertexBuffer = pVertexBuffer;
				return 1;
			}
			else
			{
				return 0;
			}
		};
		FORCEINLINE MWDVertexBuffer* GetVertexBuffer()const {
			return m_pVertexBuffer;
		};

		virtual unsigned int GetTotleNum()const {
			return 0;
		};

		virtual unsigned int GetMeshDataType() {
			return MDT_MAX;
		};
		virtual unsigned int GetActiveNum()const { 
			return GetTotleNum();
		}
	protected:
		friend class MWDGeometry;
		enum
		{
			DRAW_MATH_ELEMENT_LENGTH = 10000
		};
		MWDVertexBufferPtr	m_pVertexBuffer;
		MWDIndexBufferPtr	m_pIndexBuffer;
	};
	DECLARE_Ptr(MWDMeshData);
	MWDTYPE_MARCO(MWDMeshData);
}

