#include "pch.h"
#include "MWDEntity.h"
#include "../MWDScene.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDEntity)
	INITIAL_WITH_INIT_TERMINAL_END(MWDEntity)

	BEGIN_ADD_PROPERTY(MWDEntity)
	END_ADD_PROPERTY

	MWDScene* MWDEntity::GetScene()const {
		return m_Scene;
	}

	void MWDEntity::SetScene(MWDScene* scene) {
		m_Scene = scene;
	}

}