#pragma once
#include "MWDMeshComponent.h"
#include "MWDMaterial.h"
namespace MWDEngine {
	class MWDEntity;
	//MeshRendererά����ǰEntity��MeshData��Material
	class MWDMeshRenderer:public MWDComponent
	{
		friend class MWDRenderer;
		DECLARE_RTTI_NoCreateFun(MWDMeshRenderer, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMeshRenderer)
	protected:
		MWDMeshData*		m_data;
		MWDMaterial*		m_Material;
	public:
		friend class MWDRenderer;
		bool m_bEnable;
		//�������
		static inline unsigned int m_uiType = MWDComponent::MESHRENDERER;
		void SetEntity(MWDEntity* entity);
		MWDMeshRenderer(MWDEntity* pParent = NULL);
		~MWDMeshRenderer() {
			MWDMAC_DELETE(m_data)
			MWDMAC_DELETE(m_Material)
		}
		virtual unsigned int GetComponentType() { return MESHRENDERER; };
		bool SetMaterial(MWDMaterial* Material) {
			MWDMAC_ASSERT(Material)
			if ( !Material) {
				return false;
			}
			m_Material = Material;
			return true;
		}
		//ִ��Material�������Pass
		bool Draw(MWDRenderer* pRenderer);
	};
}


