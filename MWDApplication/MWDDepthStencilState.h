#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	//ά����ȣ�ģ�����״̬
	class  MWDDepthStencilDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDDepthStencilDesc)
		DECLARE_RTTI(MWDDepthStencilDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDepthStencilDesc)
	public:
		//�ȽϷ���(depth,stencil)
		enum 
		{
			CM_NEVER,
			CM_LESS,
			CM_EQUAL,
			CM_LESSEQUAL,
			CM_GREATER,
			CM_NOTEQUAL,
			CM_GREATEREQUAL,
			CM_ALWAYS,
			CM_MAX

		};
		//��������(stencil)
		enum 
		{
			OT_KEEP,
			OT_ZERO,
			OT_REPLACE,
			OT_INCREMENT,
			OT_DECREMENT,
			OT_INVERT,
			OT_INCREMENT_STA,
			OT_DECREMENT_STA,
			OT_MAX
		};
		enum {
			CULL_CW,
			CULL_CCW
		};
		enum {
			C_FRONT_BACK,
			C_FRONT,
			C_BACK
		};
		//Ĭ�Ͽ�����Ȳ��ԣ����д�롣Ĭ�Ϲر�ģ�����
		MWDDepthStencilDesc()
		{
			m_bDepthEnable = true;
			m_bDepthWritable = true;
			m_uiDepthCompareMethod = CM_LESSEQUAL;

			m_bStencilEnable = false;
			m_uiStencilCompareMethod = CM_ALWAYS;
			m_uiReference = 0x0;
			m_uiMask = 0xFF;
			m_uiWriteMask = 0xFF;
			m_uiSPassZPassOP = OT_KEEP;
			m_uiSPassZFailOP = OT_KEEP;
			m_uiSFailZPassOP = OT_KEEP;

			m_uiCCW_StencilCompareMethod = CM_ALWAYS;
			m_uiCCW_SPassZPassOP = OT_KEEP;
			m_uiCCW_SPassZFailOP = OT_KEEP;
			m_uiCCW_SFailZPassOP = OT_KEEP;
			m_bTwoSideStencilMode = false;

			m_bEnableCullingFace = false;
			m_uiCullFaceMode = C_BACK;
			m_uiCCW_CW = CULL_CCW;
		}
		~MWDDepthStencilDesc()
		{

		}

		//��Ȳ���ʹ��
		bool m_bDepthEnable;
		bool m_bDepthWritable;

		//��Ȳ��Է���
		unsigned char m_uiDepthCompareMethod;

		//ģ�����ʹ��
		bool m_bStencilEnable;
		unsigned char m_uiStencilCompareMethod;
		unsigned char m_uiReference;
		unsigned char m_uiMask;
		unsigned char m_uiWriteMask;
		unsigned char m_uiSPassZPassOP;
		unsigned char m_uiSPassZFailOP;
		unsigned char m_uiSFailZPassOP;

		unsigned char m_uiCCW_StencilCompareMethod;
		unsigned char m_uiCCW_SPassZPassOP;
		unsigned char m_uiCCW_SPassZFailOP;
		unsigned char m_uiCCW_SFailZPassOP;
		bool m_bTwoSideStencilMode;

		//���޳�
		bool m_bEnableCullingFace;
		unsigned char m_uiCullFaceMode;
		unsigned char m_uiCCW_CW;

		//��ȡ��ȣ�ģ�����״̬����
		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDDepthStencilDesc) - sizeof(MWDObject);
			return (void*)&m_bDepthEnable;
		}
	};
	MWDTYPE_MARCO(MWDDepthStencilDesc);





	//ά��һ��MWDDepthStencilDesc
	class  MWDDepthStencilState : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDDepthStencilState)
		DECLARE_RTTI(MWDDepthStencilState, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDepthStencilState)
	public:
		MWDDepthStencilState() {
		};
		virtual ~MWDDepthStencilState() {
		};
	protected:
		MWDDepthStencilDesc m_DepthStencilDesc;
	public:
		FORCEINLINE const MWDDepthStencilDesc& GetDepthStencilDesc()const
		{
			return m_DepthStencilDesc;
		}
		friend class MWDResourceManager;
		static MWDDepthStencilState* GetDefaultDepthStencilState() {
			if (!m_DefaultDepthStencilState) {
				m_DefaultDepthStencilState = new MWDDepthStencilState();
			}
			return m_DefaultDepthStencilState;
		}
	protected:
		static inline MWDDepthStencilState* m_DefaultDepthStencilState = NULL ;
	};
	DECLARE_Ptr(MWDDepthStencilState);
	MWDTYPE_MARCO(MWDDepthStencilState);
}

