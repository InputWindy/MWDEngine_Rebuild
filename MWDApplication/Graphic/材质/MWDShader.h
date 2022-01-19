#pragma once
#include "../MWDApplication/Graphic/Core/MWDResource.h"
#include "MWDShaderConstant.h"
#include "MWDShaderSampler.h"
namespace MWDEngine {
	class MWDStream;
	//ά��shader������shader���ƣ�shader����(������ĺ�û�������)
	class  MWDShader : public MWDBind, public MWDResource
	{
		DECLARE_RTTI_NoCreateFun(MWDShader,MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDShader)
	public:
		MWDShader(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDBind(MWDBind::MT_VRAM) {
			m_pShaderConstant.Clear();
			MWDMAC_ASSERT(pBuffer);
			if (IsFromFile)
			{
				m_ResourceName = pBuffer;
			}
			else
			{
				m_Buffer = pBuffer;
			}
			m_pCacheBuffer = NULL;
			m_uiCacheBufferSize = 0;
			m_MainFunName = MainFunName;
			m_bCreatePara = false;
			m_uiArithmeticInstructionSlots = 0;
			m_uiTextureInstructionSlots = 0;
			m_uiConstBufferSize = 0;
		};
		MWDShader(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDBind(MWDBind::MT_VRAM)
		{
			m_pShaderConstant.Clear();
			if (IsFromFile)
			{
				m_ResourceName = Buffer;
			}
			else
			{
				m_Buffer = Buffer;
			}
			m_pCacheBuffer = NULL;
			m_uiCacheBufferSize = 0;
			m_MainFunName = MainFunName;
			m_bCreatePara = false;
			m_uiArithmeticInstructionSlots = 0;
			m_uiTextureInstructionSlots = 0;

		};
		MWDShader():MWDBind(MWDBind::MT_VRAM)
		{
			m_pCacheBuffer = NULL;
			m_uiCacheBufferSize = 0;
			m_bCreatePara = false;
			m_uiArithmeticInstructionSlots = 0;
			m_uiTextureInstructionSlots = 0;
			m_uiConstBufferSize = 0;
		};
		virtual ~MWDShader() {
			MWDMAC_DELETEA(m_pCacheBuffer);
			m_uiCacheBufferSize = 0;
			ReleaseResource();
		};
		virtual unsigned int GetResourceType()const
		{
			return RT_SHADER;
		}
	public:

		virtual bool SetParam(const MWDName& Name, void* pData);
		virtual bool SetParam(const MWDName& Name, MWDTexture2D* pTexture, unsigned int uiIndex = 0);
		virtual void ClearInfo() {
			if (m_uiSwapChainNum == m_resourceArray.GetNum())
			{
				if (m_uiMemType == MT_VRAM)
				{
					m_Buffer.Clear();
				}
			}
		};

		const MWDString& GetBuffer()const { return m_Buffer; }
		FORCEINLINE const void* GetCacheBuffer()const
		{
			return m_pCacheBuffer;
		}
		FORCEINLINE unsigned int GetCacheBufferSize()const
		{
			return m_uiCacheBufferSize;
		}
		//������������ɫ��
		bool SetCacheBuffer(void* pBuffer, unsigned int uiSize);
		const MWDString& GetMainFunName()const
		{
			return m_MainFunName;
		}
		unsigned int m_uiConstBufferSize;
	protected:

		//��ŵ���û���������ɫ������(�ַ���)
		MWDString m_Buffer;
		//��ŵ��Ǳ��������ɫ�����루�ֽ��룩
		unsigned char* m_pCacheBuffer;
		//�ֽ��볤��
		unsigned int m_uiCacheBufferSize;
		//����������
		MWDString	m_MainFunName;

	public:
		void SetShaderString(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile = false);
		void SetShaderString(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile = false);
	public:
		MWDArray<MWDShaderConstantPtr>		m_pShaderConstant;
		MWDArray<MWDShaderSamplerPtr>		m_pShaderSampler;
		//MWDShaderKey						m_ShaderKey;
		bool		m_bCreatePara;
		unsigned int m_uiArithmeticInstructionSlots;
		unsigned int m_uiTextureInstructionSlots;
	};
	DECLARE_Ptr(MWDShader);
	MWDTYPE_MARCO(MWDShader);


	class  MWDVertexShader : public MWDShader
	{
		DECLARE_CLASS_FUNCTION(MWDVertexShader)
		DECLARE_RTTI(MWDVertexShader, MWDShader)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDVertexShader)
	public:
		MWDVertexShader() {};
		MWDVertexShader(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(pBuffer, MainFunName, IsFromFile) {};
		MWDVertexShader(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(Buffer, MainFunName, IsFromFile) {};
		virtual ~MWDVertexShader() {};

	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(MWDOpenglResource*& pID) ;
		virtual bool OnReleaseResource(MWDOpenglResource* pID) ;
		
	};
	DECLARE_Ptr(MWDVertexShader);
	MWDTYPE_MARCO(MWDVertexShader);


	class  MWDGeometryShader : public MWDShader
	{
		DECLARE_CLASS_FUNCTION(MWDGeometryShader)
		DECLARE_RTTI(MWDGeometryShader, MWDShader)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDGeometryShader)
	public:
		MWDGeometryShader() {};
		MWDGeometryShader(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(pBuffer, MainFunName, IsFromFile) {};
		MWDGeometryShader(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(Buffer, MainFunName, IsFromFile) {};
		virtual ~MWDGeometryShader() {};

	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);

	};
	DECLARE_Ptr(MWDGeometryShader);
	MWDTYPE_MARCO(MWDGeometryShader);

	class  MWDFragmentShader : public MWDShader
	{
		DECLARE_CLASS_FUNCTION(MWDFragmentShader)
		DECLARE_RTTI(MWDFragmentShader, MWDShader)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDFragmentShader)
	public:
		MWDFragmentShader() {};
		MWDFragmentShader(const TCHAR* pBuffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(pBuffer, MainFunName, IsFromFile) {};
		MWDFragmentShader(const MWDString& Buffer, const MWDString& MainFunName, bool IsFromFile = false) :MWDShader(Buffer, MainFunName, IsFromFile) {};
		virtual ~MWDFragmentShader() {};

	public:
		static bool ms_bIsEnableASYNLoader;
		static bool ms_bIsEnableGC;
	protected:
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		virtual bool OnReleaseResource(MWDOpenglResource* pID);

	};
	DECLARE_Ptr(MWDFragmentShader);
	MWDTYPE_MARCO(MWDFragmentShader);
}

