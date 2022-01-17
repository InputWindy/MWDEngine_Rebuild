#pragma once
#include "MWDReference.h"
#include "MWDType.h"
#include <MWDMath.h>
//===========================================================
// 这个类的实例化本应该使用ResourceMManager，但是还没有实现
//===========================================================
namespace MWDEngine {

	//MWDName维护一个ID和一个name
	class  MWDName : public MWDReference, public MWDMemObject
	{
	public:
		~MWDName() {
			m_uiID = 0;
			m_String.Clear();
		};
		MWDName() {
			m_String = NULL;
			m_uiID = 0;
		}
		MWDName(const TCHAR* pChar) {
			MWDMAC_ASSERT(pChar);
			m_uiID = MWDNameToID((unsigned char*)pChar, sizeof(TCHAR) * MWDStrLen(pChar));
			m_String.Clear();
			m_String = pChar;
		};
		MWDName(const MWDString& String) {
			m_uiID = MWDNameToID((unsigned char*)String.GetBuffer(), sizeof(TCHAR) * MWDStrLen(String.GetBuffer()));
			m_String.Clear();
			m_String = String;
		};
		MWDName& operator =(const TCHAR* tchar) {
			if (!tchar) {
				return *this;
			}
			TCHAR* tmp = m_String.GetBuffer();
			*tmp = *tchar;
			m_uiID = MWDNameToID((unsigned char*)m_String.GetBuffer(), sizeof(TCHAR) * MWDStrLen(m_String.GetBuffer()));
			return *this;
		}
		MWDName& operator =(const MWDString& String) {
			MWDMAC_ASSERT(String.GetBuffer())
			m_String = String;
			m_uiID = MWDNameToID((unsigned char*)m_String.GetBuffer(), sizeof(TCHAR)*MWDStrLen(m_String.GetBuffer()));
			return *this;
		}
		MWDName& operator =(const MWDName& name) {
			MWDMAC_ASSERT(name.GetString().GetBuffer())
			m_String = name.GetString();
			m_uiID = name.GetID();
			return *this;
		}
		//获取const TCHAR*类型的name
		FORCEINLINE const TCHAR* GetBuffer()const
		{
			return m_String.GetBuffer();
		}
		//获取const MWDString&类型的name
		FORCEINLINE const MWDString& GetString()const
		{
			return m_String;
		}
		char* MWDNameToChar() {
			return MWDString::TCHARToChar(GetBuffer());
		}
		//获取字符串长度
		FORCEINLINE unsigned int GetLength()const
		{
			return m_String.GetLength();
		}
		//获取ID
		FORCEINLINE unsigned int GetID()const
		{
			return m_uiID;
		}
		FORCEINLINE void SetID(unsigned int&id)
		{
			m_uiID = id;
		}
		 friend bool operator >(const MWDName& Name1, const MWDName& Name2) {
			if (!Name1.GetID() && !Name2.GetID())
			{
				return 0;
			}
			else if (Name1.GetID() && !Name2.GetID())
			{
				return 1;
			}
			else if (!Name1.GetID() && Name2.GetID())
			{
				return 0;
			}
			return Name1.GetID() > Name2.GetID();
		};
		 friend bool operator <(const MWDName& Name1, const MWDName& Name2) {
			if (!Name1.GetID() && !Name2.GetID())
			{
				return 0;
			}
			else if (Name1.GetID() && !Name2.GetID())
			{
				return 0;
			}
			else if (!Name1.GetID() && Name2.GetID())
			{
				return 1;
			}
			return Name1.GetID() < Name2.GetID();
		};
		 friend bool operator ==(const MWDName& Name1, const MWDName& Name2) {
			if (!Name1.GetID() && !Name2.GetID())
			{
				return 1;
			}
			else if (!Name1.GetID() || !Name2.GetID())
			{
				return 0;
			}
			return Name1.GetID() == Name2.GetID();
		};
		 friend bool operator !=(const MWDName& Name1, const MWDName& Name2) {
			if (!Name1.GetID() && !Name2.GetID())
			{
				return 0;
			}
			else if (!Name1.GetID() || !Name2.GetID())
			{
				return 1;
			}
			return Name1.GetID() != Name2.GetID();
		};
		 friend bool operator ==(const MWDName& Name, const MWDString& String) {
			if (!Name.GetID())
			{
				return 0;
			}
			return Name.GetString() == String;
		};
		 friend bool operator !=(const MWDName& Name, const MWDString& String) {
			if (!Name.GetID())
			{
				return 1;
			}
			return Name.GetString() != String;
		};
		 friend bool operator ==(const MWDName& Name, const TCHAR* pChar) {
			if (!Name.GetID() && !pChar)
			{
				return 1;
			}
			else if (!Name.GetID() || !pChar)
			{
				return 0;
			}
			return Name.GetString() == pChar;
		};
		 friend bool operator !=(const MWDName& Name, const TCHAR* pChar) {
			if (!Name.GetID() && !pChar)
			{
				return 0;
			}
			else if (!Name.GetID() || !pChar)
			{
				return 1;
			}
			return Name.GetString() != pChar;
		};
		void Print()const {
			m_String.Print();
		}

	protected:

		unsigned int m_uiID;
		MWDString  m_String;

	};
}

