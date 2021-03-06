#pragma once
#include "../MWDSystem/MWDMemManager.h"

#define CHECKED
#define MWDDATASTRUCT_EXPORTS
#ifdef MWDDATASTRUCT_EXPORTS
#define MWDDATASTRUCT_API __declspec(dllexport) 
#else 
#define MWDDATASTRUCT_API __declspec(dllimport) 
#endif

//typedef MWDSYSTEM_API MWDEngine::MWDHeapMemManager& (*MWDMemManagerFun)();

namespace MWDEngine {

	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDContainer :public MWDMemObject
	{
		protected:
			//这个New是给容器内的东西开辟空间
			T* New(unsigned int uiNum) {
				if (!uiNum)
				{
					return NULL;
				}

				#ifdef USE_CUSTOM_NEW
				T* Ptr = (T*)MMFun().Allocate(uiNum * sizeof(T), 0, true);
				MWDMAC_ASSERT(Ptr);
				if (!Ptr)
				{
					return NULL;
				}
				#else
				T* Ptr = (T*)malloc(sizeof(T) * uiNum);
				MWDMAC_ASSERT(Ptr);
				if (!Ptr)
				{
					return NULL;
				}
				#endif
				return Ptr;
			}
			//这个Delete先执行了所有东西的析构函数，然后清空存储的东西（但是自己没销毁）
			void Delete(T*& Ptr, unsigned int uiNum) {
				if (!Ptr)
				{
					return;
				}
				#ifdef USE_CUSTOM_NEW
				if (uiNum > 0)
				{
					if (ValueBase<T>::NeedsDestructor)
					{
						//手动执行一遍数组里的所有析构函数
						for (unsigned int i = 0; i < uiNum; i++)
						{
							(Ptr + i)->~T();
						}
					}
				}
				MMFun().Deallocate((char*)Ptr, 0, true);
				Ptr = NULL;
				#else
				free(Ptr);
				Ptr = NULL;
				#endif
			}

	};

	template <class T>
	void Merge(T* pBuffer, T* TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1)
	{

		unsigned int uiIndex1 = uiIndex0 + uiIndexLength0;

		unsigned int uiEnd0 = uiIndexLength0 + uiIndex0;
		unsigned int uiEnd1 = uiIndex1 + uiIndexLength1;

		unsigned int uiTempIndex = uiIndex0;
		while (uiIndex0 < uiEnd0 && uiIndex1 < uiEnd1)
		{
			if (pBuffer[uiIndex0] > pBuffer[uiIndex1])
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

	template <class T>
	void MergeSort(T* pBuffer, T* TempBuffer, unsigned int  uiBufferLength)
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
			MWDMemcpy(TempBuffer, pBuffer, sizeof(T) * uiBufferLength);
		}
	}

	template <class T, class N>
	void Merge(T* pBuffer, T* TempBuffer, unsigned int uiIndex0, unsigned int uiIndexLength0, unsigned int uiIndexLength1, N Compare)
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

	template <class T, class N>
	void MergeSort(T* pBuffer, T* TempBuffer, unsigned int  uiBufferLength, N Compare)
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
			MWDMemcpy(TempBuffer, pBuffer, sizeof(T) * uiBufferLength);
		}
	}

}