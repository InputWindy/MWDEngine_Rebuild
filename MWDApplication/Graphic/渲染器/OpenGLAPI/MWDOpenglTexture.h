#pragma once
#include <OpenglAPIInclude.h>

namespace MWDEngine {
	//维护一张没有存放数据，只开辟显存空间的2D纹理()
	class MWDOpenglTexture2D :public MWDOpenglResource 
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
	protected:
		unsigned int m_textureUnit;
		unsigned int m_uiID;
		bool m_bIsBind;
		unsigned int m_level; 
		unsigned int m_width;
		unsigned int m_height;
		bool m_mipmap;
		Filter_Mode m_FM;
		Sampling_Mode m_SM;
		
	public:
		bool bind() {
			if (m_uiID == -1|| m_bIsBind) {
				return false;
			}
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_2D, m_uiID);
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
			glBindTexture(GL_TEXTURE_2D, 0);
			glActiveTexture(GL_TEXTURE0);
			return true;
		}
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}
		unsigned int GetLevel()const {
			return m_level;
		}
		unsigned int GetTextureUnit()const {
			return m_textureUnit;
		}
		bool IsBind()const {
			return m_bIsBind;
		}
		//texture_unit:0不使用，使用1以上的
		MWDOpenglTexture2D(unsigned int texture_unit,unsigned int level=0,unsigned int width=800,unsigned int height=600) {
			m_textureUnit = texture_unit;
			m_mipmap = false;
			m_uiID = -1;
			m_level = level;
			m_width = width;
			m_height = height;
			glGenTextures(1, &m_uiID);
			SetTexture((void* )NULL);
			SetTextureParameter(Mag, ClampLinear);
		}
		~MWDOpenglTexture2D() {
			m_mipmap = false;
			m_level = 0;
			m_width = 0;
			m_height = 0;
			glDeleteTextures(1,&m_uiID);
			unBind();
			m_uiID = -1;
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}
		Filter_Mode GetFilterMode() const{
			return m_FM;
		}
		Sampling_Mode GetSamplingMode() const {
			return m_SM;
		}
		void SetTexture(void* texture_data) {
			bind();
			glTexImage2D(GL_TEXTURE_2D, m_level, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
			unBind();
		}
		void SetTexture(const char* file_name) {
			bind();
			MWDImage img = MWDImage(file_name);
			glTexImage2D(GL_TEXTURE_2D, m_level, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, img.GetBuffer());
			unBind();
		}
		void SetTextureParameter(Filter_Mode fm, Sampling_Mode sm) {
			bind();
			m_FM = fm;
			m_SM = sm;
			glTexParameteri(GL_TEXTURE_2D,fm,sm);
			unBind();
		}
		void GenMipMap() {
			bind();
			m_mipmap = true;
			glGenerateMipmap(GL_TEXTURE_2D);
			unBind();
		}
	};

}

