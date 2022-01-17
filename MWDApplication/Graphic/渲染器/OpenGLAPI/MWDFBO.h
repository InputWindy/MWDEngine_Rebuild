#pragma once
#include "OpenglAPIInclude.h"
#include "MWDOpenglTexture.h"
#include "MWDRBO.h"
namespace MWDEngine {
	// 帧缓冲区:
	// 一个完整的FBO需要满足以下条件:
	//		至少附加一个缓冲区(颜色、深度或者模板)
	//		至少有一个颜色附加
	//		所有的附加必须完整(预分配了内存)
	//		每个缓冲区的采样数需要一致
	//如果FBO不完整将不能正常工作。
	//一个FBO可以绑定很多的RBO和texture
	//FBO就是一帧画面，创建窗口时默认存在一个FBO
	//自己创建的FBO就是离屏渲染
	class  MWDFBO :public MWDOpenglResource
	{
	public :
		enum Target{
				FrameBuffer = GL_FRAMEBUFFER,
				DrawFrameBuffer = GL_DRAW_FRAMEBUFFER,
				ReadFrameBuffer = GL_READ_FRAMEBUFFER,
				T_MAX
		};
		enum Attachment {
			Color0 = GL_COLOR_ATTACHMENT0,
			Color1 = GL_COLOR_ATTACHMENT1,
			Color2 = GL_COLOR_ATTACHMENT2,
			Color3 = GL_COLOR_ATTACHMENT3,
			Color4 = GL_COLOR_ATTACHMENT4,
			Color5 = GL_COLOR_ATTACHMENT5,
			Color6 = GL_COLOR_ATTACHMENT6,
			Color7 = GL_COLOR_ATTACHMENT7,
			Color8 = GL_COLOR_ATTACHMENT8,
			Color9 = GL_COLOR_ATTACHMENT9,
			Color10 = GL_COLOR_ATTACHMENT10,
			Color11 = GL_COLOR_ATTACHMENT11,
			Color12 = GL_COLOR_ATTACHMENT12,
			Color13 = GL_COLOR_ATTACHMENT13,
			Color14 = GL_COLOR_ATTACHMENT14,
			Color15 = GL_COLOR_ATTACHMENT15,
			Color16 = GL_COLOR_ATTACHMENT16,
			Color17 = GL_COLOR_ATTACHMENT17,
			Color18 = GL_COLOR_ATTACHMENT18,
			Color19 = GL_COLOR_ATTACHMENT19,
			Color20 = GL_COLOR_ATTACHMENT20,
			Color21 = GL_COLOR_ATTACHMENT21,
			Color22 = GL_COLOR_ATTACHMENT22,
			Color23 = GL_COLOR_ATTACHMENT23,
			Color24 = GL_COLOR_ATTACHMENT24,
			Depth = GL_DEPTH_ATTACHMENT,
			Stencil = GL_STENCIL_ATTACHMENT,
			Depth_Stencil =GL_DEPTH_STENCIL_ATTACHMENT ,
			A_MAX
		};
	protected:
		bool m_GenByOuterTexture;
		bool m_GenByOuterRbo;
		Target m_target;
		Attachment m_attachment;
		unsigned int m_uiID;
		bool m_bIsBind;
		unsigned int m_width;
		unsigned int m_height;
		
		//维护第一个buffer
		MWDOpenglTexture2D* m_texture;
		MWDRBO* m_rbo;
		MWDMap<MWDOpenglTexture2D*, Attachment> m_textureMap;
		MWDMap<MWDRBO*, Attachment> m_rboMap;
	public:
		//第一个参数设定
		MWDFBO( Attachment attach,MWDOpenglTexture2D* texture = NULL,unsigned int width=600,unsigned int height=800,Target targ = FrameBuffer) {
			m_target = targ;
			m_attachment = attach;
			m_texture = texture;
			m_rbo =NULL ;
			if (texture == NULL) {
				m_GenByOuterTexture = false;
				m_GenByOuterRbo = false;
				m_texture = new MWDOpenglTexture2D(0,width,height);
				m_width = width;
				m_height = height;
			}
			else {
				m_GenByOuterTexture = true;
				m_GenByOuterRbo = false;
				m_width = texture->GetWidth();
				m_height = texture->GetHeight();
			}

			glGenFramebuffers(1, &m_uiID);
			glFramebufferTexture2D(m_target, m_attachment, GL_TEXTURE_2D, m_texture->GetHandle(),0);
			m_bIsBind = false;
		}
		MWDFBO(Attachment attach, MWDRBO* rbo = NULL,  unsigned int width = 600, unsigned int height = 800,Target targ = FrameBuffer) {
			m_texture = NULL;
			m_rbo = rbo;
			m_target = targ;
			m_attachment = attach;
			if (rbo == NULL) {
				m_GenByOuterRbo = false;
				m_GenByOuterTexture = false;
				m_rbo = new MWDRBO();
				m_width = width;
				m_height = height;
			}
			else {
				m_GenByOuterRbo = true;
				m_GenByOuterTexture = false;
				m_width = rbo->GetWidth();
				m_height = rbo->GetHeight();
			}
			glGenFramebuffers(1, &m_uiID);
			glFramebufferTexture2D(m_target, m_attachment, GL_TEXTURE_2D, m_rbo->GetHandle(), 0);
			m_bIsBind = false;
		}
		~MWDFBO() {
			m_width = 0;
			m_height = 0;
			m_target = Target::T_MAX;
			m_attachment = Attachment::A_MAX;
			bind();
			glDeleteFramebuffers(1, &m_uiID);
			unBind();
			if (!m_GenByOuterTexture) {
				MWDMAC_DELETE(m_texture)
			}
			if (!m_GenByOuterRbo) {
				MWDMAC_DELETE(m_rbo)
			}
			m_textureMap.Clear();
			m_rboMap.Clear();
			m_uiID = -1;
		}
		bool IsBind()const {
			return m_bIsBind;
		}
		//bind绑定当前FrameBuffer为渲染目标，则Draw出来的结果会保存在当前FrameBuffer
		bool bind() {
			if (m_uiID == -1) {
				return false;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, m_uiID);
			return true;
		}
		bool unBind() {
			if (m_uiID == -1) {
				return false;
			}
			if (!IsBind()) {
				return true;
			}
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			return true;
		}

		unsigned int GetHandle()const {
			return m_uiID;
		}
		MWDOpenglTexture2D* GetTexture2D() {
			return m_texture;
		}
		unsigned int GetBindTextureNum()const {
			unsigned int cnt = 0;
			if (m_texture) {
				++cnt;
			}
			cnt += m_textureMap.GetNum();
			return cnt;
		}
		unsigned int GetBindRboNum()const {
			unsigned int cnt = 0;
			if (m_rbo) {
				++cnt;
			}
			cnt += m_rboMap.GetNum();
			return cnt;
		}
		bool IsFrameBufferCompleted() {
			bool flag = false;
			flag = (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
			if (!flag) {
				cout << "FBO不完整！" << endl;
			}
			else
			{
				cout << "FBO完整！" << endl;
				
			}
			return flag;
		}
		void BindTexture(Attachment attach, MWDOpenglTexture2D* texture ) {
			MWDMapElement<MWDOpenglTexture2D*, Attachment> tmp = MWDMapElement<MWDOpenglTexture2D*, Attachment>(texture,attach);
			if (m_textureMap.Find(texture)!=m_textureMap.GetNum()) {
				return;
			}
			m_textureMap.AddElement(tmp);
			bind();
			glFramebufferTexture2D(m_target, attach, GL_TEXTURE_2D, texture->GetHandle(), 0);
			unBind();
		}
		void BindRBO(Attachment attach, MWDRBO* rbo) {
			MWDMapElement<MWDRBO*, Attachment> tmp = MWDMapElement<MWDRBO*, Attachment>(rbo, attach);
			if (m_rboMap.Find(rbo) != m_rboMap.GetNum()) {
				return;
			}
			m_rboMap.AddElement(tmp);
			bind();
			glFramebufferRenderbuffer(m_target,attach, GL_RENDERBUFFER, rbo->GetHandle());
			unBind();
		}
		void UnBindTexture(MWDOpenglTexture2D* texture) {
			int idx = m_textureMap.Find(texture);
			if (idx!= m_textureMap.GetNum()) {
				bind();
				glFramebufferTexture2D(m_target, m_textureMap[idx].Value, GL_TEXTURE_2D, 0, 0);
				unBind();
				m_textureMap.Erase(idx,idx);
			}
			return;
			
		};
		void UnBindRBO(MWDRBO* rbo) {
			int idx = m_rboMap.Find(rbo);
			if (idx != m_rboMap.GetNum()) {
				bind();
				glFramebufferTexture2D(m_target, m_rboMap[idx].Value, GL_TEXTURE_2D, 0, 0);
				unBind();
				m_rboMap.Erase(idx, idx);
			}
			return;
		}
	};
	
}


