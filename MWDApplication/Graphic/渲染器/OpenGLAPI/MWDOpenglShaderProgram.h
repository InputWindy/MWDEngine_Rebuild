#pragma once
#include "OpenglAPIInclude.h"
#include "MWDOpenglShader.h"
#include "MWDOpenglTexture.h"
namespace MWDEngine {

	//shaderprogramֻά��һ��vertexshader��һ��fragmentshader
	//����ֱ�Ӵ���һ��link�õ�shaderprogram��Ҳ���Դ���һ��û��link��ɫ���Ŀ�program
	//shaderprogramʵ��һ��������opengl��Ⱦ����
	//shaderProgram������Դ��Shaders��Shader��Ӧ��uniform����ֵ
	class  MWDOpenglShaderProgram :public MWDOpenglResource
	{
	protected:
		unsigned int		m_uiID;
		bool				m_bIsLinked;
		const char*			m_LastVertexShaderCode;
		const char*			m_LastFragmentShaderCode;
		MWDOpenglShader*	m_VertexShader;
		MWDOpenglShader*	m_FragmentShader;
	public:
		MWDOpenglShaderProgram() {
			m_VertexShader = NULL;
			m_FragmentShader = NULL;
			m_LastVertexShaderCode = NULL;
			m_LastFragmentShaderCode = NULL;
			m_bIsLinked = false;
			m_uiID = glCreateProgram();
		}
		MWDOpenglShaderProgram(MWDOpenglShader* vshader, MWDOpenglShader* fshader) {
			MWDMAC_ASSERT(vshader)
			MWDMAC_ASSERT(fshader)
			m_VertexShader = vshader;
			m_FragmentShader = fshader;
			m_LastVertexShaderCode = m_VertexShader->GetShaderCode();
			m_LastFragmentShaderCode = m_FragmentShader->GetShaderCode();
			m_bIsLinked = false;
			m_uiID = glCreateProgram();
			m_bIsLinked = LinkShader(*m_VertexShader,*m_FragmentShader);
		}
		~MWDOpenglShaderProgram() {
			m_bIsLinked = false;
			glDeleteProgram(m_uiID);
		}
		bool IsLinked()const {
			return m_bIsLinked;
		}
		bool LinkShader(MWDOpenglShader& vertex_shader, MWDOpenglShader& fragment_shader) {
			if (m_VertexShader || m_FragmentShader) {
				m_bIsLinked = true;
				return true;
			}
			glAttachShader(m_uiID, vertex_shader.GetHandle());
			glAttachShader(m_uiID, fragment_shader.GetHandle());
			
			GLint log_length;
			glGetProgramiv(m_uiID, GL_INFO_LOG_LENGTH, &log_length);
			if (log_length > 0) {
				GLchar* log = new char[log_length];
				glGetProgramInfoLog(m_uiID, NULL, &log_length, NULL);
				printf("%s", log);
				delete[] log;
				m_bIsLinked = false;
				return false;
			}
			MWDMAC_DELETE(m_VertexShader)
			MWDMAC_DELETE(m_FragmentShader)
			m_bIsLinked = true;
			return true;
		}
		bool UseShaderProgram() {
			if (!m_bIsLinked) {
				return false;
			}
			glUseProgram(m_uiID);
			return true;
		}

		void setBool(const TCHAR* name, bool value) const {
			glUniform1i(glGetUniformLocation(m_uiID, (GLchar*)name), (int)value);
		};
		void setInt(const TCHAR* name, int value) const {
			glUniform1i(glGetUniformLocation(m_uiID, (GLchar*)name), (int)value);
		};
		void setFloat(const TCHAR* name, float value) const {
			glUniform1f(glGetUniformLocation(m_uiID, (GLchar*)name), value);
		};
		void setVec2(const TCHAR* name, float x, float y) const
		{
			glUniform2f(glGetUniformLocation(m_uiID, (GLchar*)name), x, y);
		}
		void setVec3(const TCHAR* name, float x, float y, float z) const
		{
			glUniform3f(glGetUniformLocation(m_uiID, (GLchar*)name), x, y, z);
		}
		void setVec4(const TCHAR* name, float x, float y, float z, float w)
		{
			glUniform4f(glGetUniformLocation(m_uiID, (GLchar*)name), x, y, z, w);
		}
		void setMat3(const TCHAR* name, const MWDMatrix3X3& mat) const
		{
			glUniformMatrix3fv(glGetUniformLocation(m_uiID, (GLchar*)name), 1, GL_FALSE, &mat.M[0][0]);
		}
		void setMat4(const TCHAR* name, const MWDMatrix3X3W& mat) const
		{
			glUniformMatrix4fv(glGetUniformLocation(m_uiID, (GLchar*)name), 1, GL_FALSE, &mat.M[0][0]);
		}
		//����shader���sampler���Դ����texture����shader���samplerȥ����texture���ڵ�texture_unit
		void setSampler2D(MWDOpenglTexture2D* texture2d,const char* sampler_name) {
			texture2d->bind();
			//����1��sampler2D������2������Ԫ
			glUniform1i(glGetUniformLocation(m_uiID, sampler_name), texture2d->GetTextureUnit());
			texture2d->unBind();
		};

	};
}


