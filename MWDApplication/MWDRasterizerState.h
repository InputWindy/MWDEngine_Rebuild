#pragma once
#include "../MWDBind.h"
namespace MWDEngine{

	//暂时没使用光栅化状态
	class  MWDRasterizerDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDRasterizerDesc)
		DECLARE_RTTI(MWDRasterizerDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRasterizerDesc)
	public:

		//剔除模式
		enum
		{
			CT_NONE,
			CT_CW,
			CT_CCW,
			CT_MAX

		};
		MWDRasterizerDesc()
		{
			m_bWireEnable = false;
			m_uiCullType = CT_CCW;
			m_bClipPlaneEnable = false;
			m_bScissorTestEnable = false;
			m_fDepthBias = 0.0f;

		}
		~MWDRasterizerDesc()
		{

		}

		bool m_bWireEnable;
		unsigned char m_uiCullType;
		bool m_bClipPlaneEnable;
		bool m_bScissorTestEnable;
		MWDREAL	m_fDepthBias;

		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDRasterizerDesc) - sizeof(MWDObject);
			return (void*)&m_bWireEnable;
		}
	};
	MWDTYPE_MARCO(MWDRasterizerDesc);



	class  MWDRasterizerState : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDRasterizerState)
		DECLARE_RTTI(MWDRasterizerState, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRasterizerState)
	public:
		MWDRasterizerState() {
		};
		virtual ~MWDRasterizerState() {
		};
	protected:
		MWDRasterizerDesc m_RasterizerDesc;
	public:
		friend class MWDResourceManager;

		FORCEINLINE MWDRasterizerDesc& GetRasterizerDesc()
		{
			return m_RasterizerDesc;
		}

		static MWDRasterizerState* GetDefaultRasterizerState() {
			if (!m_DefaultRasterizerState) {
				m_DefaultRasterizerState = new MWDRasterizerState();
			}
			return m_DefaultRasterizerState;
		}
		static MWDRasterizerState* GetScissorEnableRasterizerState() {
			if (!m_ScissorEnableRasterizerState) {
				m_ScissorEnableRasterizerState = new MWDRasterizerState();
			}
			return m_ScissorEnableRasterizerState;
		}
	protected:
		static inline MWDRasterizerState* m_DefaultRasterizerState = NULL;
		static inline MWDRasterizerState* m_ScissorEnableRasterizerState = NULL;

	};
	DECLARE_Ptr(MWDRasterizerState);
	MWDTYPE_MARCO(MWDRasterizerState);
}

