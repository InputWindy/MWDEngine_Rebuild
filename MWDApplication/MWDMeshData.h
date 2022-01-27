#pragma once
#include "MWDIndexBuffer.h"
#include "MWDVertexBuffer.h"
namespace MWDEngine {
	//MeshDataά��IndexBuffer��VertexBuffer
	//IndexBuffer��VertexBuffer�����ݶ����Դ�Obj�ļ��������
	//IndexBuffer����Ҫ�ȶ���������
	//��ȾMesh��ʱ��Ҫ��LoadDataToVBO(),LoadDataToIBO();
	class  MWDMeshData : public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDMeshData, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMeshData)
	public:
		enum { DRAW_MATH_ELEMENT_LENGTH = 10000 };
		enum //MeshData Type
		{
			MDT_POINT,
			MDT_LINE,
			MDT_TRIANGLE,
			MDT_MAX
		};
		virtual ~MWDMeshData() {
			MWDMAC_DELETE(m_pVertexBuffer)
			MWDMAC_DELETE(m_pIndexBuffer)
		};
		MWDMeshData(MWDIndexBuffer* pIndexBuffer, MWDVertexBuffer* pVertexBuffer) {
			SetIndexBuffer(pIndexBuffer);
			SetVertexBuffer(pVertexBuffer);
		};
		MWDMeshData(unsigned int index_usage = GL_STATIC_DRAW,unsigned int vertex_target = GL_ARRAY_BUFFER, unsigned int vertex_usage = GL_STATIC_DRAW) {
			MWDIndexBuffer* idx_buf = new MWDIndexBuffer(index_usage);
			MWDVertexBuffer* vert_buf = new MWDVertexBuffer(vertex_target, vertex_usage);
			SetIndexBuffer(idx_buf);
			SetVertexBuffer(vert_buf);
		};
	protected:
		MWDVertexBuffer* m_pVertexBuffer;
		MWDIndexBuffer* m_pIndexBuffer;

		unsigned int m_meshType;
	public:
		bool SetIndexBuffer(MWDIndexBuffer* pIndexBuffer) {
			if (!pIndexBuffer)
				return 0;
			m_pIndexBuffer =pIndexBuffer ;
				return 1;

		};
		MWDIndexBuffer* GetIndexBuffer()const {
			return m_pIndexBuffer;
		};

		bool SetVertexBuffer(MWDVertexBuffer* pVertexBuffer) {
			if (!pVertexBuffer)
				return 0;
			
			m_pVertexBuffer =pVertexBuffer ;
				return 1;

		};
		MWDVertexBuffer* GetVertexBuffer()const {
			return m_pVertexBuffer;
		};

		virtual unsigned int GetTotleNum()const {
			return 0;
		}
		//��ȡMeshƬԪ����                        
		virtual unsigned int GetPrimitivesNum()const {
			return 0;
		};                
		
	};
	DECLARE_Ptr(MWDMeshData);
	MWDTYPE_MARCO(MWDMeshData);
}

