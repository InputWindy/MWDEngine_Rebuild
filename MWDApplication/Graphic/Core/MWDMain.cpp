#include "MWDMain.h"
namespace MWDEngine {

	void MWDMain::AddInitialPropertyFuntion(FunctionProperty Func)
	{
		if (!Func)
			return;
		if (!ms_pInitialPropertyArray)
		{
			ms_pInitialPropertyArray = new MWDArray<FunctionProperty>;
		}
		ms_pInitialPropertyArray->AddElement(Func);
		//cout << "ms_pInitialPropertyArray: " << ms_pInitialPropertyArray->GetNum() << endl;
	}
	void MWDMain::AddTerminalPropertyFuntion(Function Func)
	{
		if (!Func)
			return;
		if (!ms_pTerminalPropertyArray)
		{
			ms_pTerminalPropertyArray = new MWDArray<Function>;
		}
		ms_pTerminalPropertyArray->AddElement(Func);
		//cout << "ms_pTerminalPropertyArray: "<<ms_pTerminalPropertyArray->GetNum() << endl;
	}

	void MWDMain::AddInitialFuntion(Function Func, MWDPriority* pPriority)
	{
		
		/*cout << "MWDMain::AddInitialFuntion" << endl;
		if (!pPriority) {
			cout << pPriority->GetPriorityNum() << endl;
		}*/
		if (!Func)
			return;
		if (!ms_pInitialArray)
		{
			ms_pInitialArray = new MWDArray<Element>();
		}
		Element e;
		e.Func = Func;
		e.pPriority = pPriority;
		ms_pInitialArray->AddElement(e);
		
		//cout << "ms_pInitialArray: " << ms_pInitialArray->GetNum() << endl;
	}
	void MWDMain::AddTerminalFuntion(Function Func, MWDPriority* pPriority)
	{
		if (!Func)
			return;
		if (!ms_pTerminalArray)
		{
			ms_pTerminalArray = new MWDArray<Element>;
		}
		Element e;
		e.Func = Func;
		e.pPriority = pPriority;
		ms_pTerminalArray->AddElement(e);
		//cout << "ms_pTerminalArray: "<<ms_pTerminalArray->GetNum() << endl;
	}

	bool MWDMain::Initialize()
	{
		if (ms_pInitialPropertyArray) {
			//�Ȱ�û��������obj��ʼ��
			for (unsigned int i = 0; i < ms_pInitialPropertyArray->GetNum(); i++)
			{
				
				//���������Rtti
				//ms_pInitialPropertyArray->GetBuffer()[i](NULL)
				if (!(*((*ms_pInitialPropertyArray)[i]))(NULL))
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			ms_pInitialPropertyArray->Clear();
		}
		if (ms_pInitialArray) {
			//����֮����ִ����������obj��ʼ��
			//cout << "num=" << ms_pInitialArray->GetNum() << endl;
			ms_pInitialArray->Sort(0, ms_pInitialArray->GetNum() - 1, PriorityCompare());
			for (unsigned int i = 0; i < ms_pInitialArray->GetNum(); i++)
			{
				//cout <<"������ַ��"<< (*ms_pInitialArray)[i].Func << endl;
				if (!(*((*ms_pInitialArray)[i].Func))())
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			//ִ�����֮���Ҫ������ע��ĺ�������
			ms_pInitialArray->Clear();
		}
		MWDMAC_DELETE(ms_pInitialArray);
		ms_uiInitialObject = MWDObject::GetObjectManager().GetObjectNum();
		
		MWDMAC_DELETE(ms_pInitialPropertyArray);
		return 1;
	}
	bool MWDMain::Terminate()
	{
		
		//obj��������������
		ms_uiTerminalObject = MWDObject::GetObjectManager().GetObjectNum();
		//������Դ����������Դ�����
		//MWDResourceManager::CacheResource();
		if (ms_pTerminalArray) {
			//����Ҫ���ٵ�obj��������
			ms_pTerminalArray->Sort(0, ms_pTerminalArray->GetNum(), PriorityCompare());
			//��������������������������obj
			for (int i = ms_pTerminalArray->GetNum() - 1; i >= 0; i--)
			{
				if (!(*((*ms_pTerminalArray)[i].Func))())
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			//����б�
			ms_pTerminalArray->Clear();
			MWDMAC_DELETE(ms_pTerminalArray);
		}
		if (ms_pTerminalPropertyArray) {
			//��û������obj����
			for (unsigned int i = 0; i < ms_pTerminalPropertyArray->GetNum(); i++)
			{

				if (!(*((*ms_pTerminalPropertyArray)[i]))())
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			ms_pTerminalPropertyArray->Clear();
			MWDMAC_DELETE(ms_pTerminalPropertyArray);
		}
		
		
		//�����������գ���������������������
		/*MWDResourceManager::GCObject();
		MWDResourceManager::RunAllGCTask();
		MWDMAC_ASSERT(MWDResourceManager::IsReleaseAll());*/

		//obj��������������
		ms_uiTerminalObject = MWDObject::GetObjectManager().GetObjectNum();
		cout << "========================" << endl;
		cout << "||obj������������" << ms_uiTerminalObject <<"||" << endl;
		cout << "========================" << endl;
		//cout << "ִ�е���MWDMain::Terminate()" << endl;
		//�ж��Ƿ���ճɹ� (MWDResourceManagerû��ʵ�֣����Բ��������)
		//MWDMAC_ASSERT(ms_uiTerminalObject == 0);
		return 1;
	}
}
