#include "pch.h"
#include "MWDBlendState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDBlendDesc)
        ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDBlendDesc)

    BEGIN_ADD_PROPERTY(MWDBlendDesc)
    REGISTER_PROPERTY(bBlendEnable[0], BlendEnable0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[0], SrcBlend0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[0], DestBlend0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[0], BlendOp0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[0], AlphaBlendEnable0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[0], SrcBlendAlpha0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[0], DestBlendAlpha0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[0], BlendOpAlpha0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[0], WriteMask0, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[1], BlendEnable1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[1], SrcBlend1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[1], DestBlend1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[1], BlendOp1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[1], AlphaBlendEnable1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[1], SrcBlendAlpha1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[1], DestBlendAlpha1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[1], BlendOpAlpha1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[1], WriteMask1, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[2], BlendEnable2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[2], SrcBlend2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[2], DestBlend2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[2], BlendOp2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[2], AlphaBlendEnable2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[2], SrcBlendAlpha2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[2], DestBlendAlpha2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[2], BlendOpAlpha2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[2], WriteMask2, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[3], BlendEnable3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[3], SrcBlend3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[3], DestBlend3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[3], BlendOp3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[3], AlphaBlendEnable3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[3], SrcBlendAlpha3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[3], DestBlendAlpha3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[3], BlendOpAlpha3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[3], WriteMask3, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[4], BlendEnable4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[4], SrcBlend4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[4], DestBlend4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[4], BlendOp4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[4], AlphaBlendEnable4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[4], SrcBlendAlpha4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[4], DestBlendAlpha4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[4], BlendOpAlpha4, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[4], WriteMask4, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[5], BlendEnable5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[5], SrcBlend5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[5], DestBlend5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[5], BlendOp5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[5], AlphaBlendEnable5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[5], SrcBlendAlpha5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[5], DestBlendAlpha5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[5], BlendOpAlpha5, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[5], WriteMask5, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[6], BlendEnable6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[6], SrcBlend6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[6], DestBlend6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[6], BlendOp6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[6], AlphaBlendEnable6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[6], SrcBlendAlpha6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[6], DestBlendAlpha6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[6], BlendOpAlpha6, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[6], WriteMask6, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bBlendEnable[7], BlendEnable7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlend[7], SrcBlend7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlend[7], DestBlend7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOp[7], BlendOp7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bAlphaBlendEnable[7], AlphaBlendEnable7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSrcBlendAlpha[7], SrcBlendAlpha7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucDestBlendAlpha[7], DestBlendAlpha7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucBlendOpAlpha[7], BlendOpAlpha7, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucWriteMask[7], WriteMask7, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(bAlphaToCoverageEnable, AlphaToCoverageEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(bIndependentBlendEnable, IndependentBlendEnable, MWDProperty::F_SAVE_LOAD_CLONE)

    REGISTER_PROPERTY(fBlendColor[0], BlendColor0, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[1], BlendColor1, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[2], BlendColor2, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(fBlendColor[3], BlendColor3, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(ucSampleMask, SampleMask, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY




    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDBlendState)
        ADD_PRIORITY(MWDBind)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDBlendState)

    BEGIN_ADD_PROPERTY(MWDBlendState)
    END_ADD_PROPERTY

    //未实现api
    bool MWDBlendState::OnLoadResource(MWDOpenglResource*& pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadBlendState(this, pID))
            return 0;*/
        return 1;
    }
    //未实现api
    bool MWDBlendState::OnReleaseResource(MWDOpenglResource* pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnReleaseBlendState(pID))
            return 0;*/
        return 1;
    }
}