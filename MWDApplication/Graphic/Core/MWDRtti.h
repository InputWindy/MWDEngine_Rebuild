#pragma once
#include "MWDGraphic.h"
#include "MWDName.h"
#include <MWDMemManager.h>
#include <MWDArray.h>
#include "MWDProperty.h"
#include <iostream>

using namespace std;
namespace MWDEngine {
	class MWDObject;
	typedef MWDObject* (*CreateObjectFun)();
	class MWDProperty;
	class MWDFunction;
	//MWDRtti类是，类的信息包，包含类中的函数、属性信息。包括类的工厂函数
	class  MWDRtti : public MWDMemObject
	{
	public:
		friend class MWDObject;
		MWDRtti(const TCHAR* pcRttiName, MWDRtti* pBase, CreateObjectFun COF) {
			m_cRttiName = MWDName(pcRttiName);
			m_pBase = pBase;
			m_CreateFun = COF;
		};
		~MWDRtti() {
			MWDMAC_DELETE(m_pBase)
			m_CreateFun = NULL;
			m_PropertyArray.Destroy();
			m_FunctionArray.Destroy();
		};

		FORCEINLINE const MWDName& GetName()const { return m_cRttiName; };
		FORCEINLINE bool IsSameType(const MWDRtti& Type) const { return  (&Type == this); };
		FORCEINLINE bool IsDerived(const MWDRtti& Type) const {
			const MWDRtti* pTemp = this;
			while (!pTemp->IsSameType(Type))
			{
				if (pTemp->m_pBase)
				{
					pTemp = pTemp->m_pBase;
				}
				else
				{
					return 0;
				}
			}
			return 1;
		};
		FORCEINLINE MWDRtti* GetBase()const
		{
			return m_pBase;
		}
		FORCEINLINE unsigned int GetPropertyNum()const {
			return m_PropertyArray.GetNum();
		};
		FORCEINLINE unsigned int GetFunctionNum()const {
			return m_FunctionArray.GetNum();
		};

		//没找到返回NULL
		MWDProperty* GetProperty(unsigned int uiIndex)const {
			if (uiIndex >= m_PropertyArray.GetNum())
			{
				return NULL;
			}
			return m_PropertyArray[uiIndex];
		};
		MWDProperty* GetProperty(const MWDName& PropertyName)const {
			for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i]->GetName() == PropertyName)
				{
					return m_PropertyArray[i];
				}
			}
			return NULL;
		};
		MWDProperty* GetProperty(const TCHAR* PropertyName)const {
			for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i]->GetName() == PropertyName)
				{
					return m_PropertyArray[i];
				}
			}
			return NULL;
		}

		void AddProperty(MWDProperty* pProperty) {
			if (pProperty)
			{
				for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
				{
					MWDMAC_ASSERT(m_PropertyArray[i]->GetName() != pProperty->GetName());
				}
				m_PropertyArray.AddElement(pProperty);
			}
		};
		void AddProperty(MWDRtti& Rtti) {
			for (unsigned int i = 0; i < Rtti.m_PropertyArray.GetNum(); i++)
			{
				MWDProperty* pProperty = Rtti.m_PropertyArray[i];
				MWDProperty* pNewProperty = pProperty->GetInstance();
				pNewProperty->Clone(pProperty);
				pNewProperty->SetOwner(*this);
				m_PropertyArray.AddElement(pNewProperty);
			}
		};
		void ClearProperty() {
			/*for (unsigned int i = 0; i < m_PropertyArray.GetNum(); i++)
			{
				if (m_PropertyArray[i])
				{
					MWDMAC_DELETE(m_PropertyArray[i])
				}
			}*/
			m_PropertyArray.Clear();
		};
		
		MWDFunction* GetFunction(unsigned int uiIndex)const {
			if (uiIndex >= m_FunctionArray.GetNum())
			{
				return NULL;
			}
			return m_FunctionArray[uiIndex];
		};
		void AddFunction(MWDFunction* pFunction) {
			if (pFunction)
			{
				for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
				{
					if (m_FunctionArray[i]->IsSame(pFunction))
					{
						MWDMAC_DELETE(m_FunctionArray[i]);
						m_FunctionArray[i] = pFunction;
						return;
					}
				}
				m_FunctionArray.AddElement(pFunction);
			}
		};
		void AddFunction(MWDRtti& Rtti) {
			for (unsigned int i = 0; i < Rtti.m_FunctionArray.GetNum(); i++)
			{
				MWDFunction* pFunction = Rtti.m_FunctionArray[i];
				MWDFunction* pNewFunction = pFunction->GetInstance();
				pNewFunction->SetOwner(*this);
				pNewFunction->Clone(pFunction);
				m_FunctionArray.AddElement(pNewFunction);
			}
		};
		void ClearFunction() {
			/*for (unsigned int i = 0; i < m_FunctionArray.GetNum(); i++)
			{
				if (m_FunctionArray[i])
				{
					MWDMAC_DELETE(m_FunctionArray[i])
				}
			}*/
			m_FunctionArray.Clear();
		};

	private:
		//类名
		MWDName		m_cRttiName;

		//基类Rtti
		MWDRtti* m_pBase;
		
		//类工厂函数
		CreateObjectFun m_CreateFun;

		//存放属性列表
		MWDArray<MWDProperty*> m_PropertyArray;

		//存放函数列表
		MWDArray<MWDFunction*> m_FunctionArray;
	};
}


