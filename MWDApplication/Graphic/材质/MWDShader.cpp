#include "pch.h"
#include "MWDShader.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDShader)
	INITIAL_WITH_INIT_TERMINAL_END(MWDShader)

	BEGIN_ADD_PROPERTY(MWDShader)

	END_ADD_PROPERTY
		

	void MWDShader::SetShaderString(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile ) {
		MWDMAC_DELETEA(m_pCacheBuffer);
		m_uiCacheBufferSize = 0;
		ReleaseResource();

		MWDMAC_ASSERT(pBuffer);
		if (IsFromFile)
		{
			m_ResourceName = pBuffer;
		}
		else
		{
			m_Buffer = pBuffer;
		}

		m_pCacheBuffer = NULL;
		m_uiCacheBufferSize = 0;
		m_MainFunName = MainFunName;
	}
		
	void MWDShader::SetShaderString(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile)
	{
		MWDMAC_DELETEA(m_pCacheBuffer);
		m_uiCacheBufferSize = 0;
		ReleaseResource();

		if (IsFromFile)
		{
			m_ResourceName = Buffer;
		}
		else
		{
			m_Buffer = Buffer;
		}

		m_pCacheBuffer = NULL;
		m_uiCacheBufferSize = 0;
		m_MainFunName = MainFunName;
	}

	bool MWDShader::SetCacheBuffer(void* pBuffer, unsigned int uiSize)
	{
		if (!pBuffer || !uiSize)
		{
			return false;
		}
		MWDMAC_DELETEA(m_pCacheBuffer);
		m_uiCacheBufferSize = uiSize;
		m_pCacheBuffer = new UCHAR[uiSize];
		MWDMemcpy(m_pCacheBuffer, pBuffer, uiSize);
		return 1;
	}

	bool MWDShader::SetParam(const MWDName& Name, void* pData) {
		if (!pData || !m_bCreatePara)
		{
			return false;
		}
		for (unsigned int i = 0; i < m_pShaderConstant.GetNum(); i++)
		{
			if (Name == m_pShaderConstant[i]->GetShowName())
			{
				void* pConstanData = m_pShaderConstant[i]->GetData();
				if (!pConstanData)
				{
					return false;
				}
				MWDMemcpy(pConstanData, pData, m_pShaderConstant[i]->GetSize());
				break;
			}
		}
		return true;
	};

	bool MWDShader::SetParam(const MWDName& Name, MWDTexture2D* pTexture, unsigned int uiIndex )
	{
		if (!pTexture || !m_bCreatePara)
		{
			return false;
		}
		for (unsigned int i = 0; i < m_pShaderSampler.GetNum(); i++)
		{
			if (Name == m_pShaderSampler[i]->GetName()
				&& pTexture->m_pTex.get == m_pShaderSampler[i]->GetTexType())
			{
				m_pShaderSampler[i]->SetTexture(pTexture, uiIndex);
				break;
			}
		}
		return true;
	}




	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDVertexShader)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDVertexShader)

	BEGIN_ADD_PROPERTY(MWDVertexShader)

	END_ADD_PROPERTY


	bool MWDVertexShader::OnLoadResource(MWDOpenglResource*& pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnLoadVShaderProgram(this, pID))
			return 0;*/

		return 1;
	}

	bool MWDVertexShader::OnReleaseResource(MWDOpenglResource* pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnReleaseVShaderProgram(pID))
			return 0;*/


		return 1;
	}





	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDGeometryShader)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDGeometryShader)

	BEGIN_ADD_PROPERTY(MWDGeometryShader)

	END_ADD_PROPERTY


	bool MWDGeometryShader::OnLoadResource(MWDOpenglResource*& pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnLoadVShaderProgram(this, pID))
			return 0;*/

		return 1;
	}

	bool MWDGeometryShader::OnReleaseResource(MWDOpenglResource* pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnReleaseVShaderProgram(pID))
			return 0;*/


		return 1;
	}



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDFragmentShader)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDFragmentShader)

	BEGIN_ADD_PROPERTY(MWDFragmentShader)

	END_ADD_PROPERTY


	bool MWDFragmentShader::OnLoadResource(MWDOpenglResource*& pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnLoadVShaderProgram(this, pID))
			return 0;*/

		return 1;
	}

	bool MWDFragmentShader::OnReleaseResource(MWDOpenglResource* pID)
	{
		if (!m_pUser)
			return 0;

		/*if (!m_pUser->OnReleaseVShaderProgram(pID))
			return 0;*/


		return 1;
	}


}


