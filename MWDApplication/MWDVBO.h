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

		//��һ������ָ����������λ�ã��붥����ɫ����layout(location=0)��Ӧ������
		//�ڶ�������ָ���������Դ�С���ַ���������GetChannel()
		//����������ָ����������:GetDataType()
		//���ĸ����������Ƿ�ϣ�����ݱ���׼��:false
		//����������ǲ�����Stride����ָ���������Ķ�������֮��ļ������һ����������ݴ�С:m_uiOneVertexSize
		//������������ʾ���ǵ�λ�������ڻ�������ʼλ�õ�ƫ����offset
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

		//data_bit_size��data��bit����
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

