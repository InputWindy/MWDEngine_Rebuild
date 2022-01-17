#pragma once
#include "MWDGraphic.h"
#include "MWDBind.h"
#include "MWDTexture.h"
namespace MWDEngine {
	class MWDRenderer;
	class  MWDRenderTarget : public MWDBind
	{
		DECLARE_RTTI_NoCreateFun(MWDRenderTarget, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRenderTarget)
	public:
		friend class MWD2DTexture;
		friend class MWDCubeTexture;
		friend class MWDResourceManager;

		virtual ~MWDRenderTarget() {
			ReleaseResource();
			m_pCreateBy = NULL;
		};
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{

			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetLevel()const
		{
			return m_uiLevel;
		}
		FORCEINLINE unsigned int GetParam()const
		{
			return m_uiParam;
		}
		FORCEINLINE MWDTexture* GetCreateBy()const
		{
			return m_pCreateBy;
		}
		bool m_bUsed;
	protected:
		MWDRenderTarget(unsigned int uiWidth, unsigned int uiHeight, unsigned int uiFormatType,
			unsigned int uiMulSample) {
			m_uiWidth = uiWidth;
			m_uiHeight = uiHeight;
			m_uiFormatType = uiFormatType;
			m_uiMulSample = uiMulSample;
			m_pCreateBy = NULL;
			m_uiLevel = 0;
			m_uiParam = 0;
			m_bUsed = false;
		};
		/*MWDRenderTarget(MWDTexture* pCreateBy, unsigned int uiMulSample = MWDRenderer::MS_NONE
			, unsigned int uiLevel = 0, unsigned int Param = 0);*/
		MWDRenderTarget() {
			m_uiWidth = 512;
			m_uiHeight = 512;
			//m_uiFormatType = MWDRenderer::SFT_A8R8G8B8;
			//m_uiMulSample = MWDRenderer::MS_NONE;
			m_pCreateBy = NULL;
			m_bUsed = false;
		};
		//表示渲染目标是否由纹理创建，如果是，就指向这个纹理
		MWDTexturePtr m_pCreateBy;
		virtual bool OnLoadResource(MWDResourceIdentifier*& pID);
		virtual bool OnReleaseResource(MWDResourceIdentifier* pID);

		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiFormatType;
		unsigned int m_uiMulSample;
		unsigned int m_uiLevel;
		unsigned int m_uiParam;
		
	};
	DECLARE_Ptr(MWDRenderTarget);
	MWDTYPE_MARCO(MWDRenderTarget);
}

