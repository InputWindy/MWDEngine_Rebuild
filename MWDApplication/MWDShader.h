#pragma once
#include "MWDShaderConstant.h"
#include "MWDShaderSampler.h"
#include "MWDBind.h"
#include "MWDGLShader.h"
namespace MWDEngine {
	//Shader只维护一个shader代码，不维护变量，变量是shaderProgram维护的
	class MWDShader:public MWDBind,public MWDResource
	{
		DECLARE_RTTI_NoCreateFun(MWDShader, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDShader)
	public:
		enum ShaderType {
			VertexShader = GL_VERTEX_SHADER,
			FragmentShader = GL_FRAGMENT_SHADER
		};
	protected:
		ShaderType							m_shaderType;
	public:
		MWDShader(MWDString* filePath, ShaderType st)
			:MWDBind(1) {
			m_shaderType = st;
			int num = GetSwapChainNum();
			for (int i = 0; i < num; ++i) {
				MWDGLShader* shader = new MWDGLShader(filePath->GetBuffer(), m_shaderType);
				AddResource(shader);
			}
		}
		MWDShader(const TCHAR* filePath, ShaderType st) {
			m_shaderType = st;
			int num = GetSwapChainNum();
			for (int i = 0; i < num; ++i) {
				MWDGLShader* shader = new MWDGLShader(filePath, m_shaderType);
				AddResource(shader);
			}
		}
		~MWDShader() {

		}
		
		unsigned int GetHandle()const {
			return ((MWDGLShader*)m_resourceArray[m_uiCurID])->GetHandle();
		}
		bool IsCompiled()const {
			return ((MWDGLShader*)m_resourceArray[m_uiCurID])->IsCompiled();
		}
		const TCHAR* GetFilePath()const {
			return ((MWDGLShader*)m_resourceArray[m_uiCurID])->GetFilePath();
		}
		unsigned int GetShaderType()const {
			return m_shaderType;
		}
		char* GetShaderCode()const {
			return ((MWDGLShader*)m_resourceArray[m_uiCurID])->GetShaderCode();
		}
	};
}


