#include "pch.h"
#include "MWDRasterizerState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDRasterizerDesc)
        ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDRasterizerDesc)

    BEGIN_ADD_PROPERTY(MWDRasterizerDesc)
    REGISTER_PROPERTY(m_bWireEnable, WireEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCullType, CullType, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_bClipPlaneEnable, ClipPlaneEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_bScissorTestEnable, ScissorTestEnable, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_fDepthBias, DepthBias, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY




    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDRasterizerState)
    ADD_PRIORITY(MWDBind)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDRasterizerState)

    BEGIN_ADD_PROPERTY(MWDRasterizerState)
    END_ADD_PROPERTY


    //未实现api
    bool MWDRasterizerState::OnLoadResource(MWDOpenglResource*& pID)
    {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadBlendState(this, pID))
            return 0;*/
        return 1;
    }
    //未实现api
    bool MWDRasterizerState::OnReleaseResource(MWDOpenglResource* pID)
    {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnReleaseBlendState(pID))
            return 0;*/
        return 1;
    }
}