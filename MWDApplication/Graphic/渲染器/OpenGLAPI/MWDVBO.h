#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	//VBOά��ÿһ����������ݺ����ݽ�����ʽ:��(bind)SetData��������(unbind)��(bind)����������ÿ�����ԵĽ�����ʽ(unbind)
	//һ��VBO������һ�׶�Ӧ��shader��һ��VBO�ڵ����ж���ʹ��ͬ������Ⱦ��ʽ
	class  MWDVBO :public MWDOpenglResource
	{
	public:
		enum Target{
				ArrayBuffer = GL_ARRAY_BUFFER,
				ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
				PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
				PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
				Target_MAX
		};
		enum Usage {
			StreamDraw = GL_STREAM_DRAW,
			StaticDraw =  GL_STATIC_DRAW,
			DynamicDraw =  GL_DYNAMIC_DRAW,
			Usage_MAX
		};
		//����һ��vao,������vbo�󶨵Ļ�������
		MWDVBO(Target target,Usage usage) {
			m_uiID = -1;
			m_bIsBind = false;
			glGenBuffers(1, &m_uiID);
			m_BufferTarget = target;
			m_Usage = usage;
		}
		~MWDVBO() {
			m_bIsBind = false;
			m_BufferTarget = Target_MAX;
			m_Usage = Usage_MAX;
			unBind();
			glDeleteBuffers(1, &m_uiID);
			m_uiID = -1;
		}
		unsigned int GetHandle() const {
			return m_uiID;
		}
		
		bool SetData(void* data,unsigned int data_bit_size) {
			if (!data || data_bit_size == 0 || m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				bind();
			}
			glBufferData(m_BufferTarget,data_bit_size,data,m_Usage);
			unBind();
			return true;
		}

		//��һ������ָ����������λ�ã��붥����ɫ����layout(location=0)��Ӧ��
		//�ڶ�������ָ���������Դ�С��
		//����������ָ���������͡�
		//���ĸ����������Ƿ�ϣ�����ݱ���׼����
		//����������ǲ�����Stride����ָ���������Ķ�������֮��ļ����
		//������������ʾ���ǵ�λ�������ڻ�������ʼλ�õ�ƫ����
		bool SetAttributePointer(unsigned int Location_in_shader,unsigned int Attibute_size,
			unsigned int Attribute_type,bool is_normalize,unsigned int Stride,unsigned int offset) {
			if (Location_in_shader<0 || Attibute_size < 0 || m_uiID == -1 || Stride <0 || offset <0) {
				return false;
			}
			if (!IsBind()) {
				bind();
			}
			glEnableVertexAttribArray(Location_in_shader);
			glVertexAttribPointer(Location_in_shader, Attibute_size, Attribute_type, is_normalize, Stride, (void*)offset);
			unBind();
			return true;
		}

	protected:
		unsigned int m_uiID;
		Target m_BufferTarget;
		Usage m_Usage;
	private:
		bool m_bIsBind;
		bool IsBind()const {
			return m_bIsBind;
		}
		bool bind() {
			if (m_uiID == -1) {
				return false;
			}
			glBindBuffer(m_BufferTarget, m_uiID);
			return true;
		}
		bool unBind() {
			if (m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				return true;
			}
			glBindBuffer(m_BufferTarget, 0);
			return true;
		}
	};
}
