#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	class  MWDRBO :public MWDOpenglResource
	{
	public:
		enum RBO_Type{
			ColorBuffer,
			ZBuffer,
			StencilBuffer,
			NormalBuffer,
			BinormalBuffer,
			TangentBuffer,
			LookUpTable,
			RBOT_MAX
		};
	protected:
		unsigned int m_uiID;
		unsigned int m_width;
		unsigned int m_height;
		RBO_Type m_bufferType;
		bool m_bIsBind;
		
		bool bind() {
			if (m_uiID == -1) {
				return false;
			}
			glBindRenderbuffer(GL_RENDERBUFFER, m_uiID);
			return true;
		}
		bool unBind() {
			if (m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				return true;
			}
			glBindRenderbuffer(GL_RENDERBUFFER, 0);
			return true;
		}
	public:
		unsigned int GetWidth()const {
			return m_width;
		}
		unsigned int GetHeight()const {
			return m_height;
		}
		bool IsBind()const {
			return m_bIsBind;
		}
		MWDRBO(RBO_Type type = ColorBuffer,unsigned int width=800,unsigned int height=600) {
			m_uiID = -1;
			glGenRenderbuffers(1, &m_uiID);
			SetRenderBufferPara(width,height);
			m_bIsBind = false;
		}
		~MWDRBO() {
			unBind();
			glDeleteRenderbuffers(1, &m_uiID);
			m_uiID = -1;
		}
		void SetRenderBufferPara(unsigned int width,unsigned int height) {
			bind();
			m_width = width;
			m_height = height;
			glRenderbufferStorage(GL_RENDERBUFFER,GL_RGBA,width,height);
			unBind();
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}

	};

}

