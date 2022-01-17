#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	//VAO维护很多的VBO及其属性配置信息（bind和unbind之间配置的VBO都归当前VAO管理,绘制的时候传入一个vao）
	class  MWDVAO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		bool m_bIsBind;
	public:
		//生成一个vao
		MWDVAO() {
			m_uiID = -1;
			m_bIsBind = false;
			glGenVertexArrays(1,&m_uiID);
		}
		~MWDVAO() {
			m_uiID = -1;
			m_bIsBind = false;
			glDeleteVertexArrays(1, &m_uiID);
		}
		unsigned int GetHandle() const{
			return m_uiID;
		}
		bool IsBind() const {
			return m_bIsBind;
		}
		bool bind() {
			if (m_uiID == -1) {
				return false;
			}
			if (IsBind()) {
				return true;
			}
			glBindVertexArray(m_uiID);
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
			glBindVertexArray(0);
			m_bIsBind = false;
			return true;
		}

	};
}


