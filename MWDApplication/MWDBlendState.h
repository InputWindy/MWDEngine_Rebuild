#pragma once
#include "../MWDBind.h"
namespace MWDEngine {
	//维护一套混合状态
	class  MWDBlendDesc : public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDBlendDesc)
		DECLARE_RTTI(MWDBlendDesc, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBlendDesc)
	public:
		//混合参数
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
		//混合操作
		enum
		{
			BO_ADD,
			BO_SUBTRACT,
			BO_REVSUBTRACT,
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
		//初始黑色
		MWDREAL			fBlendColor[4];

		//未使用
		unsigned int	ucSampleMask;

		//混合是否开启
		bool			bBlendEnable;
		//源颜色
		unsigned char   ucSrcBlend;
		//目标颜色
		unsigned char   ucDestBlend;
		//混合操作
		unsigned char	ucBlendOp;

		//是否开启透明度混合
		bool			bAlphaBlendEnable;
		//源透明度
		unsigned char   ucSrcBlendAlpha;
		//目标透明度
		unsigned char	ucDestBlendAlpha;
		//透明度混合操作（未使用）
		unsigned char	ucBlendOpAlpha;
		//写掩码（未使用）
		unsigned char   ucWriteMask;

		//判断某通道是否使能开启
		bool IsBlendUsed()const
		{
			return (bBlendEnable || bAlphaBlendEnable);
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

	//维护一个MWDBlendDesc（8套混合状态）
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

