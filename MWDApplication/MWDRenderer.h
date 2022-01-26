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
		
		bool m_bEnableMSAA;						//开启MSAA

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


		//设置MSAA
		void SetMSAA(bool en) {
			m_bEnableMSAA = en;
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
		virtual void ClearBuffers(unsigned int uiClearFlag) {
			glClear(uiClearFlag);
		};         
		virtual void ClearColorBuffer() {
			glClear(GL_COLOR_BUFFER_BIT);
		}
		virtual void ClearZBuffer() {
			glClear(GL_DEPTH_BUFFER_BIT);
		};
		virtual void ClearStencilBuffer() {
			glClear(GL_STENCIL_BUFFER_BIT);
		};
		virtual void ClearAllBuffers() {
			ClearColorBuffer();
			ClearZBuffer();
			ClearStencilBuffer();
		}
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
			if (m_bEnableMSAA) {
				glEnable(GL_MULTISAMPLE);
			}
			MWDRasterizerDesc R_Desc = m_renderCtx.m_RS->GetRasterizerDesc();
			MWDBlendDesc B_Desc = m_renderCtx.m_BS->GetBlendDesc();
			MWDDepthStencilDesc DS_Desc = m_renderCtx.m_DSS->GetDepthStencilDesc();
			#pragma region 渲染状态解析及设置
			//R_Desc(暂时没用到)
			bool bWireEnable = R_Desc.m_bWireEnable;
			unsigned char uiCullType = R_Desc.m_bClipPlaneEnable;
			bool bClipPlaneEnable = R_Desc.m_bClipPlaneEnable;
			bool bScissorTestEnable = R_Desc.m_bScissorTestEnable;

			//B_Desc
			bool			IsBlendUsed = B_Desc.IsBlendUsed();							//ok
			bool			bAlphaToCoverageEnable = B_Desc.bAlphaToCoverageEnable;		//ok（需要启用MSAA）
			bool			bIndependentBlendEnable = B_Desc.bIndependentBlendEnable;	//意义不明

			MWDREAL fBlendColor[4] = {B_Desc.fBlendColor[0],B_Desc.fBlendColor[1],B_Desc.fBlendColor[2], B_Desc.fBlendColor[3]}; //暂时没用到
			unsigned int  ucSampleMask = B_Desc.ucSampleMask;			//意义不明
			unsigned char ucSrcBlend = (B_Desc.ucSrcBlend);				//ok
			unsigned char ucDestBlend = (B_Desc.ucDestBlend);			//ok
			unsigned char ucBlendOp = (B_Desc.ucBlendOp);				//ok

			unsigned char ucSrcBlendAlpha = (B_Desc.ucSrcBlendAlpha);	//ok
			unsigned char ucDestBlendAlpha = (B_Desc.ucDestBlendAlpha);	//ok
			unsigned char ucBlendOpAlpha = (B_Desc.ucBlendOpAlpha);		//ok
			unsigned char ucWriteMask = (B_Desc.ucWriteMask);			//意义不明

			//DS_Desc
			bool m_bDepthEnable = DS_Desc.m_bDepthEnable;                               //ok
			bool m_bDepthWritable = DS_Desc.m_bDepthWritable;                           //ok

			unsigned char m_uiDepthCompareMethod = DS_Desc.m_uiDepthCompareMethod;	    //ok

			bool m_bStencilEnable = DS_Desc.m_bStencilEnable;							//ok
			unsigned char m_uiStencilCompareMethod = DS_Desc.m_uiStencilCompareMethod;	//ok
			unsigned char m_uiReference = DS_Desc.m_uiReference;						//意义不明
			unsigned char m_uiMask = DS_Desc.m_uiMask;									//ok
			unsigned char m_uiWriteMask = DS_Desc.m_uiWriteMask;						//ok
			unsigned char m_uiSPassZPassOP = DS_Desc.m_uiSPassZPassOP;					//ok
			unsigned char m_uiSPassZFailOP = DS_Desc.m_uiSPassZFailOP;					//ok
			unsigned char m_uiSFailZPassOP = DS_Desc.m_uiSFailZPassOP;					//ok

			unsigned char m_uiCCW_StencilCompareMethod = DS_Desc.m_uiCCW_StencilCompareMethod;	//意义不明
			unsigned char m_uiCCW_SPassZPassOP = DS_Desc.m_uiCCW_SPassZPassOP;					//意义不明
			unsigned char m_uiCCW_SPassZFailOP = DS_Desc.m_uiCCW_SPassZFailOP;					//意义不明
			unsigned char m_uiCCW_SFailZPassOP = DS_Desc.m_uiCCW_SFailZPassOP;					//意义不明
			bool m_bTwoSideStencilMode = DS_Desc.m_bTwoSideStencilMode;							//意义不明

			bool m_bEnableCullingFace = DS_Desc.m_bEnableCullingFace;					//ok
			unsigned char m_uiCullFaceMode = DS_Desc.m_uiCullFaceMode;					//ok
			unsigned char m_uiCCW_CW = DS_Desc.m_uiCCW_CW;								//ok

			//启用颜色混合
			if (IsBlendUsed) {
				glEnable(GL_BLEND);
			}
			else {
				glDisable(GL_BLEND);
			}
			if (bAlphaToCoverageEnable) {
				glSampleCoverage(0.5,GL_FALSE);
			}

			#pragma region 设置混合参数
			GLenum S_color;
			if (ucSrcBlend == MWDBlendDesc::BP_ZERO) {
				S_color = GL_ZERO;
			}
			else if (ucSrcBlend == MWDBlendDesc::BP_ONE) {
				S_color = GL_ONE;
			}
			else if (ucSrcBlend == MWDBlendDesc::BP_SRCCOLOR) {
				S_color = GL_SRC_COLOR;
			}
			else if (ucSrcBlend == MWDBlendDesc::BP_INVSRCCOLOR) {
				S_color = GL_ONE_MINUS_SRC_COLOR;
			}
			else if (ucSrcBlend == MWDBlendDesc::BP_DESTCOLOR) {
				S_color = GL_DST_COLOR;
			}
			else if (ucSrcBlend == MWDBlendDesc::BP_INVDESTCOLOR) {
				S_color = GL_ONE_MINUS_DST_COLOR;
			}
			else {
				MWDMAC_ASSERT(0)
				return false;
			}
			GLenum D_Color ;
			if (ucDestBlend == MWDBlendDesc::BP_ZERO) {
				D_Color = GL_ZERO;
			}
			else if (ucDestBlend == MWDBlendDesc::BP_ONE) {
				D_Color = GL_ONE;
			}
			else if (ucDestBlend == MWDBlendDesc::BP_SRCCOLOR) {
				D_Color = GL_SRC_COLOR;
			}
			else if (ucDestBlend == MWDBlendDesc::BP_INVSRCCOLOR) {
				D_Color = GL_ONE_MINUS_SRC_COLOR;
			}
			else if (ucDestBlend == MWDBlendDesc::BP_DESTCOLOR) {
				D_Color = GL_DST_COLOR;
			}
			else if (ucDestBlend == MWDBlendDesc::BP_INVDESTCOLOR) {
				D_Color = GL_ONE_MINUS_DST_COLOR;
			}
			else {
				MWDMAC_ASSERT(0)
					return false;
			}
			GLenum S_Alpha,D_Alpha;
			if (ucSrcBlendAlpha == MWDBlendDesc::BP_SRCALPHA) {
				S_Alpha = GL_SRC_ALPHA;
			}
			else if (ucSrcBlendAlpha == MWDBlendDesc::BP_INVSRCALPHA) {
				S_Alpha = GL_ONE_MINUS_SRC_ALPHA;
			}

			if (ucDestBlendAlpha == MWDBlendDesc::BP_DESTALPHA) {
				D_Alpha = GL_DST_ALPHA;
			}
			else if (ucDestBlendAlpha == MWDBlendDesc::BP_INVDESTALPHA) {
				D_Alpha = GL_ONE_MINUS_DST_ALPHA;
			}
			GLenum Op;
			if (ucBlendOp == MWDBlendDesc::BO_ADD) {
				Op = GL_ADD;
			}
			else if (ucBlendOp == MWDBlendDesc::BO_SUBTRACT) {
				Op = GL_SUBTRACT;
			}
			else if (ucBlendOp == MWDBlendDesc::BO_REVSUBTRACT) {
				Op = GL_FUNC_REVERSE_SUBTRACT;
			}
			else if (ucBlendOp == MWDBlendDesc::BO_MIN_SRC_DEST) {
				Op = GL_MIN;
			}
			else if (ucBlendOp == MWDBlendDesc::BO_MAX_SRC_DEST) {
				Op = GL_MAX;
			}
			glBlendFuncSeparate(S_Alpha, D_Alpha, S_color, D_Color);
			glBlendEquation(Op);

			#pragma endregion
			
			//启用深度测试
			if (m_bDepthEnable) {
				glEnable(GL_DEPTH_TEST);
			}
			else {
				glDisable(GL_DEPTH_TEST);
			}
			
			//启用深度写入
			if (m_bDepthWritable) {
				glDepthMask(GL_TRUE);
			}
			else {
				glDepthMask(GL_FALSE);
			}

			if (m_bStencilEnable) {
				glEnable(GL_STENCIL_TEST);
			}
			else {
				glDisable(GL_STENCIL_TEST);
			}

			//启用面剔除
			if (m_bEnableCullingFace) {
				glEnable(GL_CULL_FACE);
			}
			else {
				glDisable(GL_CULL_FACE);
			}

			if (m_uiCullFaceMode == MWDDepthStencilDesc::C_BACK) {
				glCullFace(GL_BACK);
			}
			else if (m_uiCullFaceMode == MWDDepthStencilDesc::C_FRONT) {
				glCullFace(GL_FRONT);
			}
			else if (m_uiCullFaceMode == MWDDepthStencilDesc::C_FRONT_BACK) {
				glCullFace(GL_FRONT_AND_BACK);
			}
			else {
				MWDMAC_ASSERT(0)
				return false;
			}
			
			//设置深度/模板掩码
			glDepthMask(m_uiMask);
			glStencilMask(m_uiMask);

			#pragma region 设置深度比较方法
			if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_NEVER) {
				glDepthFunc(GL_NEVER);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_LESS) {
				glDepthFunc(GL_LESS);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_EQUAL) {
				glDepthFunc(GL_EQUAL);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_LESSEQUAL) {
				glDepthFunc(GL_LEQUAL);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_GREATER) {
				glDepthFunc(GL_GREATER);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_NOTEQUAL) {
				glDepthFunc(GL_NOTEQUAL);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_GREATEREQUAL) {
				glDepthFunc(GL_GEQUAL);
			}
			else if (m_uiDepthCompareMethod == MWDDepthStencilDesc::CM_ALWAYS) {
				glDepthFunc(GL_ALWAYS);
			}
			else {
				MWDMAC_ASSERT(0);
				return false;
			}
			#pragma endregion

			#pragma region 设置模板比较方法
			if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_NEVER) {
				glStencilFunc(GL_NEVER, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_LESS) {
				glStencilFunc(GL_LESS, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_EQUAL) {
				glStencilFunc(GL_EQUAL, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_LESSEQUAL) {
				glStencilFunc(GL_LEQUAL, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_GREATER) {
				glStencilFunc(GL_GREATER, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_NOTEQUAL) {
				glStencilFunc(GL_NOTEQUAL, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_GREATEREQUAL) {
				glStencilFunc(GL_GEQUAL, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else if (m_uiStencilCompareMethod == MWDDepthStencilDesc::CM_ALWAYS) {
				glStencilFunc(GL_ALWAYS, m_renderCtx.m_uiClearStencil, m_uiWriteMask);
			}
			else {
				MWDMAC_ASSERT(0);
				return false;
			}
			#pragma endregion

			#pragma region 设置模板操作方法
			GLenum SP_ZP;
			if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_KEEP) {
				SP_ZP = GL_KEEP;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_ZERO) {
				SP_ZP = GL_ZERO;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_REPLACE) {
				SP_ZP = GL_REPLACE;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_INCREMENT) {
				SP_ZP = GL_INCR;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_DECREMENT) {
				SP_ZP = GL_DECR;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_INVERT) {
				SP_ZP = GL_INVERT;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_INCREMENT_STA) {
				SP_ZP = GL_INCR_WRAP;
			}
			else if (m_uiSPassZPassOP == MWDDepthStencilDesc::OT_DECREMENT_STA) {
				SP_ZP = GL_DECR_WRAP;
			}

			GLenum SF_ZP;
			if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_KEEP) {
				SF_ZP = GL_KEEP;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_ZERO) {
				SF_ZP = GL_ZERO;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_REPLACE) {
				SF_ZP = GL_REPLACE;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_INCREMENT) {
				SF_ZP = GL_INCR;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_DECREMENT) {
				SF_ZP = GL_DECR;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_INVERT) {
				SF_ZP = GL_INVERT;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_INCREMENT_STA) {
				SF_ZP = GL_INCR_WRAP;
			}
			else if (m_uiSFailZPassOP == MWDDepthStencilDesc::OT_DECREMENT_STA) {
				SF_ZP = GL_DECR_WRAP;
			}

			GLenum SP_ZF;
			if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_KEEP) {
				SP_ZF = GL_KEEP;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_ZERO) {
				SP_ZF = GL_ZERO;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_REPLACE) {
				SP_ZF = GL_REPLACE;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_INCREMENT) {
				SP_ZF = GL_INCR;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_DECREMENT) {
				SP_ZF = GL_DECR;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_INVERT) {
				SP_ZF = GL_INVERT;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_INCREMENT_STA) {
				SP_ZF = GL_INCR_WRAP;
			}
			else if (m_uiSPassZFailOP == MWDDepthStencilDesc::OT_DECREMENT_STA) {
				SP_ZF = GL_DECR_WRAP;
			}
			glStencilOp(SF_ZP,SP_ZF,SP_ZP);
			#pragma endregion

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
			if (!fbo.IsFrameBufferCompleted()) {
				return false;
			}
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			vao.UnBind();
			fbo.unBind();
			return true;
		}

	};
}


