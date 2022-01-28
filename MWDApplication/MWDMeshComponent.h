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
		void SetEntity(MWDEntity* entity);
		MWDMeshComponent(MWDEntity* Owner = NULL , unsigned int index_usage = GL_STATIC_DRAW, unsigned int vertex_target = GL_ARRAY_BUFFER, unsigned int vertex_usage = GL_STATIC_DRAW) {
			m_Name = MWDName(_T("MeshComponent"));
			m_meshData = new MWDMeshData(index_usage, vertex_target, vertex_usage);
			SetEntity(Owner);
		}
		~MWDMeshComponent() {
			MWDMAC_DELETE(m_meshData)
		}

		MWDMeshData* GetMeshData()const {
			return m_meshData;
		}

	};
}


