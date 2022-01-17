
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
		if (pPriority->CheckIsCircle(this))//�ж�this�Ƿ���pPriority�����������pPriority��������������
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

	//���API�Ǽ��㵱ǰ�������λ�Σ�����ֵԽ��˵������Խ�࣬ʵ�������ȼ�Խ��
	unsigned int MWDPriority::CheckPriorityNum()
	{
		//�����ǰ��Ų���0���ߵ�ǰ��û�������������ֱ�ӷ������ȼ�
		if (m_uiPriorityNum || !m_pPriorityThanList.GetNum())
			return m_uiPriorityNum;
		else
		{
			//����������е��������ҵ��������������ߵģ�+1�����Լ������
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
