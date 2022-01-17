#pragma once
#include "OpenglAPIInclude.h"
namespace MWDEngine {

	//维护一个编译好的Shader（保存shader代码，shader类型，是否编译）
	class  MWDOpenglShader :public MWDOpenglResource
	{
	public:
		//shader类型
		enum ShaderType {
			VertexShader = GL_VERTEX_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER,
			ST_MAX
		};
		MWDOpenglShader(const TCHAR* file_path, ShaderType shader_type) {
			m_ShaderType = shader_type;
			m_FilePath = file_path;
			m_bIsCompiled = false;
			m_ShaderCode = LoadShaderSource(file_path);
			m_uiID = glCreateShader(m_ShaderType);
			GLint length = strlen(m_ShaderCode);
			glShaderSource(m_uiID,1,&m_ShaderCode, &length);
			CompileShader();
		}
		MWDOpenglShader(char* shader_code, ShaderType shader_type) {
			m_FilePath = NULL;
			m_ShaderCode = shader_code;
			m_ShaderType = shader_type;
			m_bIsCompiled = false;
			m_uiID = glCreateShader(m_ShaderType);
			GLint length = strlen(m_ShaderCode);
			glShaderSource(m_uiID, 1, &m_ShaderCode, &length);
			CompileShader();
		}
		~MWDOpenglShader() {
			m_ShaderType = ST_MAX;
			m_bIsCompiled = false;
			m_ShaderCode = NULL;
			m_FilePath = NULL;
			glDeleteShader(m_uiID);
		}

		unsigned int GetHandle()const {
			return m_uiID;
		}
		bool IsCompiled()const {
			return m_bIsCompiled;
		}
		const TCHAR* GetFilePath()const {
			return m_FilePath;
		}
		ShaderType GetShaderType()const {
			return m_ShaderType;
		}
		char* GetShaderCode()const {
			return m_ShaderCode;
		}
		
	protected:
		unsigned int m_uiID;
		bool m_bIsCompiled;
		char* m_ShaderCode;
		const TCHAR* m_FilePath;
		ShaderType m_ShaderType;
		void CompileShader() {
			glCompileShader(m_uiID);
			
			GLint shader_state;
			glGetShaderiv(m_uiID,GL_COMPILE_STATUS,&shader_state);
			if (shader_state == GL_TRUE) {
				m_bIsCompiled = true;
				return;
			}
			GLsizei buffer_size = 0;
			glGetShaderiv(m_uiID, GL_INFO_LOG_LENGTH, &buffer_size);
			if (buffer_size > 0) {
				GLchar* buffer = new char[buffer_size];
				glGetShaderInfoLog(m_uiID, buffer_size, NULL, buffer);
				printf("%s", buffer);
				delete[] buffer;
				m_bIsCompiled = false;
				return;
			}
		}
	private:
		char* LoadShaderSource(const TCHAR* filePath)
		{
			const size_t blockSize = 512;
			FILE** fp;
			char buf[blockSize];
			char* source = NULL;
			size_t tmp, sourceLength = 0;

			fopen_s(fp,(const char*)filePath, "r");
			if (!fp) {
				cout << "shaderLoadSource():Unable to open "<< filePath<< " for reading" << endl;
				return NULL;
			}

			while ((tmp =fread(buf, 1, blockSize, *fp)) > 0)
			{
				char* newSource = new char[sourceLength + tmp + 1];
				if (!newSource) {
					fprintf(stderr, "shaderLoadSource():malloc failed\n");
					if (source) {
						MWDMAC_DELETE(source)
					}
					return NULL;
				}
				if (source) {
					memcpy(newSource, source, sourceLength);
					MWDMAC_DELETE(source);
				}
				memcpy(newSource + sourceLength, buf, tmp);
				source = newSource;
				sourceLength += tmp;
			}
			fclose(*fp);
			if (source) {
				source[sourceLength] = '\0';
			}
			return source;
		}
	};
}

