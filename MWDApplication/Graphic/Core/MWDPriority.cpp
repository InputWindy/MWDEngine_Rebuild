
#include "MWDPriority.h"
namespace MWDEngine {
	MWDPriority::MWDPriority()
	{
		m_uiPriorityNum = 0;
	}

	MWDPriority::MWDPriority(unsigned int uiPriorityNum)
	{
		m_uiPriorityNum = uiPriorityNum;
	}

	bool MWDPriority::AddPriorityThan(MWDPriority* pPriority)
	{
		if (!pPriority)
			return 0;
		if (pPriority->CheckIsCircle(this))//判断this是否在pPriority的依赖里，或者pPriority的依赖的依赖里
			return 0;
		m_uiPriorityNum = 0;
		m_pPriorityThanList.AddElement(pPriority);
		return 1;
	}

	MWDPriority& MWDPriority::operator=(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		m_uiPriorityNum = rPriority.m_uiPriorityNum;
		m_pPriorityThanList.Clear();
		m_pPriorityThanList = rPriority.m_pPriorityThanList;
		return *this;
	}

	bool MWDPriority::operator==(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum == rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::operator!=(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum != rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::operator>(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum > rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::operator<(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum < rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::operator>=(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum >= rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::operator<=(MWDPriority& rPriority)
	{
		rPriority.CheckPriorityNum();
		CheckPriorityNum();
		return (m_uiPriorityNum <= rPriority.m_uiPriorityNum);
	}

	bool MWDPriority::CheckIsCircle(const MWDPriority* pChecked) const
	{
		if (!pChecked)
			return false;

		for (unsigned int i = 0; i < m_pPriorityThanList.GetNum(); i++)
		{
			if (pChecked == m_pPriorityThanList[i])
				return true;
			else
				return m_pPriorityThanList[i]->CheckIsCircle(pChecked);

		}
		return false;
	}

	//这个API是计算当前类的生成位次，返回值越大说明依赖越多，实例化优先级越低
	unsigned int MWDPriority::CheckPriorityNum()
	{
		//如果当前序号不是0或者当前类没有添加依赖，就直接返回优先级
		if (m_uiPriorityNum || !m_pPriorityThanList.GetNum())
			return m_uiPriorityNum;
		else
		{
			//否则遍历所有的依赖，找到依赖里面序号最高的，+1就是自己的序号
			unsigned int uiMax = 0;
			for (unsigned int i = 0; i < m_pPriorityThanList.GetNum(); i++)
			{
				if (uiMax < m_pPriorityThanList[i]->CheckPriorityNum())
					uiMax = m_pPriorityThanList[i]->m_uiPriorityNum;

			}
			m_uiPriorityNum = uiMax + 1;
			return m_uiPriorityNum;
		}
	}

}
