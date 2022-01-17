#pragma once
#include "MWDSystem.h"
#include "MWDMemManager.h"
#include <iostream>
using namespace std;
namespace MWDEngine{
	template <typename T>
	class  MWDSingleton
	{
	private:
		static T* m_pSingleton;
		MWDSingleton()
		{
			//MWDMAC_ASSERT(!m_pSingleton);
			/*if (!m_pSingleton)
			{
				cout << "empty" << endl;
				
			}*/
			//m_pSingleton = static_cast<T*>();
		}

		~MWDSingleton()
		{
			//MWDMAC_ASSERT(m_pSingleton);
			m_pSingleton = NULL;
		}
	public:
		
		static T& GetSingleton()
		{
			if (!m_pSingleton) {
				m_pSingleton = new T();
			}
			return (*m_pSingleton);
		}
		static T* GetSingletonPtr()
		{
			if (!m_pSingleton) {
				m_pSingleton = new T();
			}
			return (m_pSingleton);
		}
	};

	template <typename T>
	T* MWDSingleton<T>::m_pSingleton = NULL;
}
