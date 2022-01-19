#pragma once
#include <iostream>
#include "MWDContainer.h"
using namespace std;
namespace MWDEngine {
	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDArray:public MWDContainer<T, MMFun>
	{
		//�䳤����û�취����ǳ��������Ϊ�洢�Ķ���������ͬ��ֻ���������ȫ����գ���ȫ��ʵ����
		public:
			//����䳤����
			enum
			{
				DEFAULT_GROWBY = 10
			};
			//ʵ������ʱ��Arrayʱ�����鳤��Ϊ0
			MWDArray(unsigned int uiGrowBy = MWDArray::DEFAULT_GROWBY) {
				m_GrowBy = uiGrowBy;
				m_Buffer = NULL;
				m_CurUse = 0;
				m_BufferNum = 0;
				m_AllocNum = 0;
			};
			//�����������ᴴ��һ���µĶ���
			MWDArray(const MWDArray& Array) {
				m_GrowBy = Array.m_GrowBy;
				m_Buffer = NULL;
				m_CurUse = 0;
				m_BufferNum = 0;
				m_AllocNum = 0;
				//��һ��ֻ�ǰ�Array������ݸ���*this
				*this = Array;
			};
			
			//�������
			void operator= (const MWDArray<T, MMFun>& Array) {
				//����������������ȵ�ǰ��������Сʱ����������������ֱ����д�ڵ�ǰArray���ڴ�ռ�
				//GetNum���ص���m_CurUse
				if (m_BufferNum >= Array.GetNum())
				{
					Clear();
					m_CurUse = Array.GetNum();
					T* pBuffer = Array.GetBuffer();
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						//========================================
						// placement new:
						//		��˼����m_Buffer + i��ַ��newһ��T
						//========================================
						new(m_Buffer + i) T(pBuffer[i]);
					}
				}
				else
				{
					//�������յ�ǰArray���ڴ�ռ䣬���¹��쵱ǰArray
					MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
					m_BufferNum = Array.GetBufferNum();

					m_CurUse = Array.GetNum();
					//��Array���·���ռ�
					m_Buffer = MWDContainer<T, MMFun>::New(m_BufferNum);
					if (!m_Buffer)
						return;

					//��ȡԭArray��洢�Ķ���
					T* pBuffer = Array.GetBuffer();
					for (unsigned int i = 0; i < m_CurUse; i++)
					{
						//��ԭArray��Ķ���ȫ������ʵ����һ��
						//PS:�κ��඼�и������Ĭ�Ϲ��캯�����������һ��ʵ����new��������һ���µ�ʵ��
						new(m_Buffer + i) T(pBuffer[i]);
					}
				}
			};
			void operator+= (const MWDArray<T, MMFun>& Array) {
				AddElement(Array, 0, Array.GetNum());
			};
			~MWDArray() {
				//���Array���Ķ���
				MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
				m_BufferNum = 0;
				m_CurUse = 0;
				m_AllocNum = 0;
			};
			
			//��������ᵼ�����ݶ�ʧ����յ�ǰ������,ʵ����uiBufferNum���յ�T
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
						//SetBufferNumֱ�����ݶ�ʧ
						new(m_Buffer + i) T();
					}
				}
			};
			void AddBufferNum(unsigned int uiBufferNum) {
				if (uiBufferNum)
				{
					m_AllocNum++;
					//ÿ���ӳ�һ�Σ��ӳ����Ⱦ�Ҫ����
					m_GrowBy = m_AllocNum * m_GrowBy;

					T* pBuffer = NULL;
					//�����ܿռ�
					pBuffer = MWDContainer<T, MMFun>::New(m_BufferNum + uiBufferNum);
					if (!pBuffer)
						return;

					if (m_Buffer && m_CurUse)
					{
						for (unsigned int i = 0; i < m_CurUse; i++)
						{
							//��ԭArray��Ķ����������µ�pBuffer��
							new(pBuffer + i) T(m_Buffer[i]);
						}
					}
					//��ԭ���������������
					MWDContainer<T, MMFun>::Delete(m_Buffer, m_CurUse);
					m_BufferNum += uiBufferNum;
					//���������滻���µ�����
					m_Buffer = pBuffer;
				}
			};
			//��ǰԪ�ظ���
			FORCEINLINE unsigned int GetNum()const { return m_CurUse; };
			FORCEINLINE unsigned int GetBufferNum()const { return m_BufferNum; };

			FORCEINLINE T* GetBuffer()const { return m_Buffer; };
			FORCEINLINE void SetGrowBy(unsigned int uiGrowBy) { cout << "ִ����MWDArray.h::SetGrowBy=" << uiGrowBy << endl; m_GrowBy = uiGrowBy; };

			//��������Arrayռ�õ����ֽ�������������ֵ+uiNum������T�ռ�
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
				//�޶�������ĩβ
				if (uiEnd >= Array.GetNum())
				{
					uiEnd = Array.GetNum() - 1;
				}
				//�������������˵�
				int iAddNum = uiEnd - uiBegin + 1;
				//ʣ��ռ�
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
						//������鲻�������ӳ��Ļ�����ֻ�ܰ��������������Ĳ��ֶ�ʧ
						iAddNum = iHaveNum;
					}
				}
				//���ֻ���һ��Ԫ��
				if (iAddNum == 1)
				{
					AddElement(Array[uiBegin]);
				}
				//���ֻ���������Ԫ��
				else if (iAddNum == 2)
				{
					AddElement(Array[uiBegin]);
					AddElement(Array[uiBegin + 1]);
				}
				else
				{
					//����1
					N* pBuffer = Array.GetBuffer();
					for (unsigned int i = m_CurUse; i < (unsigned int)iAddNum + m_CurUse; i++)
					{
						new(m_Buffer + i) T(pBuffer[uiBegin]);
						uiBegin++;
					}
					m_CurUse += iAddNum;
					//����2
					//MWDMemcpy(&m_Buffer[m_CurUse],&Array.GetBuffer()[uiBegin], sizeof(T) * iAddNum);
				}
			};
			
			void AddElement(const T& Element) {
				//cout << "����ˣ�" << endl;
				//����ռ䲻���Ϳ��µĿռ�
				if (m_CurUse == m_BufferNum)
				{
					if (!m_GrowBy) {
						cout << "MWDArray.h::AddElement:m_GrowBy==0" <<"______"<< "m_GrowBy=" << m_GrowBy << endl;;
						return;
					}
						
					AddBufferNum(m_GrowBy);

				}
				//�ռ��㹻��ֱ������һ������λ�ÿ���һ����Ԫ��
				new(m_Buffer + m_CurUse) T(Element);
				m_CurUse++;
			};

			FORCEINLINE T& operator[] (unsigned int i)const { return m_Buffer[i]; };

			//�����������������������û���ͷ��ڴ�
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
			//�����������ݲ����ͷ��ڴ�(��������)
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

				//�м�ɾ��֮��Ҫ�Ѻ���Ĳ���ǰ��ȥ
				unsigned int uiMoveNum = m_CurUse - 1 - uiEnd;
				unsigned int uiMoveIndex;
				//ִֻ�����������������ٿռ䣬�������м��ڿ�
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
						//ͬһ���ڴ����new��Σ�����������ݱ�override
						new(m_Buffer + uiBegin + k) T(m_Buffer[uiMoveIndex]);
						if (ValueBase<T>::NeedsDestructor)
						{
							m_Buffer[uiMoveIndex].~T();
						}
					}


				}
				m_CurUse -= ((uiEnd - uiBegin) + 1);
			};

			//����ıȽϺ���NҪʵ������<=��
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
				//������buffer��tmp_buffer,���򳤶�
				MWDEngine::MergeSort(m_Buffer + uiBegin, pBuffer, uiEnd - uiBegin + 1);
				//���tmp_buffer
				MWDContainer<T, MMFun>::Delete(pBuffer, uiEnd - uiBegin + 1);
			};
			
			//�Ҳ�������
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
			
			//�������ָ���ǵ�ǰʹ���˵ĸ���,����ͬʱҲָ������һ�����е�λ�ã���
			unsigned int m_CurUse;

			//�������ָ���ǵ�ǰArray�ܳ���
			unsigned int m_BufferNum;

			//�������ָ���ǵ�ǰArray���ӳ��Ĵ���
			unsigned int m_AllocNum;
			public:
			//�������ָ���ǵ�ǰArrayÿ�α��ӳ��ĳ���
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
			//����ռ������Ҳ����ӳ����ͷ���
			if (MWDArray<T>::m_CurUse == MWDArray<T>::m_BufferNum)
			{
				if (!MWDArray<T>::m_GrowBy)
					return;
				MWDArray<T>::AddBufferNum(MWDArray<T>::m_GrowBy);
			}

			//�������е���������
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
				//�ҵ�λ��
				uiIndex = Process(0, MWDArray<T>::m_CurUse - 1, Element);
			}

			//����ĩβ
			if (MWDArray<T>::m_CurUse == uiIndex)
			{
				new(MWDArray<T>::m_Buffer + uiIndex) T(Element);

			}
			//�����ƶ���������λ��element
			else
			{
				//���һ��λ����û��ʵ�������ģ�������ʵ������
				new(MWDArray<T>::m_Buffer + MWDArray<T>::m_CurUse) T(MWDArray<T>::m_Buffer[MWDArray<T>::m_CurUse - 1]);
				for (int i = (int)MWDArray<T>::m_CurUse - 2; i >= (int)uiIndex; i--)
				{
					MWDArray<T>::m_Buffer[i + 1] = MWDArray<T>::m_Buffer[i];//ǳ����
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
		//����һ���µĵ�֮���������������
		void ReSort(unsigned int i) {
			if (i >= MWDArray<T>::m_CurUse)
			{
				return;
			}
			if (MWDArray<T>::m_Buffer[i] < MWDArray<T>::m_Buffer[i - 1] && i)
			{
				T Temp = MWDArray<T>::m_Buffer[i];//ǳ������ֻ��ֵ����ȥ��
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

		//��ReSort��ʹ�õĶ��ֲ���
		unsigned int Process(unsigned int uiIndex0, unsigned int uiIndex1, const T& Element) {
			//����1����
			if (uiIndex0 == uiIndex1)
			{
				return uiIndex0;
			}
			//����2����
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
				//ȡ�е㣨����ȡ����
				unsigned int uiIndex = (uiIndex0 + uiIndex1) >> 1;
				//����3����
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



