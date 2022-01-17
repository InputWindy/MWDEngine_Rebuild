#pragma once
#include "MWDGraphic.h"
#include "MWDBind.h"
namespace MWDEngine {
	class MWDStream;
	class  MWDDepthStencil : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDDepthStencil)
		DECLARE_RTTI(MWDDepthStencil, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDepthStencil)

	public:
		friend class MWDResourceManager;
		//MWDRendererŒ¥ µœ÷
		MWDDepthStencil(unsigned int uiWidth, unsigned int uiHeight,
			unsigned int uiMulSample, unsigned int uiFormatType) {
			m_uiWidth = uiWidth;
			m_uiHeight = uiHeight;
			/*MWDMAC_ASSERT(uiMulSample < MWDRenderer::MS_MAX)
			MWDMAC_ASSERT(uiFormatType < MWDRenderer::SFT_MAX)*/
			m_uiMulSample = uiMulSample;
			m_uiFormatType = uiFormatType;
			m_bIsStatic = 1;
			m_bUsed = false;
		};
		MWDDepthStencil() {
			m_bUsed = false;
		};
		virtual ~MWDDepthStencil() {
			ReleaseResource();
		};
		FORCEINLINE unsigned int GetWidth()const
		{
			return m_uiWidth;
		}
		FORCEINLINE unsigned int GetHeight()const
		{
			return m_uiHeight;
		}
		FORCEINLINE unsigned int GetMulSample()const
		{
			return m_uiMulSample;
		}
		FORCEINLINE unsigned int GetFormatType()const
		{
			return m_uiFormatType;
		}
		bool m_bUsed;
	protected:
		unsigned int m_uiWidth;
		unsigned int m_uiHeight;
		unsigned int m_uiMulSample;
		unsigned int m_uiFormatType;
	protected:
		virtual bool OnLoadResource(MWDResourceIdentifier*& pID);
		virtual bool OnReleaseResource(MWDResourceIdentifier* pID);

	};
	DECLARE_Ptr(MWDDepthStencil);
	MWDTYPE_MARCO(MWDDepthStencil);
}

