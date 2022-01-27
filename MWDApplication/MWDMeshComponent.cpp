#include "MWDMeshComponent.h"
#include "MWDEntity.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDMeshComponent)
	ADD_PRIORITY(MWDComponent)
	INITIAL_WITH_INIT_TERMINAL_END(MWDMeshComponent)

	BEGIN_ADD_PROPERTY(MWDMeshComponent)
	END_ADD_PROPERTY

	void MWDMeshComponent::SetEntity(MWDEntity* entity) {
		m_pEntity = entity;
	}
}