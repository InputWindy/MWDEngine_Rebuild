#pragma once
#include "../../Core/MWDResource.h"
#include "MWDOpenglTexture.h"
#include "../渲染状态/MWDSamplerState.h"

namespace MWDEngine {
	
	//维护一张Opengl纹理，和纹理的采样状态(没添加图片数据)
	//使用是手动设置参数，然后填充数据
	class  MWDTexture : public MWDObject, public MWDResource
	{
	public:
		enum
		{
			MAX_MIP_LEVEL = 20
		};
		enum	//Tex Type
		{
			TT_1D,
			TT_2D,
			TT_3D,
			TT_CUBE,
			TT_MAX
		};
		DECLARE_RTTI_NoCreateFun(MWDTexture, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTexture)
	protected:
		MWDOpenglTexture*		m_pTex;
		MWDSamplerStatePtr		m_pSamplerState;
		bool					m_bSRGB;
		unsigned int			m_TT;
		MWDName					m_TextrueName;
	public:
		virtual ~MWDTexture() {
			MWDMAC_DELETE(m_pTex)
			m_pTex = NULL;
			m_pSamplerState = NULL;
		};
		//默认生成一张纹理单元为1的2D纹理
		MWDTexture() {
			m_pTex = NULL;
			m_pSamplerState = NULL;
			m_bSRGB = false;
			m_TT = TT_MAX;
		}
		MWDName GetTextureName()const {
			return m_TextrueName;
		}
		void SetTextureName(const TCHAR* name){
			m_TextrueName = MWDName(name);
		}
		unsigned int GetLevel()const {
			return m_pTex->GetLevel();
		}
		unsigned int GetTextureUnit()const {
			return m_pTex->GetTextureUnit();
		}
		virtual unsigned int GetResourceType()const
		{
			return RT_TEXTURE;
		}
		virtual unsigned int GetTextureType()const {
			return TT_MAX;
		}
		virtual void SetTextureParameter(MWDOpenglTexture::Filter_Mode fm, MWDOpenglTexture::Sampling_Mode sm) = 0;
		MWDOpenglTexture::Filter_Mode GetFilterMode() const {
			return m_pTex->GetFilterMode();
		}
		MWDOpenglTexture::Sampling_Mode GetSamplingMode() const {
			return m_pTex->GetSamplingMode();
		}

	public:
		//SRGB是微软的标准颜色空间
		FORCEINLINE bool GetSRGB()const
		{
			return m_bSRGB;
		}
		void SetSRGBEable(bool bEnable) {
			m_bSRGB = bEnable;
		};
		void SetSamplerState(MWDSamplerState* pSamplerState)
		{
			if (!pSamplerState)
			{
				pSamplerState = (MWDSamplerState*)MWDSamplerState::GetDefaultSamplerState();
			}
			if (m_pSamplerState != pSamplerState)
			{
				m_pSamplerState = pSamplerState;
			}
		}
		MWDSamplerState* GetSamplerState()const
		{
			return m_pSamplerState;
		}
		//是否异步加载
		static inline bool ms_bIsEnableASYNLoader = false;
		//是否允许GC
		static inline bool ms_bIsEnableGC = true;
	};
	DECLARE_Ptr(MWDTexture);
	DECLARE_Proxy(MWDTexture);
	MWDTYPE_MARCO(MWDTexture);

	class  MWDTexture2D : public MWDTexture
	{
		DECLARE_CLASS_FUNCTION(MWDTexture2D)
		DECLARE_RTTI(MWDTexture2D, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTexture2D)
	public:
		virtual ~MWDTexture2D() {
			MWDMAC_DELETE(m_pTex)
			m_pTex = NULL;
			m_pSamplerState = NULL;
		};
		MWDTexture2D(MWDOpenglTexture::Filter_Mode fm = MWDOpenglTexture::Filter_Mode::Mag,
			MWDOpenglTexture::Sampling_Mode sm = MWDOpenglTexture::Sampling_Mode::ClampLinear,
			unsigned int bit_depth = 24,unsigned int texture_unit = 0, unsigned int width = 800, unsigned int height = 600,
			bool ismipmap = false, unsigned int level = 0) {
			m_pSamplerState = NULL;
			m_bSRGB = false;
			m_pTex = new MWDOpenglTexture2D(fm,sm,bit_depth,texture_unit,width,height,ismipmap,level);
			m_TT = TT_2D;
		};
		virtual unsigned int GetTextureType()const {
			return TT_2D;
		}
		virtual void SetTextureParameter(MWDOpenglTexture::Filter_Mode fm, MWDOpenglTexture::Sampling_Mode sm) {
			((MWDOpenglTexture2D*)m_pTex)->SetTextureParameter(fm,sm);
		}
		void SetData(void *texture_data,unsigned int bit_depth) {
			((MWDOpenglTexture2D*)m_pTex)->SetTexture(texture_data, bit_depth);
		}
		MWDOpenglTexture::Filter_Mode GetFilterMode() const {
			return m_pTex->GetFilterMode();
		}
		MWDOpenglTexture::Sampling_Mode GetSamplingMode() const {
			return m_pTex->GetSamplingMode();
		}

		unsigned int GetWidth()const {
			return ((MWDOpenglTexture2D*)m_pTex)->GetWidth();
		}
		unsigned int GetHeight()const {
			return ((MWDOpenglTexture2D*)m_pTex)->GetHeight();
		}

	};
	DECLARE_Ptr(MWDTexture2D);
	DECLARE_Proxy(MWDTexture2D);
	MWDTYPE_MARCO(MWDTexture2D);


	class  MWDTexture3D : public MWDTexture
	{
		DECLARE_CLASS_FUNCTION(MWDTexture3D)
		DECLARE_RTTI(MWDTexture3D, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTexture3D)
	public:
		virtual ~MWDTexture3D() {
			MWDMAC_DELETE(m_pTex)
				m_pTex = NULL;
			m_pSamplerState = NULL;
		};
		MWDTexture3D(MWDOpenglTexture::Filter_Mode fm = MWDOpenglTexture::Filter_Mode::Mag,
			MWDOpenglTexture::Sampling_Mode sm = MWDOpenglTexture::Sampling_Mode::ClampLinear,
			unsigned int bit_depth = 24, unsigned int texture_unit = 0, unsigned int width = 800, unsigned int height = 600,unsigned int depth=400,
			bool ismipmap = false, unsigned int level = 0) {
			m_pSamplerState = NULL;
			m_bSRGB = false;
			m_pTex = new MWDOpenglTexture3D(fm, sm, bit_depth, texture_unit, width, height, depth, ismipmap, level);
			m_TT = TT_3D;
		};
		virtual unsigned int GetTextureType()const {
			return TT_3D;
		}
		virtual void SetTextureParameter(MWDOpenglTexture::Filter_Mode fm, MWDOpenglTexture::Sampling_Mode sm) {
			((MWDOpenglTexture3D*)m_pTex)->SetTextureParameter(fm, sm);
		}
		void SetData(void* texture_data, unsigned int bit_depth) {
			((MWDOpenglTexture3D*)m_pTex)->SetTexture(texture_data, bit_depth);
		}
		MWDOpenglTexture::Filter_Mode GetFilterMode() const {
			return m_pTex->GetFilterMode();
		}
		MWDOpenglTexture::Sampling_Mode GetSamplingMode() const {
			return m_pTex->GetSamplingMode();
		}
		unsigned int GetWidth()const {
			return ((MWDOpenglTexture3D*)m_pTex)->GetWidth();
		}
		unsigned int GetHeight()const {
			return ((MWDOpenglTexture3D*)m_pTex)->GetHeight();
		}
		unsigned int GetDepth()const {
			return ((MWDOpenglTexture3D*)m_pTex)->GetDepth();
		}
	};
	DECLARE_Ptr(MWDTexture3D);
	DECLARE_Proxy(MWDTexture3D);
	MWDTYPE_MARCO(MWDTexture3D);


	class  MWDTextureCubic : public MWDTexture
	{
		DECLARE_CLASS_FUNCTION(MWDTextureCubic)
		DECLARE_RTTI(MWDTextureCubic, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTextureCubic)
	public:
		virtual ~MWDTextureCubic() {
			MWDMAC_DELETE(m_pTex)
			m_pTex = NULL;
			m_pSamplerState = NULL;
		};
		MWDTextureCubic(MWDOpenglTexture::Filter_Mode fm = MWDOpenglTexture::Filter_Mode::Mag,
			MWDOpenglTexture::Sampling_Mode sm = MWDOpenglTexture::Sampling_Mode::ClampLinear,
			unsigned int bit_depth = 24, unsigned int texture_unit = 0, unsigned int width = 800, unsigned int height = 600,
			bool ismipmap = false, unsigned int level = 0) {
			m_pSamplerState = NULL;
			m_bSRGB = false;
			m_pTex = new MWDOpenglTextureCubeMap(fm,sm,bit_depth,texture_unit,width,height,level);
			m_TT = TT_CUBE;
		};
		virtual unsigned int GetTextureType()const {
			return TT_CUBE;
		}
		virtual void SetTextureParameter(MWDOpenglTexture::Filter_Mode fm, MWDOpenglTexture::Sampling_Mode sm) {
			((MWDOpenglTexture2D*)m_pTex)->SetTextureParameter(fm, sm);
		}
		void SetData(void* data_px, void* data_nx, void* data_py, void* data_ny, void* data_pz, void* data_nz, unsigned int bit_depth) {
			((MWDOpenglTextureCubeMap*)m_pTex)->SetTexture(data_px,data_nx,data_py,data_ny,data_pz,data_nz,bit_depth);
		}
		MWDOpenglTexture::Filter_Mode GetFilterMode() const {
			return m_pTex->GetFilterMode();
		}
		MWDOpenglTexture::Sampling_Mode GetSamplingMode() const {
			return m_pTex->GetSamplingMode();
		}

		unsigned int GetWidth()const {
			return ((MWDOpenglTextureCubeMap*)m_pTex)->GetWidth();
		}
		unsigned int GetHeight()const {
			return ((MWDOpenglTextureCubeMap*)m_pTex)->GetHeight();
		}

	};
	DECLARE_Ptr(MWDTextureCubic);
	DECLARE_Proxy(MWDTextureCubic);
	MWDTYPE_MARCO(MWDTextureCubic);

}


