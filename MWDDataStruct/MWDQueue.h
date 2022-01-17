#pragma once
#include "MWDContainer.h"
#include "../MWDSystem/MWDMemManager.h"
#include "../MWDSystem/MWDSystem.h"

namespace MWDEngine {

	template <class T>
	struct QueueElement
	{
	public:
		QueueElement()
		{
			m_Next = NULL;
			m_Pre = NULL;
		}
		~QueueElement()
		{
			m_Next = NULL;
			m_Pre = NULL;
		}
		QueueElement* m_Next;
		QueueElement* m_Pre;
		T Element;
	};

	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDQueue :public MWDContainer<QueueElement<T>, MMFun> {
		public:
			MWDQueue(bool bUnique = false) {
				m_Head = NULL;
				m_Tail = NULL;
				m_Unique = bUnique;
				m_Num = 0;
			};
			~MWDQueue() { Clear(); };

			//接续队列
			void operator+= (const MWDQueue& Queue) {
				QueueElement<T>* pTemp = Queue.m_Head;
				while (pTemp)
				{
					Enqueue(pTemp->Element);
					pTemp = pTemp->m_Next;

				}
			};

			FORCEINLINE unsigned int GetNum()const { return m_Num; };

			//入队是从head进去
			void Enqueue(const T& Element) {
				if (m_Unique)
				{
					if (Has(Element))
					{
						return;
					}
				}

				QueueElement<T>* pElem = MWDContainer<QueueElement<T>>::New(1);
				pElem->Element = Element;
				pElem->m_Pre = NULL;
				pElem->m_Next = m_Head;
				if (m_Head)
				{
					m_Head->m_Pre = pElem;
				}
				m_Head = pElem;
				if (!m_Tail)
				{
					m_Tail = pElem;
				}
				m_Num++;
			};

			//出队是从尾部出
			bool Dequeue(T& Element) {
				if (!m_Tail)
				{
					return false;
				}
				//浅拷贝
				Element = m_Tail->Element;

				if (m_Tail == m_Head)
				{
					MWDContainer<QueueElement<T>>::Delete(m_Tail, 1);
					m_Tail = m_Head = NULL;
				}
				else
				{
					m_Tail->m_Pre->m_Next = NULL;
					QueueElement<T>* pTemp = m_Tail;
					m_Tail = m_Tail->m_Pre;
					MWDContainer<QueueElement<T>>::Delete(pTemp, 1);
				}

				m_Num--;
				return true;
			};

			//top指的是tail
			bool GetTop(T& Element) {
				if (!m_Tail)
				{
					return false;
				}
				Element = m_Tail->Element;
				return true;
			};

			void Clear() {
				QueueElement<T>* pTemp = m_Head;
				while (pTemp)
				{
					QueueElement<T>* pNext = pTemp->m_Next;
					MWDContainer<QueueElement<T>>::Delete(pTemp, 1);
					pTemp = pNext;
				}
				m_Head = NULL;
				m_Tail = NULL;
				m_Num = 0;
			};

			void Erase(const T& Element) {
				QueueElement<T>* pTemp = m_Head;
				while (pTemp)
				{
					if (pTemp->Element == Element)
					{
						//如果只有一个Element
						if (!pTemp->m_Pre && !pTemp->m_Next)
						{
							m_Head = NULL;
							m_Tail = NULL;
						}
						//在开头
						else if (!pTemp->m_Pre)
						{
							pTemp->m_Next->m_Pre = NULL;
						}
						//在末尾
						else if (!pTemp->m_Next)
						{
							pTemp->m_Next->m_Pre = pTemp->m_Pre;
						}
						//在中间
						else
						{
							pTemp->m_Next->m_Pre = pTemp->m_Pre;
							pTemp->m_Pre->m_Next = pTemp->m_Next;
						}
						MWDContainer<QueueElement<T>>::Delete(pTemp, 1);
						m_Num--;
						if (m_Unique)
						{
							continue;
						}
						else
						{
							break;
						}

					}
					pTemp = pTemp->m_Next;

				}
			};

			bool Has(const T& Element) {
				QueueElement<T>* pTemp = m_Head;
				while (pTemp)
				{
					if (&pTemp->Element == &Element)
					{
						return true;
					}
					pTemp = pTemp->m_Next;
				}
				return false;
			};
		protected:

			QueueElement<T>* m_Head;
			QueueElement<T>* m_Tail;
			unsigned int m_Num;
			bool	m_Unique;
	};
}
