#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	//VBO维护每一个顶点的数据和数据解析格式:先(bind)SetData设置数据(unbind)，(bind)再依次设置每个属性的解析方式(unbind)
	//一个VBO代表着一套对应的shader，一个VBO内的所有顶点使用同样的渲染方式
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
		//生成一个vao,并设置vbo绑定的缓存类型
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

		//第一个参数指定顶点属性位置，与顶点着色器中layout(location=0)对应。
		//第二个参数指定顶点属性大小。
		//第三个参数指定数据类型。
		//第四个参数定义是否希望数据被标准化。
		//第五个参数是步长（Stride），指定在连续的顶点属性之间的间隔。
		//第六个参数表示我们的位置数据在缓冲区起始位置的偏移量
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
