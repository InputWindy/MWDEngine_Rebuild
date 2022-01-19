#pragma once
#include <iostream>
#include "MWDContainer.h"
using namespace std;
namespace MWDEngine {
	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDArray:public MWDContainer<T, MMFun>
	{
		//变长数组没办法进行浅拷贝，因为存储的东西数量不同，只能深拷贝：先全部清空，再全部实例化
		public:
			//数组变长步长
			enum
			{
				DEFAULT_GROWBY = 10
			};
			//实例化的时候Array时，数组长度为0
			MWDArray(unsigned int uiGrowBy = MWDArray::DEFAULT_GROWBY) {
				m_GrowBy = uiGrowBy;
				m_Buffer = NULL;
				m_CurUse = 0;
				m_BufferNum = 0;
				m_AllocNum = 0;
			};
			//副本构造器会创建一个新的对象
			MWDArray(const MWDArray& Array) {
				m_GrowBy = Array.m_GrowBy;
				m_Buffer = NULL;
				m_CurUse = 0;
				m_BufferNum = 0;
				m_AllocNum = 0;
				//这一步只是把Array里的数据给了*this
				*this = Array;
			};
			
			//数组深拷贝
			void operator= (const MWDArray<T, MMFun>& Array) {
				//当拷贝过来的数组比当前数组容量小时：把外来数组内容直接重写在当前Array的内存空间
				//GetNum返回的是m_CurUse
				if (m_BufferNum >= Array.GetNum())
				{
					Clear();
					m_CurUse = Array.GetNum();
					T* pBuffer = Array.GetBuffer();
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						//========================================
						// placement new:
						//		意思是在m_Buffer + i地址上new一个T
						//========================================
						new(m_Buffer + i) T(pBuffer[i]);
					}
				}
				else
				{
					//否则就清空当前Array的内存空间，重新构造当前Array
					MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
					m_BufferNum = Array.GetBufferNum();

					m_CurUse = Array.GetNum();
					//给Array重新分配空间
					m_Buffer = MWDContainer<T, MMFun>::New(m_BufferNum);
					if (!m_Buffer)
						return;

					//获取原Array里存储的东西
					T* pBuffer = Array.GetBuffer();
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						//把原Array里的东西全部重新实例化一遍
						//PS:任何类都有个深拷贝的默认构造函数，传入的是一个实例，new出来的是一个新的实例
						new(m_Buffer + i) T(pBuffer[i]);
					}
				}
			};
			void operator+= (const MWDArray<T, MMFun>& Array) {
				AddElement(Array, 0, Array.GetNum());
			};
			~MWDArray() {
				//清空Array里存的东西
				MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
				m_BufferNum = 0;
				m_CurUse = 0;
				m_AllocNum = 0;
			};
			
			//这个函数会导致数据丢失：清空当前的数据,实例化uiBufferNum个空的T
			void ResetBufferNum(unsigned int uiBufferNum) {
				if (uiBufferNum)
				{
					MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
					m_BufferNum = uiBufferNum;

					m_Buffer = MWDContainer<T, MMFun>::New(uiBufferNum);
					if (!m_Buffer)
						return;
					m_CurUse = uiBufferNum;
					for (unsigned int i = 0; i < m_BufferNum; i++)
					{
						//SetBufferNum直接数据丢失
						new(m_Buffer + i) T();
					}
				}
			};
			void AddBufferNum(unsigned int uiBufferNum) {
				if (uiBufferNum)
				{
					m_AllocNum++;
					//每多延长一次，延长粒度就要增大
					m_GrowBy = m_AllocNum * m_GrowBy;

					T* pBuffer = NULL;
					//申请总空间
					pBuffer = MWDContainer<T, MMFun>::New(m_BufferNum + uiBufferNum);
					if (!pBuffer)
						return;

					if (m_Buffer && m_CurUse)
					{
						for (unsigned int i = 0; i < m_CurUse; i++)
						{
							//把原Array里的东西拷贝到新的pBuffer里
							new(pBuffer + i) T(m_Buffer[i]);
						}
					}
					//把原来的数组内容清空
					MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
					m_BufferNum += uiBufferNum;
					//数组内容替换成新的内容
					m_Buffer = pBuffer;
				}
			};
			//当前元素个数
			FORCEINLINE unsigned int GetNum()const { return m_CurUse; };
			FORCEINLINE unsigned int GetBufferNum()const { return m_BufferNum; };

			FORCEINLINE T* GetBuffer()const { return m_Buffer; };
			FORCEINLINE void SetGrowBy(unsigned int uiGrowBy) { cout << "执行了MWDArray.h::SetGrowBy=" << uiGrowBy << endl; m_GrowBy = uiGrowBy; };

			//返回整个Array占用的总字节数：各种属性值+uiNum个数的T空间
			FORCEINLINE unsigned int GetSize()const {
				return sizeof(unsigned int) * 4 + sizeof(T*) + sizeof(T) * m_BufferNum;
			};
			
			template <class N, MWDMemManagerFun MMFunN>
			void AddElement(const MWDArray<N, MMFunN>& Array, unsigned int uiBegin, unsigned int uiEnd) {
				MWDMAC_ASSERT(uiBegin <= uiEnd)
					if (uiBegin > uiEnd)
					{
						return;
					}
				//限定拷贝到末尾
				if (uiEnd >= Array.GetNum())
				{
					uiEnd = Array.GetNum() - 1;
				}
				//拷贝包括两个端点
				int iAddNum = uiEnd - uiBegin + 1;
				//剩余空间
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
						//如果数组不允许再延长的话，就只能把数组加满，多出的部分丢失
						iAddNum = iHaveNum;
					}
				}
				//如果只添加一个元素
				if (iAddNum == 1)
				{
					AddElement(Array[uiBegin]);
				}
				//如果只添加了两个元素
				else if (iAddNum == 2)
				{
					AddElement(Array[uiBegin]);
					AddElement(Array[uiBegin + 1]);
				}
				else
				{
					//方案1
					N* pBuffer = Array.GetBuffer();
					for (unsigned int i = m_CurUse; i < (unsigned int)iAddNum + m_CurUse; i++)
					{
						new(m_Buffer + i) T(pBuffer[uiBegin]);
						uiBegin++;
					}
					m_CurUse += iAddNum;
					//方案2
					//MWDMemcpy(&m_Buffer[m_CurUse],&Array.GetBuffer()[uiBegin], sizeof(T) * iAddNum);
				}
			};
			
			void AddElement(const T& Element) {
				//cout << "添加了！" << endl;
				//如果空间不够就开新的空间
				if (m_CurUse == m_BufferNum)
				{
					if (!m_GrowBy) {
						cout << "MWDArray.h::AddElement:m_GrowBy==0" <<"______"<< "m_GrowBy=" << m_GrowBy << endl;;
						return;
					}
						
					AddBufferNum(m_GrowBy);

				}
				//空间足够就直接在下一个数组位置拷贝一个新元素
				new(m_Buffer + m_CurUse) T(Element);
				m_CurUse++;
			};

			FORCEINLINE T& operator[] (unsigned int i)const { return m_Buffer[i]; };

			//调用所有项的析构函数，但没有释放内存
			void Clear() {
				if (m_CurUse == 0)
				{
					return;
				}
				if (ValueBase<T>::NeedsDestructor)
				{
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						m_Buffer[i].~T();
					}
				}
				m_CurUse = 0;
			};
			//这个是清空数据并且释放内存(调用析构)
			void Destroy() { 
				Clear(); 
				MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
				m_CurUse = 0;
				m_BufferNum = 0; 
			};
			FORCEINLINE void Erase(unsigned int i) {if (i < m_CurUse) Erase(i, i); };
			
			void Erase(unsigned int uiBegin, unsigned int uiEnd) {
				MWDMAC_ASSERT(uiBegin <= uiEnd)
					if (uiEnd >= m_CurUse)
					{
						uiEnd = m_CurUse - 1;
					}
				if (uiBegin > uiEnd)
				{
					return;
				}
				unsigned int k;

				//中间删除之后要把后面的并到前面去
				unsigned int uiMoveNum = m_CurUse - 1 - uiEnd;
				unsigned int uiMoveIndex;
				//只执行析构函数，不销毁空间，把数组中间挖空
				if (ValueBase<T>::NeedsDestructor)
				{
					for (unsigned int uiDelete = uiBegin; uiDelete <= uiEnd; uiDelete++)
					{
						m_Buffer[uiDelete].~T();
					}
				}
				for (k = 0; k < uiMoveNum; k++)
				{
					uiMoveIndex = uiEnd + k + 1;
					if (uiMoveIndex < m_CurUse)
					{
						//同一块内存可以new多次，结果导致数据被override
						new(m_Buffer + uiBegin + k) T(m_Buffer[uiMoveIndex]);
						if (ValueBase<T>::NeedsDestructor)
						{
							m_Buffer[uiMoveIndex].~T();
						}
					}


				}
				m_CurUse -= ((uiEnd - uiBegin) + 1);
			};

			//传入的比较函数N要实现重载<=号
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
				T* pBuffer = MWDContainer<T, MMFun>::New(uiEnd - uiBegin + 1);
				for (unsigned int i = 0; i < uiEnd - uiBegin + 1; i++)
				{
					new(pBuffer + i) T();
				}
				MWDEngine::MergeSort(m_Buffer + uiBegin, pBuffer, uiEnd - uiBegin + 1, Compare);
				MWDContainer<T, MMFun>::Delete(pBuffer, uiEnd - uiBegin + 1);
			};
			void Sort(unsigned int uiBegin, unsigned int uiEnd) {
				if (uiEnd >= m_CurUse)
				{
					uiEnd = m_CurUse - 1;
				}
				if (uiBegin >= uiEnd)
				{
					return;
				}
				T* pBuffer = MWDContainer<T, MMFun>::New(uiEnd - uiBegin + 1);
				for (unsigned int i = 0; i < uiEnd - uiBegin + 1; i++)
				{
					new(pBuffer + i) T();
				}
				//被排序buffer，tmp_buffer,排序长度
				MWDEngine::MergeSort(m_Buffer + uiBegin, pBuffer, uiEnd - uiBegin + 1);
				//清空tmp_buffer
				MWDContainer<T, MMFun>::Delete(pBuffer, uiEnd - uiBegin + 1);
			};
			
			//找不到返回
			unsigned int FindElement(const T& Element) {
				for (unsigned int i = 0; i < m_CurUse; i++)
				{
					if (Element == m_Buffer[i])
					{
						return i;
					}
				}
				return GetNum();
			};
		protected:

			T* m_Buffer;
			
			//这个属性指的是当前使用了的个数,！！同时也指向了下一个空闲的位置！！
			unsigned int m_CurUse;

			//这个属性指的是当前Array总长度
			unsigned int m_BufferNum;

			//这个属性指的是当前Array被延长的次数
			unsigned int m_AllocNum;
			public:
			//这个属性指的是当前Array每次被延长的长度
			unsigned int m_GrowBy;


	};

	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDArrayOrder : public MWDArray<T>
	{
	public:
		MWDArrayOrder(unsigned int uiGrowBy = MWDArray<T>::DEFAULT_GROWBY):MWDArray<T>(uiGrowBy) {
			
		};
		~MWDArrayOrder() {};
		void AddElement(const T& Element) {
			//如果空间满了且不给延长，就返回
			if (MWDArray<T>::m_CurUse == MWDArray<T>::m_BufferNum)
			{
				if (!MWDArray<T>::m_GrowBy)
					return;
				MWDArray<T>::AddBufferNum(MWDArray<T>::m_GrowBy);
			}

			//升序排列的有序数组
			unsigned int uiIndex;
			if (MWDArray<T>::m_CurUse == 0)
			{
				uiIndex = 0;
			}
			else if (MWDArray<T>::m_CurUse == 1)
			{
				if (MWDArray<T>::m_Buffer[0] > Element)
				{
					uiIndex = 0;
				}
				else
				{
					uiIndex = 1;
				}
			}
			else if (MWDArray<T>::m_Buffer[0] > Element)
			{
				uiIndex = 0;
			}
			else if (Element > MWDArray<T>::m_Buffer[MWDArray<T>::m_CurUse - 1])
			{
				uiIndex = MWDArray<T>::m_CurUse;
			}
			else
			{
				//找到位次
				uiIndex = Process(0, MWDArray<T>::m_CurUse - 1, Element);
			}

			//接在末尾
			if (MWDArray<T>::m_CurUse == uiIndex)
			{
				new(MWDArray<T>::m_Buffer + uiIndex) T(Element);

			}
			//往后移动，留出空位给element
			else
			{
				//最后一个位置是没有实例化过的，所以先实例化，
				new(MWDArray<T>::m_Buffer + MWDArray<T>::m_CurUse) T(MWDArray<T>::m_Buffer[MWDArray<T>::m_CurUse - 1]);
				for (int i = (int)MWDArray<T>::m_CurUse - 2; i >= (int)uiIndex; i--)
				{
					MWDArray<T>::m_Buffer[i + 1] = MWDArray<T>::m_Buffer[i];//浅拷贝
				}
				MWDArray<T>::m_Buffer[uiIndex] = Element;
			}
			MWDArray<T>::m_CurUse++;
		};

		unsigned int FindElement(const T& Element) {
			if (MWDArray<T>::m_CurUse == 0)
			{
				return 0;
			}
			if (MWDArray<T>::m_Buffer[0] > Element)
			{
				return MWDArray<T>::m_CurUse;
			}
			else if (Element > MWDArray<T>::m_Buffer[MWDArray<T>::m_CurUse - 1])
			{
				return MWDArray<T>::m_CurUse;
			}
			else
			{
				return FindElement(0, MWDArray<T>::m_CurUse - 1, Element);
			}
		};
		FORCEINLINE T operator[] (unsigned int i)const { return MWDArray<T>::m_Buffer[i]; };
		//插入一个新的点之后重新排序成升序
		void ReSort(unsigned int i) {
			if (i >= MWDArray<T>::m_CurUse)
			{
				return;
			}
			if (MWDArray<T>::m_Buffer[i] < MWDArray<T>::m_Buffer[i - 1] && i)
			{
				T Temp = MWDArray<T>::m_Buffer[i];//浅拷贝，只有值传过去了
				unsigned int uiIndex = Process(0, i - 1, MWDArray<T>::m_Buffer[i]);
				for (int k = i - 1; k >= (int)uiIndex; k--)
				{
					MWDArray<T>::m_Buffer[k + 1] = MWDArray<T>::m_Buffer[k];
				}
				MWDArray<T>::m_Buffer[uiIndex] = Temp;
			}
			else if (MWDArray<T>::m_Buffer[i] > MWDArray<T>::m_Buffer[i + 1] && i != MWDArray<T>::m_CurUse - 1)
			{
				unsigned int uiIndex = Process(i + 1, MWDArray<T>::m_CurUse - 1, MWDArray<T>::m_Buffer[i]);
				T Temp = MWDArray<T>::m_Buffer[i];
				for (unsigned int k = i; k <= uiIndex - 1; k++)
				{
					MWDArray<T>::m_Buffer[k] = MWDArray<T>::m_Buffer[k + 1];
				}
				MWDArray<T>::m_Buffer[uiIndex] = Temp;
			}
		};
	protected:

		//在ReSort里使用的二分查找
		unsigned int Process(unsigned int uiIndex0, unsigned int uiIndex1, const T& Element) {
			//收敛1：↓
			if (uiIndex0 == uiIndex1)
			{
				return uiIndex0;
			}
			//收敛2：↓
			else if (uiIndex1 - uiIndex0 == 1)
			{
				if (Element < MWDArray<T>::m_Buffer[uiIndex0])
				{
					return uiIndex0;
				}
				else
				{
					return uiIndex1;
				}
			}
			else
			{
				//取中点（向下取整）
				unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
				//收敛3：↓
				if (MWDArray<T>::m_Buffer[uiIndex] == Element)
				{
					return uiIndex;
				}
				else if (MWDArray<T>::m_Buffer[uiIndex] > Element)
				{
					return Process(uiIndex0, uiIndex, Element);
				}
				else
				{
					return Process(uiIndex, uiIndex1, Element);
				}

			}
		};

		unsigned int FindElement(unsigned int uiIndex0, unsigned int uiIndex1, const T& Element) {
			if (uiIndex0 == uiIndex1)
			{
				if (MWDArray<T>::m_Buffer[uiIndex0] == Element)
				{
					return uiIndex0;
				}
				else
				{
					return MWDArray<T>::m_CurUse;
				}
			}
			else if (uiIndex1 - uiIndex0 == 1)
			{
				if (MWDArray<T>::m_Buffer[uiIndex0] == Element)
				{
					return uiIndex0;
				}
				else if (MWDArray<T>::m_Buffer[uiIndex1] == Element)
				{
					return uiIndex1;
				}
				else
				{
					return MWDArray<T>::m_CurUse;
				}
			}
			else
			{
				unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
				if (MWDArray<T>::m_Buffer[uiIndex] == Element)
				{
					return uiIndex;
				}
				else if (MWDArray<T>::m_Buffer[uiIndex] > Element)
				{
					return FindElement(uiIndex0, uiIndex, Element);
				}
				else
				{
					return FindElement(uiIndex, uiIndex1, Element);
				}

			}
		};
	};

}



