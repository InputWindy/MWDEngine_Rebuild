#include "MWDCamera.h"
#include "MWDEntity.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDCamera)
    ADD_PRIORITY(MWDEntity)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDCamera)

    BEGIN_ADD_PROPERTY(MWDCamera)
    REGISTER_PROPERTY(m_Fov, InputWindy1, MWDProperty::F_REFLECT_NAME)
    REGISTER_PROPERTY(m_Aspect, InputWindy2, MWDProperty::F_REFLECT_NAME)
    REGISTER_PROPERTY(m_ZFar, InputWindy3, MWDProperty::F_REFLECT_NAME)
    REGISTER_PROPERTY(m_ZNear, InputWindy4, MWDProperty::F_REFLECT_NAME)
    END_ADD_PROPERTY

}