#pragma once
#include <iostream>
#include "MWDContainer.h"
using namespace std;
namespace MWDEngine {
	template <class T>
	struct ListElement
	{
	public:
		ListElement()
		{
			m_Next = NULL;
			m_Pre = NULL;
		}
		~ListElement()
		{
			m_Next = NULL;
			m_Pre = NULL;
		}
		ListElement* m_Next;
		ListElement* m_Pre;
		T Element;
	};

	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDList : public MWDContainer<ListElement<T>, MMFun>
	{
	public:

		MWDList(bool bUnique = false) {
			m_Head = NULL;
			m_Tail = NULL;
			m_Unique = bUnique;
			m_Num = 0;
		};

		FORCEINLINE ~MWDList() { Clear(); };
		
		//�������
		void operator += (const MWDList& List) {
			m_Unique = List.m_Unique;
			ListElement<T>* pTemp = List.m_Head;
			while (pTemp)
			{
				AddElement(pTemp->Element);
				pTemp = pTemp->m_Next;
			}
		};

		FORCEINLINE unsigned int GetNum()const {
			return m_Num;
		};
		
		//˫����
		void AddElement(const T& Element) {
			if (m_Unique)
			{
				if (Has(Element))
				{
					return;
				}
			}
			//ListElement<T>* pElem = MWDContainer<ListElement<T>>::New(1);
			ListElement<T>* pElem = new ListElement<T>();
			pElem->Element = Element;
			pElem->m_Pre = NULL;
			pElem->m_Next = NULL;
			if (!m_Head) {
				m_Head = pElem;
				m_Tail = pElem;
			}
			else {
				m_Tail->m_Next = pElem;
				pElem->m_Pre = m_Tail;
				m_Tail = pElem;
			}
			m_Num++;
		};					CHECKED

		bool GetAndEraseTail(T& Element) {
			if (!m_Tail)
			{
				return false;
			}

			Element = m_Tail->Element;

			if (m_Tail == m_Head)
			{
				MWDContainer<T>:: Delete(m_Tail, 1);
				m_Tail = m_Head = NULL;
			}
			else
			{
				m_Tail->m_Pre->m_Next = NULL;
				ListElement<T>* pTemp = m_Tail;
				m_Tail = m_Tail->m_Pre;
				MWDContainer<T>::Delete(pTemp, 1);
			}

			m_Num--;
			return true;
		};

		//ֻ������popӦ����GetAndEraseTail()
		bool GetTail(T& Element) {
			if (!m_Tail)
			{
				return false;
			}
			Element = m_Tail->Element;
			return true;
		};

		void Clear() {
			ListElement<T>* pTemp = m_Head;
			while (pTemp)
			{
				ListElement<T>* pNext = pTemp->m_Next;
				MWDContainer<ListElement<T>>::Delete(pTemp, 1);
				pTemp = pNext;
			}
			m_Head = NULL;
			m_Tail = NULL;
			m_Num = 0;
		};

		void Erase(const T& Element) {
			ListElement<T>* pTemp = m_Head;
			while (pTemp)
			{
				if (pTemp->Element == Element)
				{
					//�����ǰ�ڵ���Ψһ�ڵ㣬��ֱ�ӶϿ�����
					if (!pTemp->m_Pre && !pTemp->m_Next)
					{
						m_Head = NULL;
						m_Tail = NULL;
					}
					//�����ǰ�ڵ�����ͷ
					else if (!pTemp->m_Pre)
					{
						pTemp->m_Next->m_Pre = NULL;
					}
					//�����ǰ�ڵ�����β
					else if (!pTemp->m_Next)
					{
						pTemp->m_Pre->m_Next = NULL;;
					}
					//�����ǰ�ڵ����м�Ľڵ�
					else
					{
						pTemp->m_Next->m_Pre = pTemp->m_Pre;
						pTemp->m_Pre->m_Next = pTemp->m_Next;
					}

					MWDContainer<T>:: Delete(pTemp, 1);
					m_Num--;

					if (!m_Unique)
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
			ListElement<T>* pTemp = m_Head;
			while (pTemp)
			{
				if (pTemp->Element == Element)
				{
					return true;
				}
				pTemp = pTemp->m_Next;
			}
			return false;
		};

		//����������ʵ�ָ�������������أ�������ά��һ���ڵ�ָ��
		class MWDListIterator
		{
		public:
			MWDListIterator(ListElement<T>* pNode = NULL)
			{
				m_Node = pNode;
			}
			MWDListIterator(const MWDListIterator& Iterator)
			{
				m_Node = Iterator.m_Node;
			}
			~MWDListIterator()
			{
				m_Node = NULL;
			}
			void operator= (const MWDListIterator& Iterator)
			{
				m_Node = Iterator.m_Node;
			}
			bool operator!= (const MWDListIterator& Iterator)
			{
				return (m_Node != Iterator.m_Node);
			}
			bool operator== (const MWDListIterator& Iterator)
			{
				return (m_Node == Iterator.m_Node);
			}

			void operator= (ListElement<T>* pNode)
			{
				m_Node = pNode;
			}
			bool operator!= (ListElement<T>* pNode)
			{
				return (m_Node != pNode);
			}
			bool operator== (ListElement<T>* pNode)
			{
				return (m_Node == pNode);
			}

			void operator++()
			{
				if (m_Node)
				{
					m_Node = m_Node->m_Next;
				}

			}
			void operator--()
			{
				if (m_Node)
				{
					m_Node = m_Node->m_Pre;
				}
			}

			bool IsValid()
			{
				return (m_Node != NULL);
			}

			T& operator*()

			{
				if (m_Node)
				{
					return m_Node->Element;
				}
				else
				{
					MWDMAC_ASSERT(0);
					return m_Node->Element;
				}
			}
		private:
			//������ά��һ���ڵ�
			ListElement<T>* m_Node;
		};

		MWDListIterator Begin()
		{
			return MWDListIterator(m_Head);
		}
		//ĩβ���ǿյ�
		MWDListIterator End()
		{
			return MWDListIterator();
		}
		//Tail���ǿյ�
		MWDListIterator Tail()
		{
			return MWDListIterator(m_Tail);
		}
	protected:

		ListElement<T>* m_Head;
		ListElement<T>* m_Tail;
		unsigned int m_Num;

		//��ǰ�����Ƿ����ظ�Ԫ��
		bool	m_Unique;

	};
	
}
