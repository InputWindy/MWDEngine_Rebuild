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
			//先把没有依赖的obj初始化
			for (unsigned int i = 0; i < ms_pInitialPropertyArray->GetNum(); i++)
			{
				
				//传入参数是Rtti
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
			//排序之后按序执行有依赖的obj初始化
			//cout << "num=" << ms_pInitialArray->GetNum() << endl;
			ms_pInitialArray->Sort(0, ms_pInitialArray->GetNum() - 1, PriorityCompare());
			for (unsigned int i = 0; i < ms_pInitialArray->GetNum(); i++)
			{
				//cout <<"函数地址："<< (*ms_pInitialArray)[i].Func << endl;
				if (!(*((*ms_pInitialArray)[i].Func))())
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			//执行完毕之后就要把所有注册的函数销毁
			ms_pInitialArray->Clear();
		}
		MWDMAC_DELETE(ms_pInitialArray);
		ms_uiInitialObject = MWDObject::GetObjectManager().GetObjectNum();
		
		MWDMAC_DELETE(ms_pInitialPropertyArray);
		return 1;
	}
	bool MWDMain::Terminate()
	{
		
		//obj管理器已有容量
		ms_uiTerminalObject = MWDObject::GetObjectManager().GetObjectNum();
		//调用资源管理器把资源保存好
		//MWDResourceManager::CacheResource();
		if (ms_pTerminalArray) {
			//对需要销毁的obj进行排序
			ms_pTerminalArray->Sort(0, ms_pTerminalArray->GetNum(), PriorityCompare());
			//按排序的逆序进行销毁有依赖的obj
			for (int i = ms_pTerminalArray->GetNum() - 1; i >= 0; i--)
			{
				if (!(*((*ms_pTerminalArray)[i].Func))())
				{
					MWDMAC_ASSERT(0);
					return 0;
				}
			}
			//清空列表
			ms_pTerminalArray->Clear();
			MWDMAC_DELETE(ms_pTerminalArray);
		}
		if (ms_pTerminalPropertyArray) {
			//把没依赖的obj销毁
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
		
		
		//启动垃圾回收，启动所有垃圾回收事务
		/*MWDResourceManager::GCObject();
		MWDResourceManager::RunAllGCTask();
		MWDMAC_ASSERT(MWDResourceManager::IsReleaseAll());*/

		//obj管理器已有容量
		ms_uiTerminalObject = MWDObject::GetObjectManager().GetObjectNum();
		cout << "========================" << endl;
		cout << "||obj管理器容量：" << ms_uiTerminalObject <<"||" << endl;
		cout << "========================" << endl;
		//cout << "执行到了MWDMain::Terminate()" << endl;
		//判断是否清空成功 (MWDResourceManager没有实现，所以不可能清空)
		//MWDMAC_ASSERT(ms_uiTerminalObject == 0);
		return 1;
	}
}
