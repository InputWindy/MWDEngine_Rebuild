#include "pch.h"
#include "MWDRenderTarget.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDRenderTarget)
    INITIAL_WITH_INIT_TERMINAL_END(MWDRenderTarget)

    BEGIN_ADD_PROPERTY(MWDRenderTarget)
    END_ADD_PROPERTY


    //未实现api
    bool MWDRenderTarget::OnLoadResource(MWDResourceIdentifier*& pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadBlendState(this, pID))
            return 0;*/
        return 1;
    }
    //未实现api
    bool MWDRenderTarget::OnReleaseResource(MWDResourceIdentifier* pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnReleaseBlendState(pID))
            return 0;*/
        return 1;
    }
}