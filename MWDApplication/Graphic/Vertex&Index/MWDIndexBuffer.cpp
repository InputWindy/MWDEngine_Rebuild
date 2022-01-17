#include "pch.h"
#include "MWDIndexBuffer.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDIndexBuffer)
    ADD_PRIORITY(MWDBind)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDIndexBuffer)

    BEGIN_ADD_PROPERTY(MWDIndexBuffer)
    END_ADD_PROPERTY


    bool MWDIndexBuffer::OnLoadResource(MWDOpenglResource*& pID) {
        if (!m_pUser)
            return 0;
        /*if (!m_pUser->OnLoadIBuffer(this, pID))
            return 0;*/
        return 1;
    };
    void* MWDIndexBuffer::Lock()
    {
        if (m_pLockData || m_bIsStatic)
        {
            return NULL;
        }
        if (m_pUser)
        {
            //m_pLockData = m_pUser->Lock(this);
        }

        return m_pLockData;
    }
    void MWDIndexBuffer::UnLock()
    {
        if (!m_pLockData || m_bIsStatic)
        {
            return;
        }
        if (m_pUser)
        {
            //m_pUser->UnLock(this);
        }
        m_pLockData = NULL;
    };
    bool MWDIndexBuffer::OnReleaseResource(MWDOpenglResource* pID)
    {
        if (!m_pUser)
            return 0;

        /*if (!m_pUser->OnReleaseIBuffer(pID))
            return 0;*/
        return 1;
    }

}