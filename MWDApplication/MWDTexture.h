#pragma once
#include "Core/MWDResource.h"
#include "MWDGLTexture.h"
namespace MWDEngine {

	//维护一张Opengl纹理，和纹理的采样状态(没添加图片数据)
	//使用是手动设置参数，然后填充数据
	class  MWDTexture : public MWDObject, public MWDResource
	{
		DECLARE_RTTI_NoCreateFun(MWDTexture, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTexture)
	public:
		enum
		{
			MAX_MIP_LEVEL = 20
		};
		enum	//Tex Type
		{
			TT_2D,
			TT_CUBE,
			TT_MAX
		};
		enum TexMode {
			MIN = GL_TEXTURE_MIN_FILTER,
			MAG = GL_TEXTURE_MAG_FILTER,
			WRAP_S = GL_TEXTURE_WRAP_S,
			WRAP_T =  GL_TEXTURE_WRAP_T
		};
		enum TexParam {
			CLAMP = GL_CLAMP,
			LINEAR = GL_LINEAR,
			LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST
		};
		enum TexFormat {
			RGB = GL_RGB,
			RGBA = GL_RGBA
		};
		virtual ~MWDTexture() {};
	protected:
		MWDTexture() {};

		bool					m_bSRGB;
		unsigned int			m_TT;
		MWDName					m_TextrueName;
	public:
		MWDName GetTextureName()const {
			return m_TextrueName;
		}
		void SetTextureName(const TCHAR* name) {
			m_TextrueName = MWDName(name);
		}
		virtual unsigned int GetResourceType()const
		{
			return RT_TEXTURE;
		}
		virtual unsigned int GetTextureType()const {
			return TT_MAX;
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
		DECLARE_RTTI_NoCreateFun(MWDTexture2D, MWDTexture)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTexture2D)
	protected:
		MWDGLTexture2D* m_tex;
	public:
		virtual unsigned int GetTextureType()const {
			return TT_2D;
		}
		~MWDTexture2D(){
			MWDMAC_DELETE(m_tex)
		};
		MWDTexture2D(unsigned int texunit
			, TexMode texmode
			, TexParam texparam
			, unsigned int width
			, unsigned int height
			, TexFormat textureformat
			, void* data = NULL
			, bool mipmap = false) {
			m_tex = new MWDGLTexture2D(texunit,texmode,texparam,width,height,textureformat,data,mipmap);
		}
		void Bind() {
			m_tex->Bind();
		}
		void UnBind() {
			m_tex->UnBind();
		}
		bool IsBind()const {
			return m_tex->IsBind();
		}

		bool IsMipMap() {
			return m_tex->IsMipMap();
		}
		void GenMipMap() {
			m_tex->GenMipMap();
		}

		unsigned int GetTextureUnit()const {
			return m_tex->GetTextureUnit();
		}
		unsigned int GetHandle()const {
			return m_tex->GetHandle();
		}
		unsigned int GetWidth()const {
			return m_tex->GetWidth();
		}
		unsigned int GetHeight()const {
			return m_tex->GetHeight();
		}

		void SetParameter(unsigned int texmode, unsigned int param) {
			m_tex->SetParameter(texmode, param);
		}
		void SetTexture(void* texture_data, unsigned int textureformat) {
			m_tex->SetTexture(texture_data, textureformat);
		}

	};
	DECLARE_Ptr(MWDTexture2D);
	DECLARE_Proxy(MWDTexture2D);
	MWDTYPE_MARCO(MWDTexture2D);


	class  MWDTextureCubic : public MWDTexture
	{
		DECLARE_RTTI_NoCreateFun(MWDTextureCubic, MWDTexture)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTextureCubic)
	protected:
		MWDGLTextureCubic* m_tex;
	public:
		enum CubicPos{
			CUBIC_PX = 0,
			CUBIC_NX = 1,
			CUBIC_PY = 2,
			CUBIC_NY = 3,
			CUBIC_PZ = 4,
			CUBIC_NZ = 5,
		};
		virtual unsigned int GetTextureType()const {
			return TT_CUBE;
		}
		~MWDTextureCubic() {
			MWDMAC_DELETE(m_tex)
		};
		MWDTextureCubic(
			unsigned int texunit
			, TexMode texmode
			, TexParam texparam
			, unsigned int width
			, unsigned int height
			, TexFormat textureformat
			, void* data_px = NULL
			, void* data_nx = NULL
			, void* data_py = NULL
			, void* data_ny = NULL
			, void* data_pz = NULL
			, void* data_nz = NULL) {
			m_tex = new MWDGLTextureCubic(texunit, texmode, texparam, width, height, textureformat, data_px, data_nx, data_py, data_ny, data_pz, data_nz);
		}
		void Bind() {
			m_tex->Bind();
		}
		void UnBind() {
			m_tex->UnBind();
		}
		bool IsBind()const {
			return m_tex->IsBind();
		}

		unsigned int GetTextureUnit()const {
			return m_tex->GetTextureUnit();
		}
		unsigned int GetHandle(CubicPos pos)const {
			return m_tex->GetHandle(pos);
		}
		unsigned int GetWidth()const {
			return m_tex->GetWidth();
		}
		unsigned int GetHeight()const {
			return m_tex->GetHeight();
		}

		void SetParameter(unsigned int texmode, unsigned int param) {
			m_tex->SetParameter(texmode, param);
		}
		void SetTexture(void* texture_data, unsigned int textureformat, CubicPos pos) {
			m_tex->SetTexture(texture_data, textureformat,pos);
		}

	};
	DECLARE_Ptr(MWDTextureCubic);
	DECLARE_Proxy(MWDTextureCubic);
	MWDTYPE_MARCO(MWDTextureCubic);

}



