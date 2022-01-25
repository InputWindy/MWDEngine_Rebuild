#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
namespace MWDEngine {
	class  MWDIBO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		unsigned int m_usage;
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
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiID);
		}
		void unBind() {
			if (!IsBind()) {
				return ;
			}
			m_bIsBind = false;
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		MWDIBO(unsigned int usage = GL_STATIC_DRAW) {
			glGenBuffers(1, &m_uiID);
			m_bIsBind = false;
			SetState(usage);
		}
		~MWDIBO() {
			unBind();
			glDeleteBuffers(1, &m_uiID);
		}
		void SetState(unsigned int usage) {
			m_usage = usage;
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}
		bool SetData(void* data, unsigned int data_bit_size) {
			if (!data || data_bit_size == 0||m_usage==-1 ) {
				return false;
			}
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_bit_size, data, m_usage);
			unBind();
			return true;
		}

	};

}

