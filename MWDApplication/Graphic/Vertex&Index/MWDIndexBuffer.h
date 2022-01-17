#pragma once
#include "../MWDMath/MWDMath.h"
#include "../MWDBind.h"
#include "MWDDataBuffer.h"
namespace MWDEngine {
	class  MWDIndexBuffer : public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDIndexBuffer)
		DECLARE_RTTI(MWDIndexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDIndexBuffer)
	public:
		virtual ~MWDIndexBuffer() {
			ReleaseResource();
			m_pData = NULL;
			m_pLockData = NULL;
		};
		MWDIndexBuffer() {
			m_pData = NULL;
			m_uiNum = 0;
			m_uiDataType = MWDDataBuffer::DataType_USHORT;
			m_pLockData = NULL;
		};
		MWDIndexBuffer(unsigned int uiNum, unsigned int uiDataType = MWDDataBuffer::DataType_USHORT) {
			MWDMAC_ASSERT(uiNum);
			MWDMAC_ASSERT(uiDataType == MWDDataBuffer::DataType_USHORT || uiDataType == MWDDataBuffer::DataType_UINT)
				m_pData = NULL;
			m_uiNum = uiNum;
			m_uiDataType = uiDataType;
			m_pLockData = NULL;
		};;
		bool SetData(MWDDataBuffer* pData) {
			if ((pData->GetDataType() != MWDDataBuffer::DataType_USHORT && pData->GetDataType() != MWDDataBuffer::DataType_UINT)
				|| !pData->GetNum() || !pData->GetData())
			{
				return 0;
			}
			m_pData = pData;
			m_uiNum = pData->GetNum();
			m_uiDataType = pData->GetDataType();
			return 1;
		};;
	public:


		virtual void* Lock();
		virtual void UnLock();

		virtual	bool LoadResource(MWDRenderer* pRender) {
			if (!MWDBind::LoadResource(pRender))
			{
				return 0;
			}

			return 1;
		};;
		virtual void ClearInfo() {
			if (m_uiSwapChainNum == m_resourceArray.GetNum())
			{
				if (m_uiMemType == MT_VRAM)
				{
					m_pData = NULL;
				}
			}
		};;
		FORCEINLINE MWDDataBuffer* GetIndexData()const { return m_pData; }

		unsigned int GetNum()const {
			return m_uiNum;
		};;
		virtual unsigned int GetByteSize()const {
			return MWDDataBuffer::ms_uiDataTypeByte[m_uiDataType] * m_uiNum;
		};;
		FORCEINLINE void* GetLockDataPtr()const
		{
			return m_pLockData;
		}
		FORCEINLINE unsigned int GetDataType()const
		{
			return m_uiDataType;
		}
	protected:
		//MWDRenderer未实现
		virtual bool OnLoadResource(MWDOpenglResource*& pID);
		//MWDRenderer未实现
		virtual bool OnReleaseResource(MWDOpenglResource* pID);
		MWDDataBufferPtr m_pData;
		unsigned int m_uiNum;
		unsigned int m_uiDataType;
		void* m_pLockData;
	};
	DECLARE_Ptr(MWDIndexBuffer);
	MWDTYPE_MARCO(MWDIndexBuffer);
}

