#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	class MWDStream;
	//最多支持8通道，每个通道是一个渲染目标
	class  MWDBlendDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDBlendDesc)
		DECLARE_RTTI(MWDBlendDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBlendDesc)
	public:
		enum
		{
			MAX_RENDER_TARGET_NUM = 8
		};
		//混合参数
		enum 
		{
			BP_ZERO,
			BP_ONE,
			BP_SRCCOLOR,
			BP_INMWDRCCOLOR,
			BP_SRCALPHA,
			BP_INMWDRCALPHA,
			BP_DESTALPHA,
			BP_INVDESTALPHA,
			BP_DESTCOLOR,
			BP_INVDESTCOLOR,
			BP_MAX
		};
		//混合操作
		enum
		{
			BO_ADD,
			BO_SUBTRACT,
			BO_REMWDUBTRACT,
			BO_MIN_SRC_DEST,
			BO_MAX_SRC_DEST,
			BO_MAX

		};
		//写掩码
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
			for (unsigned int i = 0; i < MAX_RENDER_TARGET_NUM; i++)
			{
				bBlendEnable[i] = false;
				ucSrcBlend[i] = BP_ONE;
				ucDestBlend[i] = BP_ZERO;
				ucBlendOp[i] = BO_ADD;

				bAlphaBlendEnable[i] = false;
				ucSrcBlendAlpha[i] = BP_ONE;
				ucDestBlendAlpha[i] = BP_ZERO;
				ucBlendOpAlpha[i] = BO_ADD;
				ucWriteMask[i] = WM_ALL;
			}
			fBlendColor[0] = fBlendColor[1] = fBlendColor[2] = fBlendColor[3] = 0.0f;
			ucSampleMask = 0xffffffff;
		}
		~MWDBlendDesc()
		{

		}
		bool			bAlphaToCoverageEnable;
		bool			bIndependentBlendEnable;
		//初始黑色
		MWDREAL			fBlendColor[4];
		unsigned int	ucSampleMask;

		//混合是否开启
		bool			bBlendEnable[MAX_RENDER_TARGET_NUM];
		//源颜色
		unsigned char   ucSrcBlend[MAX_RENDER_TARGET_NUM];
		//目标颜色
		unsigned char   ucDestBlend[MAX_RENDER_TARGET_NUM];
		//混合操作
		unsigned char	ucBlendOp[MAX_RENDER_TARGET_NUM];

		//是否开启透明度混合
		bool			bAlphaBlendEnable[MAX_RENDER_TARGET_NUM];
		//源透明度
		unsigned char   ucSrcBlendAlpha[MAX_RENDER_TARGET_NUM];
		//目标透明度
		unsigned char	ucDestBlendAlpha[MAX_RENDER_TARGET_NUM];
		//透明度混合操作
		unsigned char	ucBlendOpAlpha[MAX_RENDER_TARGET_NUM];
		//写掩码
		unsigned char   ucWriteMask[MAX_RENDER_TARGET_NUM];

		//判断某通道是否使能开启
		bool IsBlendUsed(unsigned int uiChannal = 0)const
		{
			return (bBlendEnable[uiChannal] || bAlphaBlendEnable[uiChannal]);
		}
		//MWDBlendDesc二进制比特数据(传出地址和bit长度)
		void* GetCRC32Data(unsigned int& DataSize)const
		{
			DataSize = sizeof(MWDBlendDesc) - sizeof(MWDObject);
			//第一个属性的地址
			return (void*)&bAlphaToCoverageEnable;
		}
	};
	MWDTYPE_MARCO(MWDBlendDesc);





	//维护一个MWDBlendDesc
	class  MWDBlendState : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDBlendState)
		DECLARE_RTTI(MWDBlendState, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBlendState)
	public:
		MWDBlendState() {
			m_bIsStatic = 1;
		};
		virtual ~MWDBlendState() {
			ReleaseResource();
		};
	protected:
		MWDBlendDesc m_BlendDesc;
	public:
		static bool InitialDefaultState() { return true; };
		static bool TerminalDefaultState() { return true; };
	public:
		FORCEINLINE const MWDBlendDesc& GetBlendDesc()const
		{
			return m_BlendDesc;
		}
		friend class MWDResourceManager;
		static MWDBlendState* GetDefaultBlendState() {
			if (m_DefaultBlendState.IsNULL()) {
				*m_DefaultBlendState = *new MWDBlendState();
			}
			return m_DefaultBlendState;
		}

	protected:
		static inline MWDPointer<MWDBlendState> m_DefaultBlendState ;

		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);
	};
	DECLARE_Ptr(MWDBlendState);
	MWDTYPE_MARCO(MWDBlendState);
}

