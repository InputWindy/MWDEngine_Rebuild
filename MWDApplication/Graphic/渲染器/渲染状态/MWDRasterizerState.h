#pragma once
#include "../MWDBind.h"
namespace MWDEngine{
	class MWDStream;
	class  MWDRasterizerDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDRasterizerDesc)
		DECLARE_RTTI(MWDRasterizerDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRasterizerDesc)
	public:

		//ÌÞ³ýÄ£Ê½
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



	class  MWDRasterizerState : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDRasterizerState)
		DECLARE_RTTI(MWDRasterizerState, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDRasterizerState)
	public:
		MWDRasterizerState() {
			m_bIsStatic = 1;
		};
		virtual ~MWDRasterizerState() {
			ReleaseResource();
		};
	protected:
		MWDRasterizerDesc m_RasterizerDesc;
	public:
		static bool InitialDefaultState() {
			return true;
		};
		static bool TerminalDefaultState() {
			return true;
		};
	public:
		friend class MWDResourceManager;

		FORCEINLINE const MWDRasterizerDesc& GetRasterizerDesc()const
		{
			return m_RasterizerDesc;
		}

		static MWDPointer<MWDRasterizerState> GetDefaultRasterizerState() {
			if (m_DefaultRasterizerState.IsNULL()) {
				*m_DefaultRasterizerState = *new MWDRasterizerState();
			}
			return m_DefaultRasterizerState;
		}
		static MWDPointer<MWDRasterizerState> GetScissorEnableRasterizerState() {
			if (m_ScissorEnableRasterizerState.IsNULL()) {
				*m_ScissorEnableRasterizerState = *new MWDRasterizerState();
			}
			return m_ScissorEnableRasterizerState;
		}
	protected:
		static inline MWDPointer<MWDRasterizerState> m_DefaultRasterizerState;
		static inline MWDPointer<MWDRasterizerState> m_ScissorEnableRasterizerState ;

		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);
	};
	DECLARE_Ptr(MWDRasterizerState);
	MWDTYPE_MARCO(MWDRasterizerState);
}

