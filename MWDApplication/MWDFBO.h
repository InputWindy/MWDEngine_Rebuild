#pragma once
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
using namespace std;
namespace MWDEngine {
	class  MWDFBO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		bool m_bIsBind;
	public:
		bool IsBind()const {
			return m_bIsBind;
		}
		void bind() {
			if (IsBind()) {
				return;
			}
			m_bIsBind = true;
			glBindFramebuffer(GL_FRAMEBUFFER, m_uiID);
			return ;
		}
		void unBind() {
			if (!IsBind()) {
				return ;
			}
			m_bIsBind = false;
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return ;
		}

		MWDFBO() {
			glGenFramebuffers(1, &m_uiID);
		}
		~MWDFBO() {
			unBind();
			glDeleteFramebuffers(1, &m_uiID);
		}

		unsigned int GetHandle()const {
			return m_uiID;
		}
		bool IsFrameBufferCompleted() {
			bool flag = false;
			flag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			if (!flag) {
				cout << "FBO不完整！" << endl;
			}
			else
			{
				cout << "FBO完整！" << endl;

			}
			return flag;
		}
		bool BindTexture(unsigned int target,unsigned int attachment,unsigned int textarget,unsigned int texture,unsigned int level) {
			glFramebufferTexture2D(target, attachment, textarget, texture, level);
			return true;
		}
		bool BindRenderBuffer(unsigned int target,unsigned int attachment,unsigned int renderbuffertarget,unsigned int renderbuffer) {
			glFramebufferRenderbuffer(target,attachment,renderbuffertarget,renderbuffer);
			return true;
		}

	};

}


