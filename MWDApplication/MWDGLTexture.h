#pragma once
#include <MWDSystem.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
namespace MWDEngine {
	class MWDGLTexture2D :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		unsigned int m_textureUnit;

		bool m_bIsBind;
		bool m_mipmap;

		unsigned int m_width;
		unsigned int m_height;

		unsigned int m_texmode;
		unsigned int m_param;

		void* m_data;
		unsigned int ui_dataBitLength;

	public:
		void Bind() {
			if (m_bIsBind) {
				return ;
			}
			m_bIsBind = true;
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_2D, m_uiID);
			return ;
		}
		void UnBind() {
			if (!m_bIsBind) {
				return;
			}
			m_bIsBind = false;
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_2D, 0);
			return;
		}
		bool IsBind()const {
			return m_bIsBind;
		}

		bool IsMipMap() {
			return m_mipmap;
		}
		void GenMipMap() {
			Bind();
			glGenerateMipmap(GL_TEXTURE_2D);
			UnBind();
		}

		unsigned int GetTextureUnit()const {
			return m_textureUnit;
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}

		MWDGLTexture2D(
			unsigned int texunit 
			,unsigned int texmode 
			,unsigned int texparam
			,unsigned int width
			,unsigned int height 
			,unsigned int textureformat
			,void* data = NULL
			,bool mipmap = false) {
			glGenTextures(1,&m_uiID);
			m_textureUnit = texunit;
			m_texmode = texmode;
			m_param = texparam;
			m_width = width;
			m_height = height;
			SetParameter(texmode, texparam);
			SetTexture(data, textureformat);
			m_bIsBind = false;
			m_mipmap = mipmap;
			ui_dataBitLength = 0;
			m_data = data;
		}
		~MWDGLTexture2D() {
			UnBind();
			glDeleteTextures(1, &m_uiID);
			delete m_data;
		}
		void SetParameter(unsigned int texmode, unsigned int param) {
			m_texmode = texmode;
			m_param = param;
			Bind();
			glTexParameteri(GL_TEXTURE_2D, m_texmode, m_param);
			UnBind();
		}
		void SetTexture(void* texture_data, unsigned int textureformat) {
			m_data = texture_data;
			ui_dataBitLength = strlen((char*)texture_data);
			Bind();
			glTexImage2D(GL_TEXTURE_2D, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
			UnBind();
		}
		void* GetData() {
			return m_data;
		}

	};

	class MWDGLTextureCubic :public MWDOpenglResource
	{
	protected:

		unsigned int m_textureUnit;
		bool m_bIsBind;

		unsigned int m_width;
		unsigned int m_height;

		unsigned int m_texmode;
		unsigned int m_param;

		unsigned int m_uiID_px;
		unsigned int m_uiID_nx;
		unsigned int m_uiID_py;
		unsigned int m_uiID_ny;
		unsigned int m_uiID_pz;
		unsigned int m_uiID_nz;

		void* m_data_px;
		void* m_data_nx;
		void* m_data_py;
		void* m_data_ny;
		void* m_data_pz;
		void* m_data_nz;

		unsigned int ui_dataBitLength_px;
		unsigned int ui_dataBitLength_nx;
		unsigned int ui_dataBitLength_py;
		unsigned int ui_dataBitLength_ny;
		unsigned int ui_dataBitLength_pz;
		unsigned int ui_dataBitLength_nz;

	public:
		void Bind() {
			if (m_bIsBind) {
				return;
			}
			m_bIsBind = true;
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_uiID_px);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_uiID_nx);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_uiID_py);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_uiID_ny);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_uiID_pz);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_uiID_nz);
			return;
		}
		void UnBind() {
			if (!m_bIsBind) {
				return;
			}
			m_bIsBind = false;
			glActiveTexture(GL_TEXTURE0 + m_textureUnit);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0);
			glBindTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0);
			return;
		}
		bool IsBind()const {
			return m_bIsBind;
		}

		unsigned int GetTextureUnit()const {
			return m_textureUnit;
		}
		//level «cubmapŒª÷√
		unsigned int GetHandle(unsigned int level)const {
			if (level == 0) {
				return m_uiID_px;
			}
			if (level == 1) {
				return m_uiID_nx;
			}
			if (level == 2) {
				return m_uiID_py;
			}
			if (level == 3) {
				return m_uiID_ny;
			}
			if (level == 4) {
				return m_uiID_pz;
			}
			if (level == 5) {
				return m_uiID_nz;
			}
			
		}
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}

		MWDGLTextureCubic(
			unsigned int texunit
			, unsigned int texmode
			, unsigned int texparam
			, unsigned int width
			, unsigned int height
			, unsigned int textureformat
			, void* data_px = NULL
			, void* data_nx = NULL
			, void* data_py = NULL
			, void* data_ny = NULL
			, void* data_pz = NULL
			, void* data_nz = NULL) {
			glGenTextures(1, &m_uiID_px);
			glGenTextures(1, &m_uiID_nx);
			glGenTextures(1, &m_uiID_py);
			glGenTextures(1, &m_uiID_ny);
			glGenTextures(1, &m_uiID_pz);
			glGenTextures(1, &m_uiID_nz);
			m_textureUnit = texunit;
			m_texmode = texmode;
			m_param = texparam;
			m_width = width;
			m_height = height;
			m_data_px = data_px;
			m_data_nx = data_nx;
			m_data_py = data_py;
			m_data_ny = data_ny;
			m_data_pz = data_pz;
			m_data_nz = data_nz;
			SetParameter(texmode, texparam);
			SetTexture(data_px, textureformat, 0);
			SetTexture(data_nx, textureformat, 1);
			SetTexture(data_py, textureformat, 2);
			SetTexture(data_ny, textureformat, 3);
			SetTexture(data_pz, textureformat, 4);
			SetTexture(data_nz, textureformat, 5);
			m_bIsBind = false;
		}
		~MWDGLTextureCubic() {
			UnBind();
			glDeleteTextures(1, &m_uiID_px);
			glDeleteTextures(1, &m_uiID_nx);
			glDeleteTextures(1, &m_uiID_py);
			glDeleteTextures(1, &m_uiID_ny);
			glDeleteTextures(1, &m_uiID_pz);
			glDeleteTextures(1, &m_uiID_nz);
			delete m_data_px;
			delete m_data_nx;
			delete m_data_py;
			delete m_data_ny;
			delete m_data_pz;
			delete m_data_nz;
		}
		void SetParameter(unsigned int texmode, unsigned int param) {
			m_texmode = texmode;
			m_param = param;
			Bind();
			glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_X, m_texmode, m_param);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, m_texmode, m_param);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, m_texmode, m_param);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, m_texmode, m_param);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, m_texmode, m_param);
			glTexParameteri(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, m_texmode, m_param);
			UnBind();
		}
		void SetTexture(void* texture_data, unsigned int textureformat,unsigned int pos) {
			if (pos == 0) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			if (pos == 1) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			if (pos == 2) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			if (pos == 3) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			if (pos == 4) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			if (pos == 5) {
				m_data_px = texture_data;
				ui_dataBitLength_px = strlen((char*)texture_data);
				Bind();
				glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, textureformat, m_width, m_height, 0, textureformat, GL_UNSIGNED_BYTE, texture_data);
				UnBind();
				return;
			}
			
		}

	};
}


