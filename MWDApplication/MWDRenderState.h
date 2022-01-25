#pragma once
#include "MWDBlendState.h"
#include "MWDDepthStencilState.h"
#include "MWDRasterizerState.h"

namespace MWDEngine {
	//MWDRenderState维护除了SamplerState以外的所有State
	class  MWDRenderState : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDRenderState)
		DECLARE_RTTI(MWDRenderState, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRenderState)
	public:

		//继承标志位
		enum //Inherit Flag
		{
			IF_WIRE_ENABLE = 0X0000001,
			IF_CULL_TYPE = 0X0000002,

			IF_ALL = 0XFFFFFFFF
		};
		MWDRenderState()
		{
			m_pBlendState = MWDBlendState::GetDefaultBlendState();
			m_pDepthStencilState = MWDDepthStencilState::GetDefaultDepthStencilState();
			m_pRasterizerState = MWDRasterizerState::GetDefaultRasterizerState();
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		~MWDRenderState()
		{
			m_pBlendState = NULL;
			m_pDepthStencilState = NULL;
			m_pRasterizerState = NULL;
		}

		const MWDRenderState& operator =(const MWDRenderState& RenderState)
		{
			GetAll(&RenderState);
			return *this;
		}
		//浅拷贝
		void GetAll(const MWDRenderState* pRenderState)
		{
			MWDMAC_ASSERT(pRenderState);
			if (pRenderState)
			{
				SetRasterizerState(pRenderState->m_pRasterizerState);
				SetDepthStencilState(pRenderState->m_pDepthStencilState);
				SetBlendState(pRenderState->m_pBlendState);
				m_RectArray = pRenderState->m_RectArray;
				m_Plane = pRenderState->m_Plane;
			}
			else
			{
				ClearState();
			}
		}

		void Inherit(const MWDRenderState* pRenderState, unsigned int uiInheritFlag);
		void SwapCull();

		void AddRect(const MWDRect2& Rect)
		{
			m_RectArray.AddElement(Rect);
		}
		void ClearRect()
		{
			m_RectArray.Clear();
		}

		void AddPlane(const MWDPlane3& Plane)
		{
			m_Plane.AddElement(Plane);
		}
		void ClearPlane()
		{
			m_Plane.Clear();
		}

		void ClearState()
		{
			m_pDepthStencilState = NULL;
			m_pBlendState = NULL;
			m_pRasterizerState = NULL;
			m_RectArray.Clear();
			m_Plane.Clear();
		}
		void SetBlendState(MWDBlendState* pBlendState)
		{
			if (!pBlendState)
			{
				pBlendState = MWDBlendState::GetDefaultBlendState();
			}
			if (m_pBlendState != pBlendState)
			{
				m_pBlendState = pBlendState;
			}
		}
		void SetDepthStencilState(MWDDepthStencilState* pDepthStencilState)
		{
			if (!pDepthStencilState)
			{
				pDepthStencilState = MWDDepthStencilState::GetDefaultDepthStencilState();
			}
			if (m_pDepthStencilState != pDepthStencilState)
			{
				m_pDepthStencilState = pDepthStencilState;
			}
		}
		void SetRasterizerState(MWDRasterizerState* pRasterizerState)
		{
			if (!pRasterizerState)
			{
				pRasterizerState = MWDRasterizerState::GetDefaultRasterizerState();
			}
			if (m_pRasterizerState != pRasterizerState)
			{
				m_pRasterizerState = pRasterizerState;
			}
		}
		MWDDepthStencilState* GetDepthStencilState()const
		{
			return m_pDepthStencilState;
		}
		MWDBlendState* GetBlendState()const
		{
			return m_pBlendState;
		}
		MWDRasterizerState* GetRasterizerState()const
		{
			return m_pRasterizerState;
		}
	protected:
		MWDDepthStencilState*		m_pDepthStencilState;
		MWDBlendState*				m_pBlendState;
		MWDRasterizerState*			m_pRasterizerState;

		//2D裁剪平面数组
		MWDArray<MWDRect2> m_RectArray;
		//3D裁剪平面数组
		MWDArray<MWDPlane3> m_Plane;
	};
	MWDTYPE_MARCO(MWDRenderState);
	DECLARE_Ptr(MWDRenderState);
}

