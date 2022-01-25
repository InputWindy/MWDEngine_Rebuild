#include "pch.h"
#include "MWDComponent.h"
#include "../MWDEntity.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDComponent)
	ADD_PRIORITY(MWDObject)
	INITIAL_WITH_INIT_TERMINAL_END(MWDComponent)

	BEGIN_ADD_PROPERTY(MWDComponent)
	END_ADD_PROPERTY

	void MWDComponent::SetEntity(MWDEntity* entity) {
		m_pEntity = entity;
	}
	MWDEntity* MWDComponent::GetEntity()const {
		return m_pEntity;
	};
}