
#include "MWDResourceManager.h"
namespace MWDEngine {
	template<class T>
	bool MWDResourceArrayControll<T>::AddResource(const T& R)
	{
		m_ResourceArray.AddElement(R);
		m_TimeCount.AddElement(0);
		return true;
	}

	template<class T>
	bool MWDResourceArrayControll<T>::DeleteResource(const T& R)
	{
		unsigned int i = m_ResourceArray.FindElement(R);
		if (i == m_ResourceArray.GetNum())
		{
			return false;
		}
		else
		{
			m_ResourceArray.Erase(i);
			m_TimeCount.Erase(i);
		}
		return true;
	}

	template<class T>
	void MWDResourceArrayControll<T>::GCResource()
	{
		unsigned int i = 0;
		while (i < m_ResourceArray.GetNum())
		{
			if (m_ResourceArray[i]->GetRef() == 1)
			{
				m_TimeCount[i]++;
				if (m_TimeCount[i] > m_uiGCMaxTimeCount)
				{
					m_ResourceArray.Erase(i);
					m_TimeCount.Erase(i);
					break;
				}
				else
				{
					i++;
				}
			}
			else
			{
				m_TimeCount[i] = 0;
				i++;
			}
		}
	}

	template<class T>
	T MWDResourceArrayControll<T>::GetResource(unsigned int i)
	{
		if (i >= m_ResourceArray.GetNum())
		{
			return NULL;
		}
		return m_ResourceArray[i];
	}
}