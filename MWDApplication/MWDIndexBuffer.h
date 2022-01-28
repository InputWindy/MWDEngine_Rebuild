#pragma once
#include "MWDBind.h"
#include "MWDDataBuffer.h"
#include "MWDIBO.h"
namespace MWDEngine {

	//ʹ�÷�����ʵ��������DataBuffer��Ȼ�����DataBuffer
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
		MWDIndexBuffer(unsigned int usage = GL_STATIC_DRAW):MWDBind(1) {
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
		//��ȡ�������ͣ�DataBuffer���ͣ�int,double.....��
		unsigned int GetDataType()const {
			return m_pData->GetDataType();
		};
		//��ȡ����
		MWDDataBuffer* GetData()const {
			return m_pData;
		};
		//�������ݣ����ύ���Դ�
		bool SetData(MWDDataBuffer* pData,bool load = false) {
			MWDMAC_ASSERT(pData)
			if (!pData) {
				return false;
			}
			if ((pData->GetDataType() != MWDDataBuffer::DataType_USHORT && pData->GetDataType() != MWDDataBuffer::DataType_UINT)
				|| !pData->GetNum() || !pData->GetData())
			{
				//cout << "ִ�е��⣡" << endl;
				return false;
			}
			m_pData =pData ;
			//cout << m_pData << endl;
			if (load) {
				
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
			MWDMAC_DELETE(m_pData)
		};

	};
	DECLARE_Ptr(MWDIndexBuffer);
	MWDTYPE_MARCO(MWDIndexBuffer);
}


