#include "pch.h"
#include "MWDDepthStencil.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDDepthStencil)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDDepthStencil)

    BEGIN_ADD_PROPERTY(MWDDepthStencil)
    REGISTER_PROPERTY(m_uiWidth, Width, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiHeight, Height, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiMulSample, MulSample, MWDProperty::F_SAVE_LOAD_CLONE)
    REGISTER_PROPERTY(m_uiFormatType, FormatType, MWDProperty::F_SAVE_LOAD_CLONE)
    END_ADD_PROPERTY


    //未实现api
    bool MWDDepthStencil::OnLoadResource(MWDResourceIdentifier*& pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadBlendState(this, pID))
            return 0;*/
        return 1;
    }
    //未实现api
    bool MWDDepthStencil::OnReleaseResource(MWDResourceIdentifier* pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnReleaseBlendState(pID))
            return 0;*/
        return 1;
    }
}