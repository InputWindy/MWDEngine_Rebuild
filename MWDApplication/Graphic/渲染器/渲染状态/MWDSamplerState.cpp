#include "pch.h"
#include "MWDSamplerState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDSamplerDesc)
        ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDSamplerDesc)

    BEGIN_ADD_PROPERTY(MWDSamplerDesc)
    REGISTER_PROPERTY(m_uiMag, Mag, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiMin, Min, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiMip, Mip, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiMipLevel, MipLevel, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiAniLevel, AniLevel, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCoordU, CoordU, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCoordV, CoordV, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiCoordW, CoordW, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_MipLODBias, MipLODBias, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_BorderColor, BorderColor, MWDProperty::F_SAVE_LOAD_CLONE)
    
    END_ADD_PROPERTY


    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDSamplerState)
        ADD_PRIORITY(MWDBind)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDSamplerState)

    BEGIN_ADD_PROPERTY(MWDSamplerState)
    END_ADD_PROPERTY



    //未实现api
    bool MWDSamplerState::OnLoadResource(MWDOpenglResource*& pID)
    {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadBlendState(this, pID))
            return 0;*/
        return 1;
    }
    //未实现api
    bool MWDSamplerState::OnReleaseResource(MWDOpenglResource* pID)
    {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnReleaseBlendState(pID))
            return 0;*/
        return 1;
    }
}