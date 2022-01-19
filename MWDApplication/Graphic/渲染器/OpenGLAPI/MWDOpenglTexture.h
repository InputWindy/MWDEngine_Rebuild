#pragma once
#include <OpenglAPIInclude.h>

namespace MWDEngine {

	class MWDOpenglTexture :public MWDOpenglResource
	{	
	public:
		enum Filter_Mode {
			WrapS = GL_TEXTURE_WRAP_S,   
			WrapT = GL_TEXTURE_WRAP_T,
			Mag = GL_TEXTURE_MAG_FILTER,   
			Min = GL_TEXTURE_MIN_FILTER,
			FM_MAX
		};
		enum Sampling_Mode {
			Clamp = GL_CLAMP,
			ClampToEdge = GL_CLAMP_TO_EDGE,    
			ClampToBorder = GL_CLAMP_TO_BORDER,  
			ClampLinear = GL_LINEAR,    
			Nearest = GL_NEAREST,    
			MipMap_Nearest = GL_LINEAR_MIPMAP_NEAREST,
			SM_MAX
		};
		virtual ~MWDOpenglTexture() {}
	protected:
		MWDOpenglTexture() {}
		unsigned int m_textureUnit;
		unsigned int m_uiID;
		bool m_bIsBind;

		bool m_mipmap;
		unsigned int m_level; 
		
		Filter_Mode m_FM;
		Sampling_Mode m_SM;
		
	public:
		virtual bool bind() = 0;
		virtual bool unBind() = 0;
		bool IsBind()const {
			return m_bIsBind;
		}

		unsigned int GetLevel()const {
			return m_level;
		}
		unsigned int GetTextureUnit()const {
			return m_textureUnit;
		}
		
		unsigned int GetHandle()const {
			return m_uiID;
		}

		Filter_Mode GetFilterMode() const {
			return m_FM;
		}
		Sampling_Mode GetSamplingMode() const {
			return m_SM;
		}
	};
	
	//维护一张没有存放数据，只开辟显存空间的2D纹理()
	class MWDOpenglTexture2D :public MWDOpenglTexture
	{
	protected:
		unsigned int m_width;
		unsigned int m_height;
		void* m_data;
	public:
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}
		bool bind() {
			if (m_bIsBind) {
				return true;
			}
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_2D, m_uiID);
			m_bIsBind = true;
			return true;
		}
		bool unBind() {
			if (!IsBind()) {
				return true;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			m_bIsBind = false;
			return true;
		}

		const void* GetData()const {
			return m_data;
		}

		//texture_unit:默认是0
		MWDOpenglTexture2D(Filter_Mode fm = Mag, Sampling_Mode sm = ClampLinear,unsigned int bit_depth = 24,unsigned int texture_unit = 0
			,unsigned int width=800,unsigned int height=600,bool ismipmap = false,unsigned int level=0) {
			m_textureUnit = texture_unit;
			m_mipmap = ismipmap;
			m_level = level;
			m_width = width;
			m_height = height;

			glGenTextures(1, &m_uiID);
			if (m_mipmap) {
				bind();
				glGenerateMipmap(GL_TEXTURE_2D);
				unBind();
			}

			SetTextureParameter(fm, sm);
			SetTexture((void* )NULL, bit_depth);
			m_data = NULL;
		}
		~MWDOpenglTexture2D() {
			glDeleteTextures(1,&m_uiID);
			MWDMAC_DELETE(m_data)
		}
		
		void SetTexture(void* texture_data,unsigned int bit_depth) {
			m_data = texture_data;
			GLuint mode;
			if (bit_depth == 24) {
				mode = GL_RGB;
			}
			if (bit_depth == 32) {
				mode = GL_RGBA;
			}
			bind();
			glTexImage2D(GL_TEXTURE_2D, m_level, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, texture_data);
			unBind();
		}
		void SetTextureParameter(Filter_Mode fm, Sampling_Mode sm) {
			bind();
			m_FM = fm;
			m_SM = sm;
			glTexParameteri(GL_TEXTURE_2D,fm,sm);
			unBind();
		}

	};

	//维护一张没有存放数据，只开辟显存空间的3D纹理()
	class MWDOpenglTexture3D :public MWDOpenglTexture
	{
	protected:
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_depth;
		void*		 m_data;
	public:
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}
		unsigned int GetDepth()const {
			return m_depth;
		}
		const void* GetData()const {
			return m_data;
		}

		bool bind() {
			if (m_bIsBind) {
				return true;
			}
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_3D, m_uiID);
			m_bIsBind = true;
			return true;
		}
		bool unBind() {
			if (!IsBind()) {
				return true;
			}
			glBindTexture(GL_TEXTURE_3D, 0);
			m_bIsBind = false;
			return true;
		}
		
		//texture_unit:0不使用，使用1以上的
		MWDOpenglTexture3D(Filter_Mode fm = Mag, Sampling_Mode sm = ClampLinear, unsigned int bit_depth = 24, unsigned int texture_unit = 0
			, unsigned int width = 800, unsigned int height = 600,unsigned int depth=400, bool ismipmap = false, unsigned int level = 0) {
			m_textureUnit = texture_unit;
			m_mipmap = ismipmap;
			m_level = level;
			m_width = width;
			m_height = height;

			glGenTextures(1, &m_uiID);
			if (m_mipmap) {
				bind();
				glGenerateMipmap(GL_TEXTURE_3D);
				unBind();
			}

			SetTextureParameter(fm, sm);
			SetTexture((void*)NULL, bit_depth);
			m_data = NULL;
		}
		~MWDOpenglTexture3D() {
			glDeleteTextures(1, &m_uiID);
			MWDMAC_DELETE(m_data)
		}
		void SetTexture(void* texture_data,unsigned int bit_depth) {
			m_data = texture_data;
			GLuint mode;
			if (bit_depth == 24) {
				mode = GL_RGB;
			}
			if (bit_depth == 32) {
				mode = GL_RGBA;
			}
			bind();
			glTexImage3D(GL_TEXTURE_3D,m_level, mode, m_width, m_height,m_depth,0, mode, GL_UNSIGNED_BYTE,texture_data);
			unBind();
		}
		void SetTextureParameter(Filter_Mode fm, Sampling_Mode sm) {
			bind();
			m_FM = fm;
			m_SM = sm;
			glTexParameteri(GL_TEXTURE_3D, fm, sm);
			unBind();
		}
	};

	//维护一张没有存放数据，只开辟显存空间的3D纹理()
	class MWDOpenglTextureCubeMap :public MWDOpenglTexture
	{
	protected:
		unsigned int m_width;
		unsigned int m_height;

		void* m_data_px;
		void* m_data_nx;
		void* m_data_py;
		void* m_data_ny;
		void* m_data_pz;
		void* m_data_nz;
	public:
		bool bind() {
			if (m_uiID == -1 || m_bIsBind) {
				return false;
			}
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_uiID);
			m_bIsBind = true;
			return true;
		}
		bool unBind() {
			if (m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				return true;
			}
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
			glActiveTexture(GL_TEXTURE0);
			return true;
		}
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}
		bool IsBind()const {
			return m_bIsBind;
		}
		//texture_unit:需要6个texture2d,1个unit，不生成mipmap
		MWDOpenglTextureCubeMap(Filter_Mode fm = Mag, Sampling_Mode sm = ClampLinear, unsigned int bit_depth = 24, unsigned int texture_unit = 0
			, unsigned int width = 800, unsigned int height = 600, unsigned int level = 0) {
			m_textureUnit = texture_unit;
			m_mipmap = false;
			m_level = level;
			m_width = width;
			m_height = height;

			SetTextureParameter(fm, sm);

			SetTexture((void*)NULL, (void*)NULL, (void*)NULL, (void*)NULL, (void*)NULL, (void*)NULL, bit_depth);
			
		}
		~MWDOpenglTextureCubeMap() {
			glDeleteTextures(1, &m_uiID);
			MWDMAC_DELETE(m_data_px)
			MWDMAC_DELETE(m_data_nx)
			MWDMAC_DELETE(m_data_py)
			MWDMAC_DELETE(m_data_ny)
			MWDMAC_DELETE(m_data_pz)
			MWDMAC_DELETE(m_data_nz)
		}
		void SetTexture(void* data_px, void* data_nx, void* data_py, void* data_ny, void* data_pz, void* data_nz, unsigned int bit_depth) {
			m_data_px = data_px;
			m_data_nx = data_nx;
			m_data_py = data_py;
			m_data_ny = data_ny;
			m_data_pz = data_pz;
			m_data_nz = data_nz;
			GLuint mode;
			if (bit_depth == 24) {
				mode = GL_RGB;
			}
			if (bit_depth == 32) {
				mode = GL_RGBA;
			}

			bind();
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X,0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_px);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+1, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_nx);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+2, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_py);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+3, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_ny);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+4, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_pz);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+5, 0, mode, m_width, m_height, 0, mode, GL_UNSIGNED_BYTE, data_nz);
			unBind();
		}
		void SetTextureParameter(Filter_Mode fm, Sampling_Mode sm) {
			bind();
			m_FM = fm;
			m_SM = sm;
			glTexParameteri(GL_TEXTURE_CUBE_MAP, fm, sm);
			unBind();
		}
	};
}

