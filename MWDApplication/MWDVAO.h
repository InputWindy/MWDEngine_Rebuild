#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
namespace MWDEngine {
	class MWDVAO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		bool m_bisBind;
	public:
		MWDVAO() {
			glGenVertexArrays(1,&m_uiID);
			m_bisBind = false;
		}
		~MWDVAO() {
			UnBind();
			glDeleteVertexArrays(1, &m_uiID);
		}
		bool IsBind()const {
			return m_bisBind;
		}
		void Bind() {
			if (IsBind()) {
				return;
			}
			m_bisBind = true;
			glBindVertexArray(m_uiID);
		}
		void UnBind() {
			if (!IsBind()) {
				return;
			}
			m_bisBind = false;
			glBindVertexArray(0);
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}
	};
}
	

