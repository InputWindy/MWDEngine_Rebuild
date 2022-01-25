#pragma once
#include <../MWDApplication/Graphic/Core/MWDGraphic.h>
#include <../MWDApplication/Graphic/Core/MWDObject.h>
#include <../MWDApplication/Graphic/Core/MWDProperty.h>
#include <../MWDApplication/Graphic/Core/MWDName.h>
#include <MWDCompute.h>
namespace MWDEngine
{
	//����API����Դ�ĸ���
	class MWDOpenglResource
	{
	public:
		virtual ~MWDOpenglResource() {/**/ }
	protected:
		MWDOpenglResource() {/**/ }
	};

	//����������Ⱦ��Դ�ĸ��࣬�ṩ������Ⱦ��Դ��ͨ�ýӿ�:���opengl��Դ��
	class MWDBind : public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDBind, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBind)
	public:
		enum	//Lock State
		{
			LF_NOOVERWRITE,			//��������д����
			LF_DISCARD,				//������������
			LF_READONLY,			//ֻ�ܶ�ȡ����
			LF_MAX
		};
		enum	//Mem Type
		{
			MT_BOTH,				//�����ڴ���Դ������
			MT_RAM,					//�����ڴ�����
			MT_VRAM,				//�����Դ�����
			MT_MAX
		};
		enum	// Clear State
		{
			MCS_NONE,				//��ʼδ����״̬
			MCS_READY,				//׼��״̬
			MCS_DONE,				//���״̬
			MCS_MAX
		};
		//Ĭ���Ǿ�̬��
		MWDBind(unsigned int uiSwapChainNum = 3,unsigned int uiMemType = MT_BOTH, unsigned int uiClearState = MCS_NONE, unsigned int uiLockFlag = LF_READONLY) {
			m_uiSwapChainNum = uiSwapChainNum;
			m_uiCurID = 0;
			m_uiLockFlag = uiLockFlag;
			m_uiMemType = uiMemType;
			m_uiClearState = uiClearState;
		};
		virtual ~MWDBind() {
			m_resourceArray.Destroy();
		};

	protected:
		//��ǰ�������Ⱦ��Դ�б�
		MWDArray<MWDOpenglResource*> m_resourceArray;
		//�໺������
		unsigned int m_uiSwapChainNum;        //get,set
		//��ǰ����ID
		unsigned int m_uiCurID;

		//������־
		unsigned int m_uiLockFlag;           //get,set
		//�ڴ�����
		unsigned int m_uiMemType;            //get,set
		//���״̬
		unsigned int m_uiClearState;         //get,set
	public:
		//��ȡ��ǰ(M_CurID)��opengl��Դ��LoadResource��ʹ��
		virtual MWDOpenglResource* GetResource() {
			if (!m_resourceArray.GetNum())
			{
				return NULL;
			}
			MWDOpenglResource* pID = NULL;
			pID = m_resourceArray[m_uiCurID];
			SwitchResource();
			return pID;
		};
	protected:
		//������Ĺ��캯����ʹ��!
		void AddResource(MWDOpenglResource* resource) {
			if (!resource) {
				return;
			}
			m_resourceArray.AddElement(resource);
		};
		
		//ֻ�����Դ����ݵĻ�����Ҫ�����������ڴ�����
		virtual void ClearInfo() {
			
		};        
	public:
		friend class MWDRenderer;
		virtual void LoadResource(MWDRenderer* pRenderer) {};   //��Renderer�ύopengl��Դ

		unsigned int GetOpenglResourceNum() const {
			return m_resourceArray.GetNum();
		};

		//�л�����
		void SwitchResource() {
			m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
		};
		virtual unsigned int GetByteSize()const {
			return 0;
		};
		FORCEINLINE void SetLockFlag(unsigned int uiLockFlag)
		{
			if (uiLockFlag > LF_MAX)
			{
				uiLockFlag = LF_NOOVERWRITE;
			}
			m_uiLockFlag = uiLockFlag;
		}
		FORCEINLINE unsigned int GetLockFlag()const
		{
			return m_uiLockFlag;
		}
		FORCEINLINE void SetMemType(unsigned int uiMemType)
		{
			if (uiMemType < MT_MAX)
			{
				m_uiMemType = uiMemType;
			}
		}
		FORCEINLINE unsigned int GetMemType()const
		{
			return m_uiMemType;
		}
		FORCEINLINE unsigned int GetSwapChainNum()const
		{
			return m_uiSwapChainNum;
		}
	};
	DECLARE_Ptr(MWDBind);
	MWDTYPE_MARCO(MWDBind);

}



