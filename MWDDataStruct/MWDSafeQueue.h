#pragma once
#include "MWDQueue.h"
#include "../MWDSystem/MWDSynchronize.h"
#include <iostream>
using namespace std;
namespace MWDEngine {
	template <class T>
	class MWDDATASTRUCT_API MWDSafeQueue : public MWDMemObject
	{
	public:

		MWDSafeQueue(bool bUnique = false) {};
		~MWDSafeQueue() {};
		void Enqueue(const T& Element) {
			m_CriticalSec.Lock();
			m_Queue.Enqueue(Element);
			m_CriticalSec.Unlock();
		};
		void Dequeue(T& Element) {
			m_CriticalSec.Lock();
			m_Queue.Dequeue(Element);
			m_CriticalSec.Unlock();
		};
		void GetTop(T& Element) {
			m_CriticalSec.Lock();
			m_Queue.GetTop(Element);
			m_CriticalSec.Unlock();
		};
		void Clear() {
			m_CriticalSec.Lock();
			m_Queue.Clear();
			m_CriticalSec.Unlock();
		};
		void Erase(const T& Element) {
			m_CriticalSec.Lock();
			m_Queue.Erase(Element);
			m_CriticalSec.Unlock();
		};
		bool IsEmpty(){
			bool bEmpty = false;
			m_CriticalSec.Lock();
			if (m_Queue.GetNum() == 0)
			{
				bEmpty = true;
			}
			m_CriticalSec.Unlock();
			return bEmpty;
		};
	protected:
		MWDQueue<T> m_Queue;
		MWDCriticalSection m_CriticalSec;

	private:

	};
}
