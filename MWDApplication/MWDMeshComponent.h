#pragma once
#include "MWDComponent.h"
#include "MWDMeshData.h"
namespace MWDEngine {

	//mesh组件只维护一个MWDMeshData
	class MWDMeshComponent:public MWDComponent
	{
		DECLARE_RTTI_NoCreateFun(MWDMeshComponent, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMeshComponent)
	protected:
		MWDMeshData* m_meshData;
	public:
		//组件类型
		static inline unsigned int m_uiType = MESH;

		virtual unsigned int GetComponentType() { return MESH; };
		virtual void ComponentInfoLog()const {

		}

		MWDMeshComponent(MWDIndexBuffer* pIndexBuffer, MWDVertexBuffer* pVertexBuffer) {
			MWDMAC_ASSERT(pIndexBuffer)
			MWDMAC_ASSERT(pVertexBuffer)
			m_meshData = new MWDMeshData(pIndexBuffer, pVertexBuffer);
		}
		MWDMeshComponent(unsigned int index_usage = GL_STATIC_DRAW, unsigned int vertex_target = GL_ARRAY_BUFFER, unsigned int vertex_usage = GL_STATIC_DRAW) {
			m_meshData = new MWDMeshData(index_usage, vertex_target, vertex_usage);
		}
		~MWDMeshComponent() {
			MWDMAC_DELETE(m_meshData)
		}

		MWDMeshData* GetMeshData()const {
			return m_meshData;
		}
		bool SetMeshData(MWDMeshData* meshdata) {
			if (!meshdata) {
				return false;
			}
			m_meshData = meshdata;
			return true;
		}
		bool SetMeshData(MWDIndexBuffer* pIndexBuffer, MWDVertexBuffer* pVertexBuffer) {
			if (!pIndexBuffer||!pVertexBuffer){
				return false;
			}
			m_meshData->SetIndexBuffer(pIndexBuffer);
			m_meshData->SetVertexBuffer(pVertexBuffer);
			return true;
		}

		bool SetIndexMeshData(MWDDataBuffer* pIndexData) {
			if (!pIndexData) {
				return false;
			}
			MWDIndexBuffer*index_buf =  m_meshData->GetIndexBuffer();
			index_buf->SetData(pIndexData);
			return true;
		}
		/// <summary>
		/// VF_POSITION,		//Location=0
		/// VF_TEXCOORD,		//Location=1
		///	VF_NORMAL,			//Location=2
		///	VF_TANGENT,			//Location=3
		///	VF_BINORMAL,		//Location=4
		///	VF_PSIZE,			//Location=5
		///	VF_COLOR,			//Location=6
		///	VF_FOG,				//Location=7
		///	VF_DEPTH,			//Location=8
		///	VF_BLENDWEIGHT,		//Location=9
		///	VF_BLENDINDICES,	//Location=10
		///	VF_MAX
		/// </summary>
		/// <param name="pVertexData"></param>
		/// <param name="Attribute"></param>
		/// <returns></returns>
		bool SetVertexMeshData(MWDDataBuffer* pVertexData,unsigned int Attribute) {
			if (!pVertexData) {
				return false;
			}
			MWDVertexBuffer* vertex_buf = m_meshData->GetVertexBuffer();
			vertex_buf->SetData(pVertexData,Attribute);
			return true;
		}
	};
}


