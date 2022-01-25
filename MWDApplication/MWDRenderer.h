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
		MWDREAL					m_fClearDepth;				//深度测试值
		unsigned int			m_uiClearStencil;			//模板测试值

		MWDArray<MWDMeshData*>	m_Mesh;						//当前DrawCall需要绘制的Mesh集合。同一渲染队列里的相同材质的Mesh参与静态批处理

		//渲染状态
		MWDRasterizerState*		m_RS;
		MWDBlendState*			m_BS;
		MWDDepthStencilState*	m_DSS;

		//ShaderProgram
		MWDShader*							m_VShader;
		MWDShader*							m_FShader;
		MWDArray<MWDShaderConstant*>		m_pShaderConstant;                      //一个MWDShaderConstant是一个shader变量
		MWDArray<MWDShaderSampler*>			m_pShaderSampler;						//一个MWDShaderSampler是一个Shader采样器

		//Camera
		MWDCamera*							m_curCamera;                            //当前的摄像机

		//Light
		MWDArray<MWDLightEntity*>			m_curLight;                             //所有参与渲染的光源。每一个光源都会维护一张ShadowMap

		//渲染结果缓冲（FBO离屏渲染结果）
		MWDTexture2D*						m_FrameBuffer;							//屏幕渲染缓冲
		MWDTextureCubic*					m_PreComputeBuffer;						//预计算缓冲
		MWDRBO*								m_rbo;									//多Pass物体渲染时，各Pass之间进行通信的媒介
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
		HWND m_hMainWindow;						// 主窗口
		bool m_bIsRendering;					//渲染器是否正在工作
		MWDColorRGBA m_ClearColor;				//背景色
		
		MWDRenderContex m_renderCtx;			//渲染上下文

		UINT m_uiScreenWidth;					//主屏幕宽度
		UINT m_uiScreenHeight;					//主屏幕高度
		
		bool m_bWindowed;						//窗口是否可用

		MWDViewPort m_CurViewPort;				//当前视口
	
		static inline MWDRenderer* ms_pRenderer = NULL;		//主渲染器
	public:
		MWDRenderer() {}
		~MWDRenderer() {}
		static MWDRenderer* GetMainRenderer() {
			if (!ms_pRenderer) {
				ms_pRenderer = new MWDRenderer();
			}
			return ms_pRenderer;
		}

		//设置背景色
		FORCEINLINE void SetClearColor(const MWDColorRGBA& ClearColor) {
			m_ClearColor = ClearColor;
		};
		//获取背景色
		FORCEINLINE const MWDColorRGBA& GetClearColor() const {
			return m_ClearColor;
		};
		//设置深度测试值
		FORCEINLINE void SetClearDepth(MWDREAL fClearDepth) {
			m_renderCtx.m_fClearDepth = fClearDepth;
		};
		//获取深度测试值            
		FORCEINLINE MWDREAL GetClearDepth() const {
			return m_renderCtx.m_fClearDepth;
		};
		//设置模板测试值
		FORCEINLINE void SetClearStencil(unsigned int uiClearValue) {
			m_renderCtx.m_uiClearStencil = uiClearValue;
		};
		//获取模板测试值       
		FORCEINLINE unsigned int GetClearStencil() const {
			return m_renderCtx.m_uiClearStencil;
		};
		//清空各种buffer
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

		//窗口是否有效
		FORCEINLINE bool IsWindowed()const {
			return m_bWindowed;
		};
	public:
		bool BeginRendering() {};                            //清空各种buffer，m_bIsRendering = true;
		bool EndRendering() {};  
		//只填充数据
		bool SetRenderCtx(MWDMeshData* mesh) {
			if (!mesh) {
				return false;
			}
			m_renderCtx.m_Mesh.AddElement(mesh);
			return true;
		};      
		//只填充数据
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

		//FBO颜色输出默认用的是Location = 0
		bool DrawMesh() {
			if (!m_renderCtx.IsReady()) {
				return false;
			}
			MWDRasterizerDesc R_Desc = m_renderCtx.m_RS->GetRasterizerDesc();
			MWDBlendDesc B_Desc = m_renderCtx.m_BS->GetBlendDesc();
			MWDDepthStencilDesc DS_Desc = m_renderCtx.m_DSS->GetDepthStencilDesc();
			#pragma region 渲染状态解析及设置
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
			//绑定ShaderProgram
			shader_program.UseShaderProgram();
			if (shader_program.IsLinked()){
				//填写ShaderProgram的Uniform值
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
				//绑定Shader采样器纹理单元
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
			//绑定FBO
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


