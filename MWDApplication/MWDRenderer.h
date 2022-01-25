#pragma once
#include "MWDMeshRenderer.h"
#include "MWDWorld.h"

#include "MWDVAO.h"
#include "MWDVBO.h"
#include "MWDIBO.h"
#include "MWDFBO.h"
#include "MWDRBO.h"
#include "MWDGLShaderProgram.h"
#include "MWDGLTexture.h"
namespace MWDEngine {

	class MWDRenderContex {
		friend class MWDMeshRenderer;
	public:
		MWDREAL					m_fClearDepth;				//��Ȳ���ֵ
		unsigned int			m_uiClearStencil;			//ģ�����ֵ

		MWDArray<MWDMeshData*>	m_Mesh;						//��ǰDrawCall��Ҫ���Ƶ�Mesh���ϡ�ͬһ��Ⱦ���������ͬ���ʵ�Mesh���뾲̬������

		//��Ⱦ״̬
		MWDRasterizerState*		m_RS;
		MWDBlendState*			m_BS;
		MWDDepthStencilState*	m_DSS;

		//ShaderProgram
		MWDShader*							m_VShader;
		MWDShader*							m_FShader;
		MWDArray<MWDShaderConstant*>		m_pShaderConstant;                      //һ��MWDShaderConstant��һ��shader����
		MWDArray<MWDShaderSampler*>			m_pShaderSampler;						//һ��MWDShaderSampler��һ��Shader������

		//Camera
		MWDCamera*							m_curCamera;                            //��ǰ�������

		//Light
		MWDArray<MWDLightEntity*>			m_curLight;                             //���в�����Ⱦ�Ĺ�Դ��ÿһ����Դ����ά��һ��ShadowMap

		//��Ⱦ������壨FBO������Ⱦ�����
		MWDTexture2D*						m_FrameBuffer;							//��Ļ��Ⱦ����
		MWDTextureCubic*					m_PreComputeBuffer;						//Ԥ���㻺��
		MWDRBO*								m_rbo;									//��Pass������Ⱦʱ����Pass֮�����ͨ�ŵ�ý��
		bool IsReady() {
			return (
				m_Mesh.GetNum()>0&&
				m_RS&&
				m_BS&&
				m_DSS&&
				m_VShader&&
				m_FShader&&
				m_FrameBuffer
				);
		}
	};

	class MWDRenderer
	{
	
	protected:
		HWND m_hMainWindow;						// ������
		bool m_bIsRendering;					//��Ⱦ���Ƿ����ڹ���
		MWDColorRGBA m_ClearColor;				//����ɫ
		
		MWDRenderContex m_renderCtx;			//��Ⱦ������

		UINT m_uiScreenWidth;					//����Ļ���
		UINT m_uiScreenHeight;					//����Ļ�߶�
		
		bool m_bWindowed;						//�����Ƿ����

		MWDViewPort m_CurViewPort;				//��ǰ�ӿ�
	
		static inline MWDRenderer* ms_pRenderer = NULL;		//����Ⱦ��
	public:
		MWDRenderer() {}
		~MWDRenderer() {}
		static MWDRenderer* GetMainRenderer() {
			if (!ms_pRenderer) {
				ms_pRenderer = new MWDRenderer();
			}
			return ms_pRenderer;
		}

		//���ñ���ɫ
		FORCEINLINE void SetClearColor(const MWDColorRGBA& ClearColor) {
			m_ClearColor = ClearColor;
		};
		//��ȡ����ɫ
		FORCEINLINE const MWDColorRGBA& GetClearColor() const {
			return m_ClearColor;
		};
		//������Ȳ���ֵ
		FORCEINLINE void SetClearDepth(MWDREAL fClearDepth) {
			m_renderCtx.m_fClearDepth = fClearDepth;
		};
		//��ȡ��Ȳ���ֵ            
		FORCEINLINE MWDREAL GetClearDepth() const {
			return m_renderCtx.m_fClearDepth;
		};
		//����ģ�����ֵ
		FORCEINLINE void SetClearStencil(unsigned int uiClearValue) {
			m_renderCtx.m_uiClearStencil = uiClearValue;
		};
		//��ȡģ�����ֵ       
		FORCEINLINE unsigned int GetClearStencil() const {
			return m_renderCtx.m_uiClearStencil;
		};
		//��ո���buffer
		virtual void ClearBuffers(unsigned int uiClearFlag) {};                 
		virtual void ClearBackBuffer() {};
		virtual void ClearZBuffer() {};
		virtual void ClearStencilBuffer() {};
		virtual void ClearBuffers() {};

		virtual void ClearBuffers(unsigned int uiClearFlag, int iXPos, int iYPos, int iWidth,int iHeight) {};
		virtual void ClearBackBuffer(int iXPos, int iYPos, int iWidth,int iHeight) {};
		virtual void ClearZBuffer(int iXPos, int iYPos, int iWidth,int iHeight) {};
		virtual void ClearStencilBuffer(int iXPos, int iYPos, int iWidth,int iHeight) {};
		virtual void ClearBuffers(int iXPos, int iYPos, int iWidth,int iHeight) {};

		//�����Ƿ���Ч
		FORCEINLINE bool IsWindowed()const {
			return m_bWindowed;
		};
	public:
		bool BeginRendering() {};                            //��ո���buffer��m_bIsRendering = true;
		bool EndRendering() {};  
		//ֻ�������
		bool SetRenderCtx(MWDMeshData* mesh) {
			if (!mesh) {
				return false;
			}
			m_renderCtx.m_Mesh.AddElement(mesh);
			return true;
		};      
		//ֻ�������
		bool SetRenderCtx(MWDPass* pass) {
			if (!pass) {
				return false;
			}
			m_renderCtx.m_VShader = pass->m_VShader;
			m_renderCtx.m_FShader = pass->m_FShader;
			m_renderCtx.m_BS = pass->m_Owner->m_RenderState->GetBlendState();
			m_renderCtx.m_DSS = pass->m_Owner->m_RenderState->GetDepthStencilState();
			m_renderCtx.m_RS = pass->m_Owner->m_RenderState->GetRasterizerState();
			m_renderCtx.m_pShaderConstant = pass->m_Owner->m_pShaderConstant;
			m_renderCtx.m_pShaderSampler = pass->m_Owner->m_pShaderSampler;
			return true;
		};

		//FBO��ɫ���Ĭ���õ���Location = 0
		bool DrawMesh() {
			if (!m_renderCtx.IsReady()) {
				return false;
			}
			MWDRasterizerDesc R_Desc = m_renderCtx.m_RS->GetRasterizerDesc();
			MWDBlendDesc B_Desc = m_renderCtx.m_BS->GetBlendDesc();
			MWDDepthStencilDesc DS_Desc = m_renderCtx.m_DSS->GetDepthStencilDesc();
			#pragma region ��Ⱦ״̬����������
			//R_Desc
			bool bWireEnable = R_Desc.m_bWireEnable;
			unsigned char uiCullType = R_Desc.m_bClipPlaneEnable;
			bool bClipPlaneEnable = R_Desc.m_bClipPlaneEnable;
			bool bScissorTestEnable = R_Desc.m_bScissorTestEnable;

			//B_Desc
			bool IsBlendUsed = B_Desc.IsBlendUsed();
			bool			bAlphaToCoverageEnable = B_Desc.bAlphaToCoverageEnable;
			bool			bIndependentBlendEnable = B_Desc.bIndependentBlendEnable;

			unsigned int max_target_num = MWDBlendDesc::MAX_RENDER_TARGET_NUM;
			MWDREAL* fBlendColor = &(B_Desc.fBlendColor[0]);
			unsigned int	ucSampleMask = B_Desc.ucSampleMask;
			bool* bBlendEnable = &(B_Desc.bBlendEnable[0]);
			unsigned char* ucSrcBlend = &(B_Desc.ucSrcBlend[0]);
			unsigned char* ucDestBlend = &(B_Desc.ucDestBlend[0]);
			unsigned char* ucBlendOp = &(B_Desc.ucBlendOp[0]);

			bool* bAlphaBlendEnable = &(B_Desc.bAlphaBlendEnable[0]);
			unsigned char* ucSrcBlendAlpha = &(B_Desc.ucSrcBlendAlpha[0]);
			unsigned char* ucDestBlendAlpha = &(B_Desc.ucDestBlendAlpha[0]);
			unsigned char* ucBlendOpAlpha = &(B_Desc.ucBlendOpAlpha[0]);
			unsigned char* ucWriteMask = &(B_Desc.ucWriteMask[0]);

			//DS_Desc
			bool m_bDepthEnable = DS_Desc.m_bDepthEnable;
			bool m_bDepthWritable = DS_Desc.m_bDepthWritable;

			unsigned char m_uiDepthCompareMethod = DS_Desc.m_uiDepthCompareMethod;

			bool m_bStencilEnable = DS_Desc.m_bStencilEnable;
			unsigned char m_uiStencilCompareMethod = DS_Desc.m_uiStencilCompareMethod;
			unsigned char m_uiReference = DS_Desc.m_uiReference;
			unsigned char m_uiMask = DS_Desc.m_uiMask;
			unsigned char m_uiWriteMask = DS_Desc.m_uiWriteMask;
			unsigned char m_uiSPassZPassOP = DS_Desc.m_uiSPassZPassOP;
			unsigned char m_uiSPassZFailOP = DS_Desc.m_uiSPassZFailOP;
			unsigned char m_uiSFailZPassOP = DS_Desc.m_uiSFailZPassOP;

			unsigned char m_uiCCW_StencilCompareMethod = DS_Desc.m_uiCCW_StencilCompareMethod;
			unsigned char m_uiCCW_SPassZPassOP = DS_Desc.m_uiCCW_SPassZPassOP;
			unsigned char m_uiCCW_SPassZFailOP = DS_Desc.m_uiCCW_SPassZFailOP;
			unsigned char m_uiCCW_SFailZPassOP = DS_Desc.m_uiCCW_SFailZPassOP;
			bool m_bTwoSideStencilMode = DS_Desc.m_bTwoSideStencilMode;
			#pragma endregion
			MWDVAO vao = MWDVAO();
			vao.Bind();
			int vbo_num = m_renderCtx.m_Mesh.GetNum();
			int vertex_cnt = 0;
			for (int i = 0; i < vbo_num; ++i) {
				((MWDIBO*)(m_renderCtx.m_Mesh[i]->GetIndexBuffer()->GetResource()))->bind();
				((MWDVBO*)(m_renderCtx.m_Mesh[i]->GetVertexBuffer()->GetResource()))->Bind();
				vertex_cnt += m_renderCtx.m_Mesh[i]->GetVertexBuffer()->GetNum();
			}
			MWDShader* vshader = m_renderCtx.m_VShader;
			MWDShader* fshader = m_renderCtx.m_FShader;
			MWDGLShaderProgram shader_program = MWDGLShaderProgram(vshader->GetHandle(), fshader->GetHandle(),vshader->GetShaderCode(),fshader->GetShaderCode());
			//��ShaderProgram
			shader_program.UseShaderProgram();
			if (shader_program.IsLinked()){
				//��дShaderProgram��Uniformֵ
				int shader_constant_num = m_renderCtx.m_pShaderConstant.GetNum();
				for (int i = 0; i < shader_constant_num; ++i) {
					MWDShaderConstant* sc = m_renderCtx.m_pShaderConstant[i];
					auto constant = sc->GetData();
					unsigned int type = sc->GetValueType();
					const TCHAR* name = sc->GetNameInShader().GetBuffer();
					if (type ==MWDShaderConstant::VT_FLOAT) {
						shader_program.setFloat(name,*(float*)constant);
					}
					else if (type == MWDShaderConstant::VT_BOOL) {
						shader_program.setBool(name, *(bool*)constant);
					}
					else if (type == MWDShaderConstant::VT_INT) {
						shader_program.setInt(name, *(int*)constant);
					}
					else if (type == MWDShaderConstant::VT_VEC2) {
						shader_program.setVec2(name, *(float*)constant, *((float*)constant+sizeof(float)));
					}
					else if (type == MWDShaderConstant::VT_VEC3) {
						shader_program.setVec3(name, *(float*)constant, *((float*)constant + sizeof(float)) , *((float*)constant +2*sizeof(float)));
					}
					else if (type == MWDShaderConstant::VT_VEC4) {
						shader_program.setVec4(name, *(float*)constant, *((float*)constant + sizeof(float)), *((float*)constant + 2 * sizeof(float)), *((float*)constant + 3 * sizeof(float)));
					}
					else if (type == MWDShaderConstant::VT_MAT3) {
						shader_program.setMat3(name, *(MWDMatrix3X3*)constant);
					}
					else if (type == MWDShaderConstant::VT_MAT4) {
						shader_program.setMat4(name, *(MWDMatrix3X3W*)constant);
					}
					else {
						MWDMAC_ASSERT(0);
						return false;
					}
					
				}
				//��Shader����������Ԫ
				int shader_sampler_num = m_renderCtx.m_pShaderSampler.GetNum();
				for (int i = 0; i < shader_sampler_num; ++i) {
					MWDShaderSampler* shader_sampler = m_renderCtx.m_pShaderSampler[i];
					const TCHAR* name = shader_sampler->GetName().GetBuffer();
					MWDTexture* tex = shader_sampler->GetTex();
					if (tex->GetTextureType() == MWDTexture::TT_2D) {
						unsigned int tex_unit = ((MWDTexture2D*)tex)->GetTextureUnit();
						((MWDTexture2D*)tex)->Bind();
						shader_program.setInt(name, tex_unit);
					}
					
				}
			}
			//��FBO
			MWDFBO fbo = MWDFBO();
			fbo.bind();
			fbo.BindTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,m_renderCtx.m_FrameBuffer->GetHandle(),0);
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			vao.UnBind();
			fbo.unBind();
			return true;
		}

	};
}


