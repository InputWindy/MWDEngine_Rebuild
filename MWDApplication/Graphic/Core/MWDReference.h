#pragma once
#include "MWDGraphic.h"
#include "MWDPointer.h"
#include <MWDMemManager.h>
#include <iostream>
using namespace std;
namespace MWDEngine {
	class  MWDReference
	{
	public:
		template <class T>
		friend class MWDPointer;
		friend class MWDVariant;
		MWDReference() { m_iReference = 0; };
		virtual ~MWDReference() { m_iReference = 0; };
		//SmartPointer
	public:
		int GetRef()const { return m_iReference; }
	protected:
		void IncreRef()
		{
			MWDLockedIncrement((long*)&m_iReference);
			//m_iReference++;
		}
		virtual void DecreRef()
		{
			//m_iReference--; 
			MWDLockedDecrement((long*)&m_iReference);
			if (!m_iReference)
				delete this;
		}
		//维护一个智能指针次数，归零的时候就销毁obj
		int m_iReference;
	};
}
