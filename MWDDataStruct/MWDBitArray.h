#pragma once
#include "MWDContainer.h"
namespace MWDEngine {

	//按bit形式组织起来的T类型
	template<class T>
	class MWDBit
	{
	public:
		MWDBit(const MWDBit<T>& Bit)
		{
			m_Member = Bit.m_Member;
		}
		MWDBit()
		{
			m_Member = 0;
		}
		bool operator[] (unsigned int uiIndex)
		{
			int bit = uiIndex % sizeof(T);
			return (((m_Member & (1 << bit)) >> bit) == 1);
		}

		void operator= (const MWDBit<T>& Bit)
		{
			m_Member = Bit.m_Member;
		}
		void Set(unsigned int uiIndex, bool bValue)
		{

			int bit = uiIndex % SYSTEM_BIT_WIDTH;

			if (bValue == true)
				m_Member = (m_Member | (1 << bit));
			else
				m_Member = (m_Member & (~(1 << bit)));
		}
		void ClearAll()
		{
			m_Member = 0;
		}
		void SetAll()
		{
			m_Member = 0xFFFFFFFF;
		}
		bool HasValue()
		{
			return m_Member > 0;
		}
	protected:
		T m_Member;
	};


	class MWDBitArray
	{
	public:
		MWDBitArray(const MWDBitArray& BitArray)
		{
			MWDMAC_DELETEA(m_pBuffer);
			m_uiSize = BitArray.m_uiSize;
			m_pBuffer = new unsigned int[m_uiSize];
			MWDMemcpy(m_pBuffer, BitArray.m_pBuffer, sizeof(unsigned int) * m_uiSize);
		}
		MWDBitArray(unsigned int uiSize = 0)
		{
			m_pBuffer = 0;
			m_uiSize = 0;
			Resize(uiSize);
		}
		~MWDBitArray()
		{
			MWDMAC_DELETEA(m_pBuffer);
		}

		void Resize(unsigned int uiSize)
		{
			if (uiSize == 0)
			{
				return;
			}
			unsigned int* pNewVector = 0;

			if (uiSize % SYSTEM_BIT_WIDTH == 0)
				uiSize = uiSize / SYSTEM_BIT_WIDTH;
			else
				uiSize = (uiSize / SYSTEM_BIT_WIDTH) + 1;

			pNewVector = new unsigned int[uiSize];


			if (!pNewVector)
				return;
			MWDMemset(pNewVector, 0, uiSize * sizeof(unsigned int));
			unsigned int uiMin;
			if (uiSize < m_uiSize)
				uiMin = uiSize;
			else
				uiMin = m_uiSize;

			for (unsigned int uiIndex = 0; uiIndex < uiMin; uiIndex++)
				pNewVector[uiIndex] = m_pBuffer[uiIndex];

			m_uiSize = uiSize;

			MWDMAC_DELETEA(m_pBuffer);

			m_pBuffer = pNewVector;

		}

		bool operator[] (unsigned int uiIndex)
		{
			int cell = uiIndex / SYSTEM_BIT_WIDTH;
			int bit = uiIndex % SYSTEM_BIT_WIDTH;
			return (((m_pBuffer[cell] & (1 << bit)) >> bit) == 1);
		}

		void operator= (const MWDBitArray& BitArray)
		{
			m_uiSize = BitArray.m_uiSize;
			MWDMAC_DELETEA(m_pBuffer);
			m_pBuffer = new unsigned int[m_uiSize];
			MWDMemcpy(m_pBuffer, BitArray.m_pBuffer, m_uiSize * sizeof(unsigned int));
		}
		void Set(unsigned int uiIndex, bool bValue)
		{
			int cell = uiIndex / SYSTEM_BIT_WIDTH;

			int bit = uiIndex % SYSTEM_BIT_WIDTH;


			if (bValue == true)

				m_pBuffer[cell] = (m_pBuffer[cell] | (1 << bit));
			else

				m_pBuffer[cell] = (m_pBuffer[cell] & (~(1 << bit)));
		}
		void ClearAll()
		{
			for (unsigned int uiIndex = 0; uiIndex < m_uiSize; uiIndex++)
				m_pBuffer[uiIndex] = 0;
		}
		void SetAll()
		{

			for (unsigned int uiIndex = 0; uiIndex < m_uiSize; uiIndex++)
				m_pBuffer[uiIndex] = 0xFFFFFFFF;
		}
		unsigned int Size()
		{
			return m_uiSize * SYSTEM_BIT_WIDTH;
		}
		unsigned int GetCell(int uiIndex)
		{
			return m_pBuffer[uiIndex];
		}

		unsigned int* m_pBuffer;
		unsigned int m_uiSize;
	};
}

