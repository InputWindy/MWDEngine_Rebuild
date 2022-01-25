#include "MWDMaterial.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDMaterial)
    ADD_PRIORITY(MWDObject)
    INITIAL_WITH_INIT_TERMINAL_END(MWDMaterial)

    BEGIN_ADD_PROPERTY(MWDMaterial)
    REGISTER_PROPERTY(m_CastShadow, CastShadow, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_ReciveShadow, ReciveShadow, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY
}