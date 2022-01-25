#pragma once
#include "MWDPass.h"
namespace MWDEngine {

	//MWDMaterial维护一组Shader Program，主Pass是可编程Pass，Shader代码和shader变量都是自定义设置。其他Pass是内置Pass，shader代码是固定的，shader变量是内置变量。
	//渲染流程：MWDCuller遍历搜索所有的Entity，筛选出开启阴影投射的物体，逐光源烘焙ShadowMap。（预计算）
	//			MWDCuller遍历所有Entity，调用Entity组件MeshRenderer的Draw方法，绘制物体。
	class MWDMaterial:public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDMaterial, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMaterial)
	protected:
		friend class MWDRenderer;
		friend class MWDPass;
		friend class MWDMeshRenderer;
		
		MWDArray<MWDPass*> m_PassArr;

		MWDArray<MWDShaderConstant*>		m_pShaderConstant;                      //一个MWDShaderConstant是一个shader变量
		MWDArray<MWDShaderSampler*>			m_pShaderSampler;

		MWDShader*							m_VShader;								//自定义Pass的Shader
		MWDShader*							m_FShader;								//自定义Pass的Shader

		MWDRenderState*						m_RenderState;							//自定义Pass的RenderState
	public:
		
		bool m_CastShadow;							//是否参与生成Shadow（不参与生成Shadow，在ShadowPass的时候会被Culler剔除掉）
		bool m_ReciveShadow;						//渲染时是否读取全局ShadowPass（AddShadowPass）

		//Shader变量需要根据Shader代码对照着填写。
		MWDMaterial(MWDShader* VShader,MWDShader* FShader, MWDRenderState* m_RenderState) {
			MWDMAC_ASSERT(VShader)
			MWDMAC_ASSERT(FShader)
			MWDMAC_ASSERT(m_RenderState)
			m_VShader = VShader;
			m_FShader = FShader;
			m_RenderState = m_RenderState;
		}
		~MWDMaterial() {
			MWDMAC_DELETE(m_VShader)
			MWDMAC_DELETE(m_FShader)
			MWDMAC_DELETE(m_RenderState)
			m_PassArr.Destroy();
			m_pShaderConstant.Destroy();
			m_pShaderSampler.Destroy();
		}

		//添加内置固定Pass
		void AddPass(MWDPass* pass) {
			if (!pass) {
				return;
			}
			m_PassArr.AddElement(pass);
		}

		//设置Shader变量
		bool SetParam(const TCHAR* name, void* pData) {
			MWDName Name = MWDName(name);
			if (!pData)
			{
				return false;
			}
			for (unsigned int i = 0; i < m_pShaderConstant.GetNum(); i++)
			{
				if (Name == m_pShaderConstant[i]->GetShowName())
				{
					void* pConstanData = m_pShaderConstant[i]->GetData();
					if (!pConstanData)
					{
						return false;
					}
					MWDMemcpy(pConstanData, pData, m_pShaderConstant[i]->GetSize());
					break;
				}
			}
			return true;

		};
		bool SetParam(const TCHAR* name, MWDTexture* pTexture, unsigned int uiIndex = 0) {
			MWDName Name = MWDName(name);
			if (!pTexture)
			{
				return false;
			}
			for (unsigned int i = 0; i < m_pShaderSampler.GetNum(); i++)
			{
				if (Name == m_pShaderSampler[i]->GetName()
					&& pTexture->GetTextureType() == m_pShaderSampler[i]->GetTexType())
				{
					m_pShaderSampler[i]->SetTexture(pTexture, uiIndex);
					break;
				}
			}
			return true;

		};

		//添加Shader变量
		bool AddShaderConstant(const TCHAR* Name, const TCHAR* NameInShader, const void* pData, unsigned int uiSize,
			unsigned int uiRegisterIndex =0, unsigned int uiRegisterNum =1, unsigned int uiValueType = MWDShaderConstant::VT_FLOAT) {
			MWDName name = MWDName(Name);
			MWDName name_in_shader = MWDName(NameInShader);
			MWDShaderConstant* SC = new MWDShaderConstant(name, name_in_shader, pData, uiSize, uiRegisterIndex, uiRegisterNum, uiValueType);
			if (!SC) {
				return false;
			}
			m_pShaderConstant.AddElement(SC);
			return true;
		}
		bool AddShaderSampler(const TCHAR* Name, unsigned int uiTexType, unsigned int uiRegisterNum = 1, unsigned int uiRegisterIndex = 0) {
			MWDName name = MWDName(Name);
			MWDShaderSampler* SS = new MWDShaderSampler(name,uiTexType,uiRegisterNum,uiRegisterIndex);
			if (!SS) {
				return false;
			}
			m_pShaderSampler.AddElement(SS);
			return true;
		}
	};
}


