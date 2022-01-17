#pragma once
#include <iostream>
#include "MWDContainer.h"
#include <map>
using namespace std;
namespace MWDEngine
{

	template<class KEY, class VALUE>
	class MWDDATASTRUCT_API MWDMapElement : public MWDMemObject
	{
	public:
		KEY		Key;
		VALUE	Value;

		MWDMapElement() {};
		~MWDMapElement() {};
		MWDMapElement(const MWDMapElement<KEY, VALUE>& Element)
		{
			Key = Element.Key;
			Value = Element.Value;
		}
		MWDMapElement(const KEY& _Key, const VALUE& _Value)
		{
			Key = _Key;
			Value = _Value;
		}
		void operator= (const MWDMapElement<KEY, VALUE>& Element)
		{
			Key = Element.Key;
			Value = Element.Value;
		}
	};

	template <class KEY, class VALUE, class N>
	void Merge(MWDMapElement<KEY, VALUE>* pBuffer, MWDMapElement<KEY, VALUE>* TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1, N Compare)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (Compare(pBuffer[uiIndex0], pBuffer[uiIndex1]) == 0)
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}

	template <class KEY, class VALUE, class N>
	void MergeSort(MWDMapElement<KEY, VALUE>* pBuffer, MWDMapElement<KEY, VALUE>* TempBuffer, unsigned int  uiBufferLength, N Compare)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength, Compare);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength, Compare);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0, Compare);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			MWDMemcpy(TempBuffer, pBuffer, sizeof(MWDMapElement<KEY, VALUE>) * uiBufferLength);
		}
	}

	template <class KEY, class VALUE>
	void Merge(MWDMapElement<KEY, VALUE>* pBuffer, MWDMapElement<KEY, VALUE>* TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (pBuffer[uiIndex0].Key > pBuffer[uiIndex1].Key)
			{

				TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
				uiIndex1++;
			}
			else
			{
				TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
				uiIndex0++;
			}
			uiTempIndex++;
		}


		while (uiIndex0 < uiEnd0)
		{

			TempBuffer[uiTempIndex] = pBuffer[uiIndex0];
			uiTempIndex++;
			uiIndex0++;
		}
		while (uiIndex1 < uiEnd1)
		{
			TempBuffer[uiTempIndex] = pBuffer[uiIndex1];
			uiTempIndex++;
			uiIndex1++;
		}

	}

	template <class KEY, class VALUE>
	void MergeSort(MWDMapElement<KEY, VALUE>* pBuffer, MWDMapElement<KEY, VALUE>* TempBuffer, unsigned int  uiBufferLength)
	{
		unsigned int uiDetLength = 1;
		unsigned int uiNumSwap = 0;
		while (uiDetLength < uiBufferLength)
		{
			unsigned int uiLastDetLength = uiDetLength;
			uiDetLength = uiDetLength << 1;
			unsigned int i = 0;
			for (; i + uiDetLength < uiBufferLength; i = i + uiDetLength)
			{
				Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLastDetLength);
			}


			if (i < uiBufferLength)
			{
				unsigned int uiLeftLength = uiBufferLength - i;
				if (uiLeftLength > uiLastDetLength)
				{
					Merge(pBuffer, TempBuffer, i, uiLastDetLength, uiLeftLength - uiLastDetLength);
				}
				else
				{
					Merge(pBuffer, TempBuffer, i, uiLeftLength, 0);
				}
			}
			Swap(pBuffer, TempBuffer);
			uiNumSwap++;
		}
		if ((uiNumSwap % 2) != 0)
		{
			MWDMemcpy(TempBuffer, pBuffer, sizeof(MWDMapElement<KEY, VALUE>) * uiBufferLength);
		}
	}


	template<class KEY, class VALUE, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDMap : public MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>
	{
	public:
		enum
		{
			DEFAULT_GROWBY = 10
		};

		MWDMap(unsigned int uiGrowBy = DEFAULT_GROWBY) {
			m_GrowBy = uiGrowBy;
			m_Buffer = NULL;
			m_CurUse = 0;
			m_BufferNum = 0;
			m_AllocNum = 0;
		};
		~MWDMap(){
			MWDContainer<MWDMapElement<KEY,VALUE>,MMFun>::Delete(m_Buffer, m_CurUse);
		};

		void SetBufferNum(unsigned int uiBufferNum) {
			if (uiBufferNum)
			{
				MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(m_Buffer, m_CurUse);
				m_BufferNum = uiBufferNum;
				m_Buffer =MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::New(uiBufferNum);
				MWDMAC_ASSERT(m_Buffer)
				if (!m_Buffer)
					return;
				m_CurUse = uiBufferNum;
				for (unsigned int i = 0; i < m_BufferNum; i++)
				{
					new(m_Buffer + i) MWDMapElement<KEY, VALUE>();
				}
			}
		};
		
		void AddBufferNum(unsigned int uiBufferNum) {
			if (uiBufferNum)
			{
				m_AllocNum++;
				m_GrowBy = m_AllocNum * m_GrowBy;
				MWDMapElement<KEY, VALUE>* pBuffer = MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::New(m_BufferNum + uiBufferNum);
				MWDMAC_ASSERT(pBuffer)
				if (!pBuffer)
					return;

				if (m_Buffer && m_CurUse)
				{
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						new(pBuffer + i) MWDMapElement<KEY, VALUE>(m_Buffer[i]);
					}
				}
				MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(m_Buffer, m_CurUse);
				m_BufferNum = m_BufferNum + uiBufferNum;
				m_Buffer = pBuffer;
			}
		};
		
		//深拷贝
		void operator= (const MWDMap<KEY, VALUE, MMFun>& Map) {
			//如果map空间足够
			if (m_BufferNum >= Map.GetNum())
			{
				Clear();
				m_CurUse = Map.GetNum();
				MWDMapElement<KEY, VALUE>* pBuffer = Map.GetBuffer();
				for (unsigned int i = 0; i < m_CurUse; i++)
				{
					//构造函数里传入同类是浅拷贝，=不重载的时候也是浅拷贝，要重载成深拷贝
					new(m_Buffer + i) MWDMapElement<KEY, VALUE>(pBuffer[i]);
				}
			}
			else
			{
				//如果空间不够就把当前map直接清空，然后把别的map直接深拷贝进来
				MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(m_Buffer, m_CurUse);
				m_BufferNum = Map.GetBufferNum();

				m_CurUse = Map.GetNum();


				m_Buffer = MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::New(m_BufferNum);
				if (!m_Buffer)
					return;
				MWDMapElement<KEY, VALUE>* pBuffer = Map.GetBuffer();
				for (unsigned int i = 0; i < m_CurUse; i++)
				{
					new(m_Buffer + i) MWDMapElement<KEY, VALUE>(pBuffer[i]);
				}
			}
			return;
		};


		FORCEINLINE unsigned int GetNum()const {
			return m_CurUse;
		};

		FORCEINLINE unsigned int GetBufferNum()const {
			return m_BufferNum;
		};

		FORCEINLINE MWDMapElement<KEY, VALUE>* GetBuffer()const {
			return m_Buffer;
		};
		FORCEINLINE void SetGrowBy(unsigned int uiGrowBy) {
			m_GrowBy = uiGrowBy;
		};

		template<class KEY1, class VALUE1>
		void AddElement(const MWDMapElement<KEY1, VALUE1>& Element) {
			if (Find(Element.Key) != m_CurUse)
				return;
			if (m_CurUse == m_BufferNum)
			{
				if (!m_GrowBy)
					return;
				AddBufferNum(m_GrowBy);
			}
			new(m_Buffer + m_CurUse) MWDMapElement<KEY, VALUE>(Element);
			m_CurUse++;
		};
		
		void AddElement(const KEY& Key, const VALUE& Value) {
			if (Find(Key) != m_CurUse)
				return;

			if (m_CurUse == m_BufferNum)
			{
				if (!m_GrowBy)
					return;
				AddBufferNum(m_GrowBy);
			}
			
			
			//空间足够就直接填写信息传进去
			//如果已经有了内存空间，可以不实例化，直接声明类，填写信息，然后把这套信息作为构造函数参数用placement new写进内存空间
			MWDMapElement<KEY, VALUE> Element;
			Element.Key = Key;
			Element.Value = Value;

			new(m_Buffer + m_CurUse) MWDMapElement<KEY, VALUE>(Element);
			m_CurUse++;
		};

		template<class KEY1, class VALUE1, MWDMemManagerFun MMFun1>
		void AddElement(const MWDMap<KEY1, VALUE1, MMFun1>& Map, unsigned int uiBegin, unsigned int uiEnd) {
			if (uiBegin > uiEnd)
			{
				MWDMAC_ASSERT(0)
				return;
			}
			if (uiEnd >= Map.GetNum())
			{
				uiEnd = Map.GetNum() - 1;
			}
			int iAddNum = uiEnd - uiBegin + 1;
			int iHaveNum = m_BufferNum - m_CurUse;

			int iNeedNum = iHaveNum - iAddNum;

			if (iNeedNum < 0)
			{
				if (m_GrowBy)
				{
					iNeedNum = -iNeedNum;
					if ((unsigned int)iNeedNum < m_GrowBy)
						AddBufferNum(m_GrowBy);
					else
					{
						AddBufferNum(iNeedNum);
					}
				}
				else
				{
					iAddNum = iHaveNum;
				}
			}

			if (iAddNum == 1)
			{
				AddElement(Map[uiBegin]);
			}
			else if (iAddNum == 2)
			{
				AddElement(Map[uiBegin]);
				AddElement(Map[uiBegin + 1]);
			}
			else
			{
				MWDMapElement<KEY1, VALUE1>* pBuffer = Map.GetBuffer();
				for (unsigned int i = m_CurUse; i < (unsigned int)iAddNum + m_CurUse; i++)
				{
					new(m_Buffer + i) MWDMapElement<KEY, VALUE>(pBuffer[uiBegin]);
					uiBegin++;
				}
				m_CurUse += iAddNum;
			}
		};

		FORCEINLINE MWDMapElement<KEY, VALUE>& operator[] (unsigned int i)const {
			return m_Buffer[i];
		};

		//手动调用所有内容的析构函数
		void Clear() {
			for (unsigned int i = 0; i < m_CurUse; i++)
			{
				m_Buffer[i].~MWDMapElement<KEY, VALUE>();
			}
			m_CurUse = 0;
		};
		
		//当前map容器总共的内存占用大小
		FORCEINLINE unsigned int GetSize()const {
			return sizeof(unsigned int) * 4 + sizeof(MWDMapElement<KEY, VALUE> *) + sizeof(MWDMapElement<KEY, VALUE>) * m_BufferNum;
		};

		void Erase(unsigned int i) {
			Erase(i,i);
		};

		void Erase(unsigned int uiBegin, unsigned int uiEnd) {
			if (uiEnd >= m_CurUse)
			{
				MWDMAC_ASSERT(0)
				return;
			}
			if (uiBegin > uiEnd)
			{
				MWDMAC_ASSERT(0)
				return;
			}
			unsigned int k;
			unsigned int uiMoveNum = m_CurUse - 1 - uiEnd;
			unsigned int uiMoveIndex;
			for (unsigned int uiDelete = uiBegin; uiDelete <= uiEnd; uiDelete++)
			{
				m_Buffer[uiDelete].~MWDMapElement<KEY, VALUE>();
				//cout << m_Buffer[uiDelete].Key << endl;
			}
			//把后面的移到前面
			for (k = 0; k < uiMoveNum; k++)
			{
				uiMoveIndex = uiEnd + k + 1;
				if (uiMoveIndex < m_CurUse)
				{
					new(m_Buffer + uiBegin + k) MWDMapElement<KEY, VALUE>(m_Buffer[uiMoveIndex]);
					//cout << m_Buffer[uiBegin + k].Key <<"   " << m_Buffer[uiBegin + k].Value << endl;
					m_Buffer[uiMoveIndex].~MWDMapElement<KEY, VALUE>();
				}
			}
			m_CurUse -= ((uiEnd - uiBegin) + 1);
		};

		template <class N>
		void Sort(unsigned int uiBegin, unsigned int uiEnd, N Compare) {
			if (uiEnd >= m_CurUse)
			{
				uiEnd = m_CurUse - 1;
			}
			if (uiBegin >= uiEnd)
			{
				return;
			}
			MWDMapElement<KEY, VALUE>* pBuffer = MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::New(uiEnd - uiBegin + 1);
			for (unsigned int i = 0; i < uiEnd - uiBegin + 1; i++)
			{
				new(pBuffer + i) MWDMapElement<KEY, VALUE>();
			}
			MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::MergeSort(m_Buffer + uiBegin, pBuffer, uiEnd - uiBegin + 1, Compare);
			MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(pBuffer, uiEnd - uiBegin + 1);
		};

		void Sort(unsigned int uiBegin, unsigned int uiEnd) {
			if (uiEnd >= m_CurUse)
			{
				uiEnd = m_CurUse - 1;
			}
			if (uiBegin >= uiEnd)
			{
				MWDMAC_ASSERT(0)
				return;
			}
			MWDMapElement<KEY, VALUE>* pBuffer = MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::New(uiEnd - uiBegin + 1);
			for (unsigned int i = 0; i < uiEnd - uiBegin + 1; i++)
			{
				new(pBuffer + i) MWDMapElement<KEY, VALUE>();
			}
			MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::MergeSort(m_Buffer + uiBegin, pBuffer, uiEnd - uiBegin + 1);
			MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(pBuffer, uiEnd - uiBegin + 1);
		};
		
		//返回的是当前element在buffer中的位置,没找到返回m_CurUse
		unsigned int Find(const KEY& Key)const {
			unsigned int i = 0;
			for (i = 0; i < m_CurUse; i++)
			{
				if (m_Buffer[i].Key == Key)
				{
					break;
				}
			}
			return i;
		};

		//返回的是当前element在buffer中的位置,没找到返回m_CurUse
		unsigned int FindValueIndex(const VALUE& Value)const {
			unsigned int i = 0;
			for (i = 0; i < m_CurUse; i++)
			{
				if (m_Buffer[i].Value == Value)
				{
					break;
				}
			}
			return i;
		};


		FORCEINLINE void Destroy() {
			Clear();MWDContainer<MWDMapElement<KEY, VALUE>, MMFun>::Delete(m_Buffer, m_CurUse);
		};
	protected:
		//map<KEY, VALUE>* m_Buffer;
		MWDMapElement<KEY, VALUE>* m_Buffer;
		unsigned int m_GrowBy;
		unsigned int m_CurUse;
		unsigned int m_BufferNum;
		unsigned int m_AllocNum;
	};

	template<class KEY, class VALUE, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDMapOrder : public MWDMap<KEY, VALUE, MMFun>
	{
	public:
		MWDMapOrder(unsigned int uiGrowBy = MWDMap<KEY, VALUE, MMFun>::DEFAULT_GROWBY) {
			
		};
		~MWDMapOrder() {
			
		};

		//没有重写父类的AddElement，如果使用里氏赋值，会调用到父类的AddElement
		template<class KEY1, class VALUE1>
		void AddElement(const MWDMapElement<KEY1, VALUE1>& Element) {
			if (Find(Element.Key) != MWDMap<KEY, VALUE, MMFun>::m_CurUse)
				return;
			if (MWDMap<KEY, VALUE, MMFun>::m_CurUse == MWDMap<KEY, VALUE, MMFun>::m_BufferNum)
			{
				if (!MWDMap<KEY, VALUE, MMFun>::m_GrowBy)
					return;
				MWDMap<KEY, VALUE, MMFun>::AddBufferNum(MWDMap<KEY, VALUE, MMFun>::m_GrowBy);
			}

			unsigned int uiIndex;
			if (MWDMap<KEY, VALUE, MMFun>::m_CurUse == 0)
			{
				uiIndex = 0;
			}
			else if (MWDMap<KEY, VALUE, MMFun>::m_CurUse == 1)
			{
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[0].Key > Element.Key)
				{
					uiIndex = 0;
				}
				else
				{
					uiIndex = 1;
				}
			}
			else if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[0].Key > Element.Key)
			{
				uiIndex = 0;
			}
			else if (Element.Key > MWDMap<KEY, VALUE, MMFun>::m_Buffer[MWDMap<KEY, VALUE, MMFun>::m_CurUse - 1].Key)
			{
				uiIndex = MWDMap<KEY, VALUE, MMFun>::m_CurUse;
			}
			else
			{
				uiIndex = Process(0, MWDMap<KEY, VALUE, MMFun>::m_CurUse - 1, Element);
			}
			if (MWDMap<KEY, VALUE, MMFun>::m_CurUse == uiIndex)
			{
				new(MWDMap<KEY, VALUE, MMFun>::m_Buffer + uiIndex) MWDMapElement<KEY, VALUE>(Element);
			}
			else
			{
				new(MWDMap<KEY, VALUE, MMFun>::m_Buffer + MWDMap<KEY, VALUE, MMFun>::m_CurUse) MWDMapElement<KEY, VALUE>(MWDMap<KEY, VALUE, MMFun>::m_Buffer[MWDMap<KEY, VALUE, MMFun>::m_CurUse - 1]);
				for (int i = (int)MWDMap<KEY, VALUE, MMFun>::m_CurUse - 2; i >= (int)uiIndex; i--)
				{
					MWDMap<KEY, VALUE, MMFun>::m_Buffer[i + 1] = MWDMap<KEY, VALUE, MMFun>::m_Buffer[i];
				}
				MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex] = Element;
			}
			MWDMap<KEY, VALUE, MMFun>::m_CurUse++;
		};

		void AddElement(const KEY& Key, const VALUE& Value) {

			MWDMapElement<KEY, VALUE> Element;

			Element.Key = Key;
			Element.Value = Value;
			
			AddElement(Element);
			
		};

		//找不到返回m_CurUse
		unsigned int Find(const KEY& Key)const {
			if (MWDMap<KEY, VALUE, MMFun>::m_CurUse)
			{
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[0].Key > Key)
				{
					return MWDMap<KEY, VALUE, MMFun>::m_CurUse;
				}
				else if (Key > MWDMap<KEY, VALUE, MMFun>::m_Buffer[MWDMap<KEY, VALUE, MMFun>::m_CurUse - 1].Key)
				{
					return MWDMap<KEY, VALUE, MMFun>::m_CurUse;
				}
				else
				{
					return FindElement(0, MWDMap<KEY, VALUE, MMFun>::m_CurUse - 1, Key);
				}
			}
			else
			{
				return MWDMap<KEY, VALUE, MMFun>::m_CurUse;
			}
		};

	protected:
		//二分递归查找，找不到返回m_CurUse
		template<class KEY1, class VALUE1>
		unsigned int Process(unsigned int uiIndex0, unsigned int uiIndex1, const MWDMapElement<KEY1, VALUE1>& Element) {
			if (uiIndex0 == uiIndex1)
			{
				return uiIndex0;
			}
			else if (uiIndex1 - uiIndex0 == 1)
			{
				return uiIndex1;
			}
			else
			{
				unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex].Key == Element.Key)
				{
					return uiIndex;
				}
				else if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex].Key > Element.Key)
				{
					return Process(uiIndex0, uiIndex, Element);
				}
				else
				{
					return Process(uiIndex, uiIndex1, Element);
				}
			}
		};

		//二分递归查找，找不到返回m_CurUse
		unsigned int FindElement(unsigned int uiIndex0, unsigned int uiIndex1, const KEY& Key)const {
			if (uiIndex0 == uiIndex1)
			{
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex0].Key == Key)
				{
					return uiIndex0;
				}
				else
				{
					return MWDMap<KEY, VALUE, MMFun>::m_CurUse;
				}
			}
			else if (uiIndex1 - uiIndex0 == 1)
			{
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex0].Key == Key)
				{
					return uiIndex0;
				}
				else if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex1].Key == Key)
				{
					return uiIndex1;
				}
				else
				{
					return MWDMap<KEY, VALUE, MMFun>::m_CurUse;
				}
			}
			else
			{
				unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
				if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex].Key == Key)
				{
					return uiIndex;
				}
				else if (MWDMap<KEY, VALUE, MMFun>::m_Buffer[uiIndex].Key > Key)
				{
					return FindElement(uiIndex0, uiIndex, Key);
				}
				else
				{
					return FindElement(uiIndex, uiIndex1, Key);
				}
			}
		};
	};
	#define  MAP_ERASE(MAP,KEY)  MAP.Erase(MAP.Find(KEY));
}