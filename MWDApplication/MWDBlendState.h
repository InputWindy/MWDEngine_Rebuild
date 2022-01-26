#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	//ά��һ�׻��״̬
	class  MWDBlendDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDBlendDesc)
		DECLARE_RTTI(MWDBlendDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBlendDesc)
	public:
		//��ϲ���
		enum 
		{
			BP_ZERO,
			BP_ONE,
			BP_SRCCOLOR,
			BP_INVSRCCOLOR,
			BP_SRCALPHA,
			BP_INVSRCALPHA,
			BP_DESTALPHA,
			BP_INVDESTALPHA,
			BP_DESTCOLOR,
			BP_INVDESTCOLOR,
			BP_MAX
		};
		//��ϲ���
		enum
		{
			BO_ADD,
			BO_SUBTRACT,
			BO_REVSUBTRACT,
			BO_MIN_SRC_DEST,
			BO_MAX_SRC_DEST,
			BO_MAX

		};
		//д����
		enum 
		{
			WM_NONE = 0,
			WM_ALPHA = BIT(0),
			WM_RED = BIT(1),
			WM_Green = BIT(2),
			WM_BLUE = BIT(3),
			WM_ALL = 0X0F
		};
		MWDBlendDesc()
		{
			bAlphaToCoverageEnable = false;
			bIndependentBlendEnable = false;
			bBlendEnable = false;
			ucSrcBlend = BP_ONE;
			ucDestBlend = BP_ZERO;
			ucBlendOp = BO_ADD;

			bAlphaBlendEnable = false;
			ucSrcBlendAlpha = BP_ONE;
			ucDestBlendAlpha = BP_ZERO;
			ucBlendOpAlpha = BO_ADD;
			ucWriteMask = WM_ALL;
			fBlendColor[0] = fBlendColor[1] = fBlendColor[2] = fBlendColor[3] = 0.0f;
			ucSampleMask = 0xffffffff;
		}
		~MWDBlendDesc()
		{

		}
		bool			bAlphaToCoverageEnable;
		bool			bIndependentBlendEnable;
		//��ʼ��ɫ
		MWDREAL			fBlendColor[4];

		//δʹ��
		unsigned int	ucSampleMask;

		//����Ƿ���
		bool			bBlendEnable;
		//Դ��ɫ
		unsigned char   ucSrcBlend;
		//Ŀ����ɫ
		unsigned char   ucDestBlend;
		//��ϲ���
		unsigned char	ucBlendOp;

		//�Ƿ���͸���Ȼ��
		bool			bAlphaBlendEnable;
		//Դ͸����
		unsigned char   ucSrcBlendAlpha;
		//Ŀ��͸����
		unsigned char	ucDestBlendAlpha;
		//͸���Ȼ�ϲ�����δʹ�ã�
		unsigned char	ucBlendOpAlpha;
		//д���루δʹ�ã�
		unsigned char   ucWriteMask;

		//�ж�ĳͨ���Ƿ�ʹ�ܿ���
		bool IsBlendUsed()const
		{
			return (bBlendEnable || bAlphaBlendEnable);
		}
		//MWDBlendDesc�����Ʊ�������(������ַ��bit����)
		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDBlendDesc) - sizeof(MWDObject);
			//��һ�����Եĵ�ַ
			return (void*)&bAlphaToCoverageEnable;
		}
	};
	MWDTYPE_MARCO(MWDBlendDesc);

	//ά��һ��MWDBlendDesc��8�׻��״̬��
	class  MWDBlendState : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDBlendState)
		DECLARE_RTTI(MWDBlendState, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBlendState)
	public:
		MWDBlendState() {
		};
		virtual ~MWDBlendState() {
		};
	protected:
		MWDBlendDesc m_BlendDesc;
	public:
		FORCEINLINE const MWDBlendDesc& GetBlendDesc()const
		{
			return m_BlendDesc;
		}
		friend class MWDResourceManager;
		static MWDBlendState* GetDefaultBlendState() {
			if (!m_DefaultBlendState) {
				m_DefaultBlendState = new MWDBlendState();
			}
			return m_DefaultBlendState;
		}

	protected:
		static inline MWDBlendState* m_DefaultBlendState  = NULL;
	};
	DECLARE_Ptr(MWDBlendState);
	MWDTYPE_MARCO(MWDBlendState);
}

