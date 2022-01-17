#include "pch.h"
#include "MWDVertexBuffer.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDVertexBuffer)
	ADD_PRIORITY(MWDBind)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDVertexBuffer)

    BEGIN_ADD_PROPERTY(MWDVertexBuffer)
    END_ADD_PROPERTY

	bool MWDVertexBuffer::LoadResource(MWDRenderer* pRender) {
		if (m_uiMemType == MT_RAM)
		{
			return 1;
		}
		if (m_uiSwapChainNum == m_resourceArray.GetNum())
			return 1;
		if (!m_pVertexFormat)
		{
			//MWDResourceManager::LoadVertexFormat(this);
		}
		if (!m_pVertexFormat->LoadResource(pRender))
			return 0;


		if (!MWDBind::LoadResource(pRender))
			return 0;

		return 1;
	};

	void* MWDVertexBuffer::Lock()
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

	void MWDVertexBuffer::UnLock()
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
	}

	bool MWDVertexBuffer::OnLoadResource(MWDOpenglResource*& pID)
	{
		if (!m_pUser)
			return 0;
		/*if (!m_pUser->OnLoadVBufferData(this, pID))
			return 0;*/
		return 1;
	}

	bool MWDVertexBuffer::OnReleaseResource(MWDOpenglResource* pID)
	{
		if (!m_pUser)
			return 0;
		/*if (!m_pUser->OnReleaseVBufferData(pID))
			return 0;*/
		return 1;
	};

}
