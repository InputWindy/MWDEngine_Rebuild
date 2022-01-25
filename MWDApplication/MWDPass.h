#pragma once
#include "MWDRenderState.h"
#include "MWDShader.h"
namespace MWDEngine {
	//������ȾPass�Ļ��࣬����Passά���Լ����е�Texture��RBO��Pass������Renderer�ύShaderProgram�����Դ������FBO�����Դ��
	//����MWDMaterial���ShaderConstant��ShaderSampler�������ݸ�Renderer
	class MWDPass:public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDPass, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDPass)
		friend class MWDRenderer;
		friend class MWDMaterial;
		friend class MWDMeshRenderer;
	public:
		enum PassType {
			ShadowPass     //ShadowPass��һ��ȫ��Pass�����������еĹ�Դ������ShadowMap�洢��ÿһ����Դ��
		};
		~MWDPass() {
			MWDMAC_DELETE(m_renderState)
			MWDMAC_DELETE(m_Owner)
			MWDMAC_DELETE(m_VShader)
			MWDMAC_DELETE(m_FShader)
		}
		//������дRenderCtx,ʵ�ʵĻ�����Renderer����
		virtual bool Draw(MWDRenderer* pRenderer);
	protected:
		MWDRenderState*		m_renderState;
		MWDMaterial*		m_Owner;
		MWDShader*			m_VShader;
		MWDShader*			m_FShader;

		MWDPass(MWDRenderState* renderState, MWDMaterial* Owner, MWDShader* VShader,MWDShader* FShader) {
			MWDMAC_ASSERT(renderState)
			MWDMAC_ASSERT(Owner)
			MWDMAC_ASSERT(VShader)
			MWDMAC_ASSERT(FShader)

			m_renderState = renderState;
			m_Owner = Owner;
			m_VShader = VShader;
			m_FShader = FShader;
		}
	};
}


