#include "MWDBlendState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDBlendDesc)
        ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDBlendDesc)

    BEGIN_ADD_PROPERTY(MWDBlendDesc)
    REGISTER_PROPERTY(bBlendEnable, BlendEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend, SrcBlend, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend, DestBlend, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp, BlendOp, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable, AlphaBlendEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha, SrcBlendAlpha, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha, DestBlendAlpha, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha, BlendOpAlpha, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask, WriteMask, MWDProperty::F_SAVE_LOAD_CLONE)


    REGISTER_PROPERTY(bAlphaToCoverageEnable, AlphaToCoverageEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bIndependentBlendEnable, IndependentBlendEnable, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(fBlendColor[0], BlendColor0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[1], BlendColor1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[2], BlendColor2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[3], BlendColor3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSampleMask, SampleMask, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY




    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDBlendState)
    ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDBlendState)

    BEGIN_ADD_PROPERTY(MWDBlendState)
    END_ADD_PROPERTY

}