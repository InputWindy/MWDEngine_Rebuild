#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	class  MWDEBO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		bool m_bIsBind;
		
		bool bind() {
			if (m_uiID == -1) {
				return false;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_uiID);
			return true;
		}
		bool unBind() {
			if (m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				return true;
			}
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
			return true;
		}
	public:
		bool IsBind()const {
			return m_bIsBind;
		}
		MWDEBO() {
			m_uiID = -1;
			glGenBuffers(1, &m_uiID);
			m_bIsBind = false;
		}
		~MWDEBO() {
			m_bIsBind = false;
			unBind();
			glDeleteBuffers(1, &m_uiID);
			m_uiID = -1;
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}
		bool SetData(void* data, unsigned int data_bit_size) {
			if (!data || data_bit_size == 0 || m_uiID == -1) {
				return false;
			}
			bind();
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, data_bit_size, data, GL_STATIC_DRAW);
			unBind();
			return true;
		}

	};
		
}

