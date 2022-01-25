#pragma once
#include "MWDRenderState.h"
#include "MWDShader.h"
namespace MWDEngine {
	//所有渲染Pass的基类，各种Pass维护自己特有的Texture和RBO（Pass负责向Renderer提交ShaderProgram相关资源，还有FBO相关资源）
	//负责将MWDMaterial里的ShaderConstant和ShaderSampler参数传递给Renderer
	class MWDPass:public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDPass, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDPass)
		friend class MWDRenderer;
		friend class MWDMaterial;
		friend class MWDMeshRenderer;
	public:
		enum PassType {
			ShadowPass     //ShadowPass是一个全局Pass，作用于所有的光源，生成ShadowMap存储在每一个光源里
		};
		~MWDPass() {
			MWDMAC_DELETE(m_renderState)
			MWDMAC_DELETE(m_Owner)
			MWDMAC_DELETE(m_VShader)
			MWDMAC_DELETE(m_FShader)
		}
		//负责填写RenderCtx,实际的绘制由Renderer负责
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


