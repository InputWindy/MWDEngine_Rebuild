#include "MWDDepthStencilState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDDepthStencilDesc)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDDepthStencilDesc)

    BEGIN_ADD_PROPERTY(MWDDepthStencilDesc)
    REGISTER_PROPERTY(m_bDepthEnable, DepthEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_bDepthWritable, DepthWritable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiDepthCompareMethod, DepthCompareMethod, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_bStencilEnable, StencilEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiStencilCompareMethod, StencilCompareMethod, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiReference, Reference, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiMask, Mask, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiWriteMask, WriteMask, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiSPassZPassOP, SPassZPassOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiSPassZFailOP, SPassZFailOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiSFailZPassOP, SFailZPassOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCCW_SPassZPassOP, CCW_SPassZPassOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCCW_SPassZFailOP, CCW_SPassZFailOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCCW_SFailZPassOP, CCW_SFailZPassOP, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_bTwoSideStencilMode, TwoSideStencilMode, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCCW_StencilCompareMethod, CCW_StencilCompareMethod, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY



    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDDepthStencilState)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDDepthStencilState)

    BEGIN_ADD_PROPERTY(MWDDepthStencilState)
    END_ADD_PROPERTY

}

