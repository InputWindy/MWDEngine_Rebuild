#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	class MWDStream;
	//ά����ȣ�ģ�����״̬
	class  MWDDepthStencilDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDDepthStencilDesc)
		DECLARE_RTTI(MWDDepthStencilDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDepthStencilDesc)
	public:
		//�ȽϷ���
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
		//��������
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

		//��ȡ��ȣ�ģ�����״̬����
		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDDepthStencilDesc) - sizeof(MWDObject);
			return (void*)&m_bDepthEnable;
		}
	};
	MWDTYPE_MARCO(MWDDepthStencilDesc);





	//ά��һ��MWDDepthStencilDesc
	class  MWDDepthStencilState : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDDepthStencilState)
		DECLARE_RTTI(MWDDepthStencilState, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDepthStencilState)
	public:
		MWDDepthStencilState() {
			m_bIsStatic = 1;
		};
		virtual ~MWDDepthStencilState() {
			ReleaseResource();
		};
	protected:
		MWDDepthStencilDesc m_DepthStencilDesc;
		static bool InitialDefaultState() { return true; };
		static bool TerminalDefaultState() { return true; };
	public:
		FORCEINLINE const MWDDepthStencilDesc& GetDepthStencilDesc()const
		{
			return m_DepthStencilDesc;
		}
		friend class MWDResourceManager;
		static MWDDepthStencilState* GetDefaultDepthStencilState() {
			if (m_DefaultDepthStencilState.IsNULL()) {
				*m_DefaultDepthStencilState = *new MWDDepthStencilState();
			}
			return m_DefaultDepthStencilState;
		}
	protected:
		static inline MWDPointer<MWDDepthStencilState> m_DefaultDepthStencilState ;
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);
	};
	DECLARE_Ptr(MWDDepthStencilState);
	MWDTYPE_MARCO(MWDDepthStencilState);
}

