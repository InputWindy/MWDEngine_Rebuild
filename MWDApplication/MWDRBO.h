#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
namespace MWDEngine {

	class  MWDRBO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		unsigned int m_width;
		unsigned int m_height;
		unsigned int m_internalformat;
		bool m_bIsBind;

	public:
		void Bind() {
			if (IsBind()) {
				return;
			}
			m_bIsBind = true;
			glBindRenderbuffer(GL_RENDERBUFFER, m_uiID);
		}
		void UnBind() {
			if (!IsBind()) {
				return;
			}
			m_bIsBind = false;
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			return;
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

		MWDRBO() {
			glGenRenderbuffers(1, &m_uiID);
			m_width = -1;
			m_height = -1;
			m_internalformat = -1;
			m_bIsBind = false;
		}

		MWDRBO(unsigned int  internalformat , unsigned int width , unsigned int height ) {
			glGenRenderbuffers(1, &m_uiID);
			SetRenderBufferPara(internalformat,width, height);
			m_bIsBind = false;
		}
		~MWDRBO() {
			UnBind();
			glDeleteRenderbuffers(1, &m_uiID);
		}
		void SetRenderBufferPara(unsigned int internalformat,unsigned int width, unsigned int height) {
			Bind();
			m_width = width;
			m_height = height;
			m_internalformat = internalformat;
			glRenderbufferStorage(GL_RENDERBUFFER, internalformat, width, height);
			UnBind();
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}

	};

}

