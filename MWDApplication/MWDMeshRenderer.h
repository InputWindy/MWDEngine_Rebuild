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
		MWDMeshRenderer(MWDEntity* pParent);
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

