#pragma once
#include "MWDArray.h"
namespace MWDEngine {
	template <class T, MWDMemManagerFun MMFun = MWDMemObject::GetMemManager>
	class MWDDATASTRUCT_API MWDStack{
	public:
		MWDStack() {};
		~MWDStack() {};
		bool GetTop(T& Element) {
			if (m_Array.GetNum() == 0)
			{
				return false;
			}
			Element = m_Array[m_Array.GetNum() - 1];
			return true;
		};
		FORCEINLINE void Push(const T& Element) {
			m_Array.AddElement(Element);
		};
		bool Pop(T& Element) {
			if (m_Array.GetNum() == 0)
			{
				return false;
			}
			Element = m_Array[m_Array.GetNum() - 1];
			m_Array.Erase(m_Array.GetNum() - 1);
			return true;
		};
		FORCEINLINE unsigned int GetNum() {
			return m_Array.GetNum();
		};
	protected:
		MWDArray<T, MMFun> m_Array;
	};
}
