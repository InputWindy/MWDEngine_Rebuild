#include "pch.h"
#include "MWDEntity.h"
//#include "MWDScene.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDEntity)
	INITIAL_WITH_INIT_TERMINAL_END(MWDEntity)

	BEGIN_ADD_PROPERTY(MWDEntity)
	END_ADD_PROPERTY

	MWDEntity::~MWDEntity() {
		m_pChildren.Destroy();
		m_pComponent.Destroy();
		if (m_pParent) {
			m_pParent->DeleteChildByName(m_Name);
		}
		
		/*m_Scene->DeleteStaticEntityByName(m_Name);
		m_Scene->DeleteDynamicEntityByName(m_Name);
		m_Scene->DeleteCameraByName(m_Name);
		m_Scene->DeleteLightEntityByName(m_Name);*/
	};

	MWDScene* MWDEntity::GetScene()const {
		return m_Scene;
	}

	void MWDEntity::SetScene(MWDScene* scene) {
		m_Scene = scene;
	}

}