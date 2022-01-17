#pragma once
#include "MWDGraphic.h"
#include <MWDArray.h>
#include <iostream>

using namespace std;
namespace MWDEngine {
	class  MWDPriority
	{
	public:
		MWDPriority();
		MWDPriority(unsigned int uiPriorityNum);
		~MWDPriority() {
			m_pPriorityThanList.Clear();
		};
		bool AddPriorityThan(MWDPriority* pPriority);
		FORCEINLINE unsigned int GetPriorityNum() {
			m_uiPriorityNum = CheckPriorityNum();
			return m_uiPriorityNum;
		};


		//�������ȼ��Ƚ������
		MWDPriority& operator= (MWDPriority& rPriority);
		bool operator== (MWDPriority& rPriority);
		bool operator!= (MWDPriority& rPriority);

		bool operator >(MWDPriority& rPriority);
		bool operator <(MWDPriority& rPriority);

		bool operator >=(MWDPriority& rPriority);
		bool operator <=(MWDPriority& rPriority);

	private:

		//�ж��Ƿ�ɻ�
		bool CheckIsCircle(const MWDPriority* pChecked)const;

		//������ȼ�num
		unsigned int CheckPriorityNum();

		//��ǰ���ȼ�
		unsigned int m_uiPriorityNum;

		//ά��һ�����Լ����ȼ��ߵ�����
		MWDArray<MWDPriority*> m_pPriorityThanList;
	};

}

