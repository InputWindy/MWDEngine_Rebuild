#pragma once
#include "MWDBind.h"
#include "MWDDataBuffer.h"
#include "MWDIBO.h"
namespace MWDEngine {

	//���ȴ���MWDIndexBuffer��Ȼ�������ݡ�
	class MWDIndexBuffer:public MWDBind
	{
		DECLARE_CLASS_FUNCTION(MWDIndexBuffer)
		DECLARE_RTTI(MWDIndexBuffer, MWDBind)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDIndexBuffer)

	protected:
		friend class MWDRenderer;
		friend class MWDModel;
		MWDDataBuffer* m_pData;         //�ڴ�����(һ��)
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
		//��ȡ������
		unsigned int GetNum()const {
			MWDMAC_ASSERT(m_pData)
			return m_pData->GetNum();
		};
		//��ȡBuffer��Byte����
		unsigned int GetByteSize()const {
			return m_pData->GetSize();
		};
		//��ȡ�������ͣ�int,double.....��
		unsigned int GetDataType()const {
			return m_pData->GetDataType();
		};
		//��ȡ����
		MWDDataBuffer* GetData()const {
			return m_pData;
		};
		//�������ݣ����ύ���Դ�
		bool SetData(MWDDataBuffer* pData,bool load = false) {
			if (!pData) {
				return false;
			}
			if ((pData->GetDataType() != MWDDataBuffer::DataType_USHORT && pData->GetDataType() != MWDDataBuffer::DataType_UINT)
				|| !pData->GetNum() || !pData->GetData())
			{
				return false;
			}
			if (load) {
				m_pData =pData ;
				LoadDataToIBO();
			}
			return true;
			
		};
	protected:
		//���ڴ������ύ���Դ棬�����õ���ClearInfo
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


