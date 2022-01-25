#pragma once
#include <MWDMemManager.h>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "MWDString.h"
#include <MWDCompute.h>
using namespace std;
namespace MWDEngine {
	class MWDGLShaderProgram
	{
	protected:
		unsigned int		m_uiID;
		bool				m_bIsLinked;

		char* m_VertexShaderCode;
		char* m_FragmentShaderCode;

	public:
		MWDGLShaderProgram(unsigned int vShader,unsigned int fShader,char* vShaderCode = NULL,char* fShaderCode = NULL) {
			m_VertexShaderCode = vShaderCode;
			m_FragmentShaderCode = fShaderCode;
			m_bIsLinked = false;
			m_uiID = glCreateProgram();
			LinkShader(vShader, fShader);
		}
		~MWDGLShaderProgram() {
			MWDMAC_DELETE(m_VertexShaderCode)
			MWDMAC_DELETE(m_FragmentShaderCode)
			glDeleteProgram(m_uiID);
		}
		bool IsLinked()const {
			return m_bIsLinked;
		}
		bool UseShaderProgram() {
			if (!m_bIsLinked) {
				return false;
			}
			glUseProgram(m_uiID);
			return true;
		}
        void setBool(const TCHAR* name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_uiID, TcharToChar(name)), (int)value);
        }
        void setInt(const TCHAR* name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_uiID, TcharToChar(name)), value);
        }
        void setFloat(const TCHAR* name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_uiID, TcharToChar(name)), value);
        }
        void setVec2(const TCHAR* name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(m_uiID, TcharToChar(name)), x, y);
        }
        void setVec3(const TCHAR* name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(m_uiID, TcharToChar(name)), x, y, z);
        }
        void setVec4(const TCHAR* name, float x, float y, float z, float w)
        {
            glUniform4f(glGetUniformLocation(m_uiID, TcharToChar(name)), x, y, z, w);
        }
        void setMat3(const TCHAR* name, const MWDMatrix3X3& mat) 
        {
            glUniformMatrix3fv(glGetUniformLocation(m_uiID, TcharToChar(name)), 1, GL_FALSE, &mat.M[0][0]);
        }
        void setMat4(const TCHAR* name, const MWDMatrix3X3W& mat) 
        {
            glUniformMatrix4fv(glGetUniformLocation(m_uiID, TcharToChar(name)), 1, GL_FALSE, &mat.M[0][0]);
        }
	private:
		bool LinkShader(unsigned int vshader,unsigned int fshader) {
			glAttachShader(m_uiID, vshader);
			glAttachShader(m_uiID, fshader);

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
			m_bIsLinked = true;
			return true;
		}
	};
}


