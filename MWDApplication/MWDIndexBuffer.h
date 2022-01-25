#pragma once
#include "MWDBind.h"
#include "MWDDataBuffer.h"
#include "MWDIBO.h"
namespace MWDEngine {

	//首先创建MWDIndexBuffer，然后传入数据。
	class MWDIndexBuffer:public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDIndexBuffer)
		DECLARE_RTTI(MWDIndexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDIndexBuffer)

	protected:
		friend class MWDRenderer;
		MWDDataBuffer* m_pData;         //内存数据(一份)
	public:
		MWDIndexBuffer(unsigned int usage = GL_STATIC_DRAW) {
			int num = GetSwapChainNum();
			for (int i = 0; i < num; ++i) {
				MWDIBO* ibo = new MWDIBO(usage);
				AddResource(ibo);
			}
		}
		~MWDIndexBuffer() {
			MWDMAC_DELETE(m_pData)
		}
		//获取顶点数
		unsigned int GetNum()const {
			return m_pData->GetNum();
		};
		//获取Buffer总Byte长度
		unsigned int GetByteSize()const {
			return m_pData->GetSize();
		};
		//获取数据类型（int,double.....）
		unsigned int GetDataType()const {
			return m_pData->GetDataType();
		};
		//读取数据
		MWDDataBuffer* GetData()const {
			return m_pData;
		};
		//设置数据，并提交给显存
		bool SetData(MWDDataBuffer* pData) {
			if (!pData) {
				return false;
			}
			if ((pData->GetDataType() != MWDDataBuffer::DataType_USHORT && pData->GetDataType() != MWDDataBuffer::DataType_UINT)
				|| !pData->GetNum() || !pData->GetData())
			{
				return false;
			}
			m_pData =pData ;
			LoadDataToIBO();
		};
		//帮当前实例的数据提交给渲染器
		void LoadResource(MWDRenderer* pRenderer) {
			
		};
	protected:
		//将内存数据提交到显存，看设置调用ClearInfo
		void LoadDataToIBO() {
			((MWDIBO*)GetResource())->SetData(m_pData->GetData(), m_pData->GetSize());
			SwitchResource();
			return;
		};
		void ClearInfo() {

		};

	};
	DECLARE_Ptr(MWDIndexBuffer);
	MWDTYPE_MARCO(MWDIndexBuffer);
}


