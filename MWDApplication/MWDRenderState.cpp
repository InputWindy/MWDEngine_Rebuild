#include "MWDRenderState.h"
namespace MWDEngine {
    INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDRenderState)
		ADD_PRIORITY(MWDObject)
    ADD_INITIAL_FUNCTION(InitialClassFactory)
    INITIAL_WITH_INIT_TERMINAL_END(MWDRenderState)

    BEGIN_ADD_PROPERTY(MWDRenderState)
    END_ADD_PROPERTY

	void MWDRenderState::Inherit(const MWDRenderState* pRenderState, unsigned int uiInheritFlag = 0) {
		if (!uiInheritFlag)
		{
			return;
		}
		MWDMAC_ASSERT(pRenderState);
		bool bReCreateDepthStencil = false;
		bool bReCreateRasterizer = false;
		bool bReCreateBlend = false;
		if (uiInheritFlag & IF_WIRE_ENABLE)
		{
			if (m_pRasterizerState->GetRasterizerDesc().m_bWireEnable !=
				pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable)
			{
				bReCreateRasterizer = true;
				m_pRasterizerState->GetRasterizerDesc().m_bWireEnable = pRenderState->m_pRasterizerState->GetRasterizerDesc().m_bWireEnable;
			}
		}
		if (bReCreateRasterizer)
		{
			m_pRasterizerState = new MWDRasterizerState();//MWDResourceManager::CreateRasterizerState(m_RasterizerDesc);
		}
		if (bReCreateDepthStencil)
		{
			m_pDepthStencilState = new MWDDepthStencilState(); //MWDResourceManager::CreateDepthStencilState(m_DepthStencilDesc);
		}
		if (bReCreateBlend)
		{
			m_pBlendState = new MWDBlendState();//MWDResourceManager::CreateBlendState(m_BlendDesc);
		}
		return;
	};
	void MWDRenderState::SwapCull() {
		unsigned int uiChangeType[3] = { MWDRasterizerDesc::CT_NONE,MWDRasterizerDesc::CT_CCW,MWDRasterizerDesc::CT_CW };
		m_pRasterizerState->GetRasterizerDesc().m_uiCullType = uiChangeType[m_pRasterizerState->GetRasterizerDesc().m_uiCullType];
		m_pRasterizerState = new MWDRasterizerState();// MWDResourceManager::CreateRasterizerState(m_RasterizerDesc);
	};
}