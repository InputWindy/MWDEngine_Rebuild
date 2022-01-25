#pragma once
#include "MWDPass.h"
namespace MWDEngine {

	//MWDMaterialά��һ��Shader Program����Pass�ǿɱ��Pass��Shader�����shader���������Զ������á�����Pass������Pass��shader�����ǹ̶��ģ�shader���������ñ�����
	//��Ⱦ���̣�MWDCuller�����������е�Entity��ɸѡ��������ӰͶ������壬���Դ�決ShadowMap����Ԥ���㣩
	//			MWDCuller��������Entity������Entity���MeshRenderer��Draw�������������塣
	class MWDMaterial:public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDMaterial, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDMaterial)
	protected:
		friend class MWDRenderer;
		friend class MWDPass;
		friend class MWDMeshRenderer;
		
		MWDArray<MWDPass*> m_PassArr;

		MWDArray<MWDShaderConstant*>		m_pShaderConstant;                      //һ��MWDShaderConstant��һ��shader����
		MWDArray<MWDShaderSampler*>			m_pShaderSampler;

		MWDShader*							m_VShader;								//�Զ���Pass��Shader
		MWDShader*							m_FShader;								//�Զ���Pass��Shader

		MWDRenderState*						m_RenderState;							//�Զ���Pass��RenderState
	public:
		
		bool m_CastShadow;							//�Ƿ��������Shadow������������Shadow����ShadowPass��ʱ��ᱻCuller�޳�����
		bool m_ReciveShadow;						//��Ⱦʱ�Ƿ��ȡȫ��ShadowPass��AddShadowPass��

		//Shader������Ҫ����Shader�����������д��
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

		//������ù̶�Pass
		void AddPass(MWDPass* pass) {
			if (!pass) {
				return;
			}
			m_PassArr.AddElement(pass);
		}

		//����Shader����
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

		//���Shader����
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


