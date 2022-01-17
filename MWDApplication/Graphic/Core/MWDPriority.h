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


		//重载优先级比较运算符
		MWDPriority& operator= (MWDPriority& rPriority);
		bool operator== (MWDPriority& rPriority);
		bool operator!= (MWDPriority& rPriority);

		bool operator >(MWDPriority& rPriority);
		bool operator <(MWDPriority& rPriority);

		bool operator >=(MWDPriority& rPriority);
		bool operator <=(MWDPriority& rPriority);

	private:

		//判断是否成环
		bool CheckIsCircle(const MWDPriority* pChecked)const;

		//检查优先级num
		unsigned int CheckPriorityNum();

		//当前优先级
		unsigned int m_uiPriorityNum;

		//维护一个比自己优先级高的数组
		MWDArray<MWDPriority*> m_pPriorityThanList;
	};

}

