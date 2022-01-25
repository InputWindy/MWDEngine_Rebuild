#pragma once
#include <iostream>
using namespace std;
#include "MWDContainer.h"
namespace MWDEngine {
#define TcharToChar(pTchar) \
	MWDString::TCHARToChar(pTchar)

#define CharToTchar(pChar) \
	MWDString::CharToTCHAR(pChar)


	class MWDDATASTRUCT_API MWDString
	{
	public:
		enum
		{
			BUFFER_SIZE = 2048
		};
		static inline MWDString* ms_StringNULL = NULL;
		static inline TCHAR TempBuffer[BUFFER_SIZE] = {0};
	protected:
		TCHAR* m_Buffer;
	public :
	#pragma region ДњТы
		MWDString() {
			m_Buffer = NULL;
			m_Buffer = new TCHAR[1];
			MWDMAC_ASSERT(m_Buffer);
			m_Buffer[0] = _T('\0');
		};
		MWDString(const TCHAR* String) {
			MWDMAC_ASSERT(String);
			m_Buffer = NULL;
			if (String)
			{
				unsigned int  uiLength = (unsigned int)MWDStrLen(String);
				m_Buffer = new TCHAR[uiLength + 1];
				MWDMAC_ASSERT(m_Buffer);

				MWDStrCopy(m_Buffer, uiLength + 1, String);
			}
			else
			{
				m_Buffer = new TCHAR[1];
				MWDMAC_ASSERT(m_Buffer);
				m_Buffer[0] = _T('\0');
			}
		};
		MWDString(const MWDString& String) {
			m_Buffer = NULL;
			unsigned int  uiLength = String.GetLength();
			m_Buffer = new TCHAR[uiLength + 1];
			MWDMAC_ASSERT(m_Buffer);

			MWDStrCopy(m_Buffer, uiLength + 1, String.GetBuffer());
		};
		~MWDString() {
			MWDMAC_DELETEA(m_Buffer);
		};

		FORCEINLINE TCHAR* GetBuffer()const
		{
			return m_Buffer;
		}
		char* MWDStringToChar() {
			return MWDString::TCHARToChar(GetBuffer());
		}
		FORCEINLINE unsigned int GetLength()const
		{
			return (unsigned int)MWDStrLen(m_Buffer);
		}
		void SetTCHARBufferNum(unsigned int uiLength) {
			MWDMAC_DELETEA(m_Buffer);
			m_Buffer = new TCHAR[uiLength + 1];
			m_Buffer[uiLength] = _T('\0');
		};
		void Clear() {
			MWDMAC_DELETEA(m_Buffer);
			m_Buffer = new TCHAR[1];
			MWDMAC_ASSERT(m_Buffer);
			m_Buffer[0] = _T('\0');
		};

		MWDString& operator =(const MWDString& String) {
			MWDMAC_DELETEA(m_Buffer);
			unsigned int  uiLength = String.GetLength();
			m_Buffer = new TCHAR[uiLength + 1];
			MWDMAC_ASSERT(m_Buffer);

			MWDStrCopy(m_Buffer, uiLength + 1, String.GetBuffer());
			return *this;
		};
		MWDString& operator =(const TCHAR* String) {
			//MWDMAC_ASSERT(String);
			if (!String) {
				return *this;
			}
			MWDMAC_DELETEA(m_Buffer);
			if (String)
			{
				unsigned int  uiLength = (unsigned int)MWDStrLen(String);
				m_Buffer = new TCHAR[uiLength + 1];
				MWDMAC_ASSERT(m_Buffer);

				MWDStrCopy(m_Buffer, uiLength + 1, String);
			}
			return *this;
		};
		void Print() const{
			for (int i = 0; i < GetLength(); i++) {
				cout << (char)m_Buffer[i];
			}
			cout << endl;
		}
		TCHAR& operator[](unsigned int i) {
			return m_Buffer[i];
		};

		const MWDString& operator +=(const MWDString& String) {
			unsigned int uiLength1 = GetLength();
			unsigned int uiLength2 = String.GetLength();
			if (!uiLength1)
			{
				*this = String;
				return *this;
			}
			else if (!uiLength2)
				return *this;
			else
			{
				TCHAR* pBuffer = NULL;
				unsigned int uiLength = uiLength2 + uiLength1 + 1;
				pBuffer = new TCHAR[uiLength];
				MWDMAC_ASSERT(pBuffer);

				MWDStrCopy(pBuffer, uiLength, GetBuffer());
				MWDStrcat(pBuffer, uiLength, String.GetBuffer());

				MWDMAC_DELETEA(m_Buffer);
				m_Buffer = pBuffer;
				return *this;

			}
		};
		const MWDString& operator +=(const TCHAR* String) {
			if (!String) {
				return *this;
			}
			unsigned int uiLength1 = GetLength();
			unsigned int uiLength2 = 0;
			if (String)
			{
				uiLength2 = (unsigned int)MWDStrLen(String);
			}
			if (!uiLength1)
			{
				*this = String;
				return *this;
			}
			else if (!uiLength2)
				return *this;
			else
			{
				TCHAR* pBuffer = NULL;
				unsigned int uiLength = uiLength2 + uiLength1 + 1;
				pBuffer = new TCHAR[uiLength];
				MWDMAC_ASSERT(pBuffer);

				MWDStrCopy(pBuffer, uiLength, GetBuffer());
				MWDStrcat(pBuffer, uiLength, String);

				MWDMAC_DELETEA(m_Buffer);
				m_Buffer = pBuffer;
				return *this;

			}
		};

		bool GetString(const MWDString& String, unsigned int uiFind, bool bIsFront = true, bool bIsHaveFind = false) {
			if (uiFind >= String.GetLength())
			{
				return false;
			}

			unsigned int uiLength = 0;
			unsigned int uiIndex = 0;
			if (bIsFront)
			{
				uiLength = uiFind;
				uiIndex = 0;
			}
			else
			{
				uiLength = String.GetLength() - uiFind - 1;
				uiIndex = uiFind + 1;
			}
			if (bIsHaveFind)
			{
				uiLength++;
				if (!bIsFront)
				{
					uiIndex--;
				}
			}
			if (uiIndex >= String.GetLength())
			{
				return false;
			}
			MWDMAC_DELETEA(m_Buffer);
			m_Buffer = new TCHAR[uiLength + 1];
			MWDMemcpy(m_Buffer, &String.m_Buffer[uiIndex], uiLength, uiLength + 1);
			m_Buffer[uiLength] = _T('\0');
			return true;
		};
		bool GetString(const MWDString& String, TCHAR cFind, int iFIndNum, bool bIsFront = true, bool bIsHaveFind = false) {
			if (iFIndNum == 0)
			{
				return false;
			}
			unsigned int uiFIndNumTemp = 0;
			int uiIndex = -1;
			for (unsigned int i = 0; i < String.GetLength(); i++)
			{
				if (cFind == String.m_Buffer[i])
				{
					uiFIndNumTemp++;
					uiIndex = i;
				}
				if (uiFIndNumTemp == iFIndNum)
				{
					break;
				}
			}
			if (uiIndex == -1)
			{
				return false;
			}
			return GetString(String, uiIndex, bIsFront, bIsHaveFind);
		};

		int GetSubStringIndex(const MWDString& String, int iFindNum)const {
			if (GetLength() < String.GetLength())
			{
				return -1;
			}
			unsigned int uiFindNumTemp = 0;
			int iFindIndex = -1;
			for (unsigned int Index = 0; Index < GetLength(); Index++)
			{
				unsigned int j = 0;
				for (unsigned int i = Index; i < String.GetLength() + Index; i++)
				{
					if (m_Buffer[i] == String.m_Buffer[j])
					{
						j++;
						continue;
					}
					else
					{
						break;
					}
				}
				if (j == String.GetLength())
				{
					iFindIndex = Index;
					uiFindNumTemp++;
					if (uiFindNumTemp == iFindNum)
					{

						return iFindIndex;
					}
				}
			}

			return iFindIndex;
		};

		bool StripChars(const MWDString& String, TCHAR* pStripChars) {
			if (String.GetLength() > BUFFER_SIZE || !pStripChars)
			{
				return false;
			}

			unsigned int strip_chars_len = MWDStrLen(pStripChars);
			TCHAR* _string_in = String.GetBuffer(), * _string_out = TempBuffer;
			while (*_string_in != _T('\0'))
			{
				int flag = 0;
				for (unsigned int i = 0; i < strip_chars_len; i++)
				{
					if (*_string_in == pStripChars[i])
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0)
				{
					*_string_out = *_string_in;
					_string_out++;


				}
				_string_in++;

			}
			*_string_out = _T('\0');
			*this = TempBuffer;
			return true;
		};

		bool ReplaceChars(const MWDString& String, TCHAR* pReplaceChars, TCHAR UseChar) {
			if (String.GetLength() > BUFFER_SIZE || !pReplaceChars)
			{
				return false;
			}
			unsigned int replace_chars_len = MWDStrLen(pReplaceChars);
			TCHAR* _string_in = String.GetBuffer(), * _string_out = TempBuffer;
			while (*_string_in != _T('\0'))
			{
				int flag = 0;
				for (unsigned int i = 0; i < replace_chars_len; i++)
				{
					if (*_string_in == pReplaceChars[i])
					{
						flag = 1;
						break;
					}
				}
				if (flag == 0)
				{
					*_string_out = *_string_in;

				}
				else
				{
					*_string_out = UseChar;

				}
				_string_out++;
				_string_in++;

			}
			*_string_out = _T('\0');

			*this = TempBuffer;
			return true;
		};

		void StringLtrim(const MWDString& String) {
			int index = 0;
			TCHAR* pString = String.GetBuffer();
			while (_istspace(pString[index]))
				index++;
			if (index == 0)
				return;
			else
			{
				unsigned int uiLength = String.GetLength();
				MWDMemcpy((void*)TempBuffer, (void*)(pString + index), ((uiLength - index) + 1) * sizeof(TCHAR));
				*this = TempBuffer;
			}
		};

		void StringRtrim(const MWDString& String) {
			unsigned int uiLength = String.GetLength();
			if (!uiLength)
			{
				return;
			}
			unsigned int index = uiLength - 1;
			TCHAR* pString = String.GetBuffer();
			while (_istspace(pString[index]))
				index--;

			MWDMemcpy((void*)TempBuffer, (void*)(pString), (index + 1) * sizeof(TCHAR));
			index++;
			TempBuffer[index] = _T('\0');
			*this = TempBuffer;
		};

		void Format(const TCHAR* pcString, ...) {
			static TCHAR LogBuffer[LOG_BUFFER_SIZE];
			char* pArgs;
			pArgs = (char*)&pcString + sizeof(pcString);
			MWDSprintf(LogBuffer, LOG_BUFFER_SIZE, pcString, pArgs);
			*this = LogBuffer;
		};

		MWDDATASTRUCT_API friend MWDString operator +(const MWDString& String1, const MWDString& String2) {
			unsigned int uiLength1 = String1.GetLength();
			unsigned int uiLength2 = String2.GetLength();
			if (!uiLength2)
				return String1;
			else if (!uiLength1)
				return String2;
			else
			{
				TCHAR* pBuffer = NULL;
				unsigned int uiLength = uiLength2 + uiLength1 + 1;
				pBuffer = new TCHAR[uiLength];
				MWDMAC_ASSERT(pBuffer);

				MWDStrCopy(pBuffer, uiLength, String1.GetBuffer());
				MWDStrcat(pBuffer, uiLength, String2.GetBuffer());
				MWDString ReturnValue(pBuffer);
				MWDMAC_DELETEA(pBuffer);
				return ReturnValue;
			}
		};
		MWDDATASTRUCT_API friend MWDString operator +(const MWDString& String1, const TCHAR* String2) {
			MWDMAC_ASSERT(String2);
			unsigned int uiLength1 = String1.GetLength();
			unsigned int uiLength2 = 0;
			if (String2)
			{
				uiLength2 = (unsigned int)MWDStrLen(String2);
			}

			if (!uiLength2)
				return String1;
			else if (!uiLength1)
				return MWDString(String2);
			else
			{
				TCHAR* pBuffer = NULL;
				unsigned int uiLength = uiLength2 + uiLength1 + 1;
				pBuffer = new TCHAR[uiLength];
				MWDMAC_ASSERT(pBuffer);

				MWDStrCopy(pBuffer, uiLength, String1.GetBuffer());
				MWDStrcat(pBuffer, uiLength, String2);
				MWDString ReturnValue(pBuffer);
				MWDMAC_DELETEA(pBuffer);
				return ReturnValue;
			}
		};
		MWDDATASTRUCT_API friend MWDString operator +(const TCHAR* String1, const MWDString& String2) {
			MWDMAC_ASSERT(String1);
			unsigned int uiLength2 = String2.GetLength();
			unsigned int uiLength1 = 0;
			if (String1)
			{
				uiLength1 = (unsigned int)MWDStrLen(String1);
			}

			if (!uiLength1)
				return String2;
			else if (!uiLength2)
				return MWDString(String1);
			else
			{
				TCHAR* pBuffer = NULL;
				unsigned int uiLength = uiLength2 + uiLength1 + 1;
				pBuffer = new TCHAR[uiLength];
				MWDMAC_ASSERT(pBuffer);

				MWDStrCopy(pBuffer, uiLength, String1);
				MWDStrcat(pBuffer, uiLength, String2.GetBuffer());
				MWDString ReturnValue(pBuffer);
				MWDMAC_DELETEA(pBuffer);
				return ReturnValue;
			}
		};

		MWDDATASTRUCT_API friend int CompareString(const MWDString& String1, const MWDString& String2) {
			return MWDStrCmp(String1.GetBuffer(), String2.GetBuffer());
		};
		MWDDATASTRUCT_API friend int CompareString(const MWDString& String1, const TCHAR* String2) {
			return MWDStrCmp(String1.GetBuffer(), String2);
		};
		MWDDATASTRUCT_API friend int CompareString(const TCHAR* String1, const MWDString& String2) {
			return MWDStrCmp(String1, String2.GetBuffer());
		};

		MWDDATASTRUCT_API friend bool operator ==(const MWDString& String1, const MWDString& String2) {
			return (MWDStrCmp(String1.GetBuffer(), String2.GetBuffer()) == 0);
		};
		MWDDATASTRUCT_API friend bool operator ==(const MWDString& String1, const TCHAR* String2) {
			return (MWDStrCmp(String1.GetBuffer(), String2) == 0);
		};
		MWDDATASTRUCT_API friend bool operator ==(const TCHAR* String1, MWDString& String2) {
			return (MWDStrCmp(String1, String2.GetBuffer()) == 0);
		};

		MWDDATASTRUCT_API friend bool operator !=(const MWDString& String1, const MWDString& String2) {
			return (MWDStrCmp(String1.GetBuffer(), String2.GetBuffer()) != 0);
		};
		MWDDATASTRUCT_API friend bool operator !=(const MWDString& String1, const TCHAR* String2) {
			return (MWDStrCmp(String1.GetBuffer(), String2) != 0);
		};
		MWDDATASTRUCT_API friend bool operator !=(const TCHAR* String1, const MWDString& String2) {
			return (MWDStrCmp(String1, String2.GetBuffer()) != 0);
		};
		static int StringToInt(const MWDString& String) {
			int i = 0;
			MWDScanf(String.GetBuffer(), _T("%d"), (char*)&i);
			return i;
		};
		static MWDString IntToString(int Value) {
			MWDSprintf(MWDString::TempBuffer, 2048, _T("%d"), Value);
			return MWDString(MWDString::TempBuffer);
		};

		static unsigned int StringToUInt(const MWDString& String) {
			unsigned int i = 0;
			MWDScanf(String.GetBuffer(), _T("%d"), (char*)&i);
			return i;
		};
		static MWDString UIntToString(unsigned int Value) {
			MWDSprintf(MWDString::TempBuffer, 2048, _T("%d"), Value);
			return MWDString(MWDString::TempBuffer);
		};

		static bool StringToBool(const MWDString& String) {
			if (String == _T("True") || String == _T("true") || String == _T("TRUE"))
			{
				return true;
			}
			else if (String == _T("False") || String == _T("false") || String == _T("FALSE"))
			{
				return false;
			}

			int d = 0;
			_stscanf_s(String.GetBuffer(), _T("%d"), &d);
			if (d)
			{
				return true;
			}
			else
				return false;
		};
		static MWDString BoolToString(bool i) {
			if (i)
			{
				return _T("True");
			}
			else
			{
				return _T("False");
			}
		};
		static MWDString BoolToString(unsigned int i) {
			if (i)
			{
				return _T("True");
			}
			else
			{
				return _T("False");
			}
		};

		static float StringToReal(const MWDString& String) {
			float f = 0.0f;
			_stscanf_s(String.GetBuffer(), _T("%f"), &f);
			//MWDScanf(String.GetBuffer(), "%d", (TCHAR *)&f);
			return f;
		};
		static MWDString RealToString(float Value) {
			MWDSprintf(MWDString::TempBuffer, 2048, _T("%f"), Value);
			return MWDString(MWDString::TempBuffer);
		};
		static char* TCHARToChar(const TCHAR* pTchar)
		{
			char* pChar = nullptr;
			int nLen = wcslen(pTchar) + 1;
			pChar = new char[nLen * 2];
			WideCharToMultiByte(CP_ACP, 0, pTchar, nLen, pChar, 2 * nLen, NULL, NULL);
			return pChar;
		}
		static TCHAR* CharToTCHAR(const char* pChar)
		{
			TCHAR* pTchar = nullptr;
			int nLen = strlen(pChar) + 1;
			pTchar = new wchar_t[nLen];
			MultiByteToWideChar(CP_ACP, 0, pChar, nLen, pTchar, nLen);
			return pTchar;
		}
#pragma endregion

	};
	class MWDDATASTRUCT_API MWDFileName : public MWDString
	{
	public:
		MWDFileName()
		{

		}
		MWDFileName(const TCHAR* String) :MWDString(String)
		{
		}
		MWDFileName(const MWDString& String) :MWDString(String)
		{

		}
		~MWDFileName()
		{

		}
		bool GetExtension(MWDString& Extension) {
			return Extension.GetString(*this, _T('.'), -1, false, false);
		};
		bool GetBaseName(MWDString& BaseName) {
			bool bResult = false;
			MWDString BaseNameAndExtension;
			bResult = GetBaseNameAndExtension(BaseNameAndExtension);
			if (!bResult)
			{
				return false;
			}
			bResult = BaseName.GetString(BaseNameAndExtension, _T('.'), -1);
			return bResult;
		};
		bool GetBaseNameAndExtension(MWDString& BaseNameAndExtension) {
			bool bResult = false;
			bResult = BaseNameAndExtension.GetString(*this, _T('\\'), -1, false, false);
			if (!bResult)
			{
				bResult = BaseNameAndExtension.GetString(*this, _T('/'), -1, false, false);
			}
			return bResult;
		};
		bool GetPath(MWDString& Path) {
			bool bResult = false;
			bResult = Path.GetString(*this, _T('\\'), -1, true, false);
			if (!bResult)
			{
				bResult = Path.GetString(*this, _T('/'), -1, true, false);
			}
			return bResult;
		};
		bool GetPathAndName(MWDString& PathAndName) {
			return PathAndName.GetString(*this, _T('.'), -1);
		};
	};
}
