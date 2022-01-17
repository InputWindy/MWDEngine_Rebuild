#include "MWDObject.h"

namespace MWDEngine {
	INITIAL_ONLY_BEGIN(MWDObject)
	INITIAL_ONLY_END(MWDObject)

	unsigned int MWDFastObjectManager::AddObject(MWDObject* p)
	{
		MWDMAC_ASSERT(m_FreeTable.GetNum() > 0);
		//最后一个ID分配给ｐ，然后删除掉最后一个ID
		unsigned int ID = m_FreeTable[m_FreeTable.GetNum() - 1];
		m_ObjectArray[ID] = p;
		int idx = m_FreeTable.GetNum() - 1;
		m_FreeTable.Erase(idx,idx);

		////ADD_COUNT_PROFILE(ObjectNum, 1)
		return ID;
	}
	void MWDFastObjectManager::DeleteObject(MWDObject* p)
	{
		if (m_ObjectArray[p->m_uiObjectID] != NULL)
		{
			//回收ID，清除指针记录，把Obj的ID设置为最大，表示已经不在Obj管理器内
			m_FreeTable.AddElement(p->m_uiObjectID);
			m_ObjectArray[p->m_uiObjectID] = NULL;
			//ADD_COUNT_PROFILE(ObjectNum, -1)
				p->m_uiObjectID = MAX_OBJECT_FLAG;
		}
	}
	
	bool MWDObject::Process(MWDName& FunName, void* para, void* ret, int ParaNum)
	{
		MWDRtti& t = GetType();
		//遍历所有的Function，找到名字相同的那个
		for (unsigned int i = 0; i < t.GetFunctionNum(); i++)
		{
			MWDFunction* p = t.GetFunction(i);
			if (p->GetName() == FunName)
			{
				if (ParaNum == -1)
				{
					p->ObjectFun(this, p, para, ret);
					return true;
				}
				else if (p->GetPropertyNum() == ParaNum)
				{
					p->ObjectFun(this, p, para, ret);
					return true;
				}

			}
		}
		return false;
	}

}
