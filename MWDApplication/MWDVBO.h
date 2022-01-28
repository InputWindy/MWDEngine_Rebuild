#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDBind.h"
namespace MWDEngine {
	class MWDVBO :public MWDOpenglResource
	{
	protected:
		unsigned int m_uiID;
		unsigned int m_target;
		unsigned int m_usage;
		bool m_bisBind;
	public:
		MWDVBO(unsigned int target = GL_ARRAY_BUFFER, unsigned int usage = GL_STATIC_DRAW) {
			glGenBuffers(1, &m_uiID);
			SetState(target, usage);
			m_bisBind = false;
		}
		~MWDVBO() {
			UnBind();
			glDeleteBuffers(1, &m_uiID);
		}
		void SetState(unsigned int target,unsigned int usage) {
			m_target = target;
			m_usage = usage;
		}
		bool IsBind()const {
			return m_bisBind;
		}
		void Bind() {
			if (IsBind()) {
				return;
			}
			m_bisBind = true;
			glBindBuffer(m_target,m_uiID);
		}
		void UnBind() {
			if (!IsBind()) {
				return;
			}
			m_bisBind = false;
			glBindBuffer(m_target,0);
		}
		unsigned int GetHandle()const {
			return m_uiID;
		}

		//第一个参数指定顶点属性位置，与顶点着色器中layout(location=0)对应：自填
		//第二个参数指定顶点属性大小（字符个数）：GetChannel()
		//第三个参数指定数据类型:GetDataType()
		//第四个参数定义是否希望数据被标准化:false
		//第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔，即一个顶点的数据大小:m_uiOneVertexSize
		//第六个参数表示我们的位置数据在缓冲区起始位置的偏移量offset
		bool SetAttributePointer(unsigned int Location_in_shader, unsigned int Attibute_size,
			unsigned int Attribute_type, bool is_normalize, unsigned int Stride, unsigned int offset) {
			if (Location_in_shader < 0 || Attibute_size < 0 || m_uiID == -1 || Stride < 0 || offset < 0) {
				return false;
			}
			if (!IsBind()) {
				Bind();
			}
			glEnableVertexAttribArray(Location_in_shader);
			glVertexAttribPointer(Location_in_shader, Attibute_size, Attribute_type, is_normalize, Stride, (void*)offset);
			UnBind();
			return true;
		}

		//data_bit_size：data总bit长度
		bool SetData(void* data, unsigned int data_bit_size) {
			if (!data || data_bit_size == 0 || m_usage==-1||m_target==-1) {
				return false;
			}
			if (!IsBind()) {
				Bind();
			}
			glBufferData(m_target, data_bit_size, data, m_usage);
			UnBind();
			return true;
		}

	};
}

