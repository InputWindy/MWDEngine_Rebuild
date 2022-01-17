#pragma once
#include <MWDSynchronize.h>
#include <iostream>
using namespace std;
namespace MWDEngine{
	template <class T>
	class MWDPointer
	{
	public:
		MWDPointer(T* pObject = 0) {
			SetObject(pObject);
			if (pObject) {
				pObject->IncreRef();
			}
		};
		MWDPointer(const MWDPointer& rPointer) {
			m_pObject = rPointer.m_pObject;
			if (m_pObject)
			{
				m_pObject->IncreRef();

			}
		};
		~MWDPointer() {
			if (m_pObject)
			{
				m_pObject->DecreRef();
				m_pObject = NULL;
			}
		};


		T& operator* () const { return *m_pObject; };
		T* operator-> () const { return m_pObject; };

		MWDPointer& operator= (T* pObject) {
			if (m_pObject != pObject)
			{
				//被指向的obj++，不被指向的obj--
				if (pObject)
				{
					pObject->IncreRef();
				}

				if (m_pObject)
				{
					m_pObject->DecreRef();
				}

				m_pObject = pObject;
			}
			return *this;
		};
		MWDPointer& operator= (const MWDPointer& rPointer) {
			if (m_pObject != rPointer.m_pObject)
			{
				if (rPointer.m_pObject)
				{
					rPointer.m_pObject->IncreRef();
				}

				if (m_pObject)
				{
					m_pObject->DecreRef();
				}

				m_pObject = rPointer.m_pObject;
			}
			return *this;
		};

		bool operator== (T* pObject) const {
			return (m_pObject == pObject);
		};
		bool operator!= (T* pObject) const {
			return (m_pObject != pObject);
		};
		bool operator== (const MWDPointer& rReference) const {
			return (m_pObject== rReference.m_pObject);
		};
		bool operator!= (const MWDPointer& rReference) const {
			return (m_pObject != rReference.m_pObject);
		};

		//C++特性之隐式类型转换：智能指针自动转换成普通指针
		operator T* ()const {
			return m_pObject;
		};

		FORCEINLINE T* GetObject()const
		{
			return m_pObject;
		}

		//MWDStream可以随意使用MWDPointer
		friend class MWDStream;
		bool IsNULL(){
			if (m_pObject) {
				return true;
			}
			return false;
		}
	private:
		//只能指针维护一个T*指针
		T* m_pObject;
		FORCEINLINE void SetObject(T* pObject)
		{
			m_pObject = pObject;
		}
		
	};

}
