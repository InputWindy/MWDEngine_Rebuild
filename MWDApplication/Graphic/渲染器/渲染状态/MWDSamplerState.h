#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	class MWDStream;
	class  MWDSamplerDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDSamplerDesc)
		DECLARE_RTTI(MWDSamplerDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDSamplerDesc)
	public:

		//纹理坐标模式
		enum 
		{
			CM_CLAMP,
			CM_WRAP,
			CM_MIRROR,
			CM_BORDER,
			CM_MAX
		};
		//滤波模式
		enum //Filter Method
		{
			FM_NONE,
			FM_POINT,
			FM_LINE,
			FM_ANISOTROPIC,
			FM_MAX

		};
		MWDSamplerDesc()
		{
			m_uiMag = FM_POINT;
			m_uiMin = FM_POINT;
			m_uiMip = FM_NONE;
			m_uiMipLevel = 0;
			m_uiAniLevel = 1;

			m_uiCoordU = CM_WRAP;
			m_uiCoordV = CM_WRAP;
			m_uiCoordW = CM_WRAP;
			m_BorderColor = MWDColorRGBA(0.0f, 0.0f, 0.0f, 0.0f);
			m_MipLODBias = 0.0f;
		}
		~MWDSamplerDesc()
		{

		}



		unsigned char m_uiMag;
		unsigned char m_uiMin;
		unsigned char m_uiMip;
		unsigned char m_uiMipLevel;
		unsigned char m_uiAniLevel;

		unsigned char m_uiCoordU;
		unsigned char m_uiCoordV;
		unsigned char m_uiCoordW;
		MWDREAL       m_MipLODBias;
		MWDColorRGBA  m_BorderColor;
		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDSamplerDesc) - sizeof(MWDObject);
			return (void*)&m_uiMag;
		}
	};
	MWDTYPE_MARCO(MWDSamplerDesc);



	class  MWDSamplerState : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDSamplerState)
		DECLARE_RTTI(MWDSamplerState, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDSamplerState)
	public:
		MWDSamplerState() {
			m_bIsStatic = 1;
		};
		virtual ~MWDSamplerState() {
			ReleaseResource();
		};
	protected:
		MWDSamplerDesc m_SamplerDesc;
	protected:
		static inline MWDPointer<MWDSamplerState> m_DefaultSamplerState ;
		static inline MWDPointer<MWDSamplerState> m_TriLineSamplerState ;
		static inline MWDPointer<MWDSamplerState> m_DoubleLineSamplerState ;
		static inline MWDPointer<MWDSamplerState> m_ShadowMapSamplerState ;
		static inline MWDPointer<MWDSamplerState> m_BorderARGB0SamplerState ;
		static inline MWDPointer<MWDSamplerState> m_LightFunctionSamplerState ;
	public:
		static MWDPointer<MWDSamplerState> GetDefaultSamplerState() {
			if (m_DefaultSamplerState.IsNULL()) {
				*m_DefaultSamplerState = *new MWDSamplerState();
			}
			return m_DefaultSamplerState;
		}
		static MWDPointer<MWDSamplerState> GetTriLineSamplerState() {
			if (m_TriLineSamplerState.IsNULL()) {
				*m_TriLineSamplerState = *new MWDSamplerState();
			}
			return m_TriLineSamplerState;
		}
		static MWDPointer<MWDSamplerState> GetDoubleLineSamplerState() {
			if (m_DoubleLineSamplerState.IsNULL()) {
				*m_DoubleLineSamplerState = *new MWDSamplerState();
			}
			return m_DoubleLineSamplerState;
		}
		static MWDPointer<MWDSamplerState> GetShadowMapSamplerState() {
			if (m_ShadowMapSamplerState.IsNULL()) {
				*m_ShadowMapSamplerState = *new MWDSamplerState();
			}
			return m_ShadowMapSamplerState;
		}
		static MWDPointer<MWDSamplerState> GetBorderARGB0SamplerState() {
			if (m_BorderARGB0SamplerState.IsNULL()) {
				*m_BorderARGB0SamplerState = *new MWDSamplerState();
			}
			return m_BorderARGB0SamplerState;
		}
		static MWDPointer<MWDSamplerState> GetLightFunctionSamplerState() {
			if (m_LightFunctionSamplerState.IsNULL()) {
				*m_LightFunctionSamplerState = *new MWDSamplerState();
			}
			return m_LightFunctionSamplerState;
		}

		static bool InitialDefaultState() { return true; };
		static bool TerminalDefaultState() { return true; };
	public:
		const MWDSamplerDesc& GetSamplerDesc()const
		{
			return m_SamplerDesc;
		}
		friend class MWDResourceManager;
	protected:
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);
	};
	DECLARE_Ptr(MWDSamplerState);
	MWDTYPE_MARCO(MWDSamplerState);
}

