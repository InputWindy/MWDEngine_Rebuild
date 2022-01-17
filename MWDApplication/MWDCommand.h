#pragma once
#include <MWDMath.h>
#include <MWDString.h>
#include <MWDMap.h>
#include <MWDMemManager.h>
#include <MWDSystem.h>
namespace MWDEngine {
	class MWDCommand : public MWDMemObject
	{
	public:
		MWDCommand(int numArguments, TCHAR** arguments) {
			for (int i = 0; i < numArguments; ++i)
			{
				//µÚiÌõ×Ö·û´®
				MWDString Command(arguments[i]);
				MWDString Value;
				if (Value.GetString(Command, '=', 1, false))
				{
					MWDString Key;
					Key.GetString(Command, '=', 1);
					m_CommandMap.AddElement(Key, Value);
				}
				else
				{
					m_CommandMap.AddElement(Command, Value);
				}
			}
		};
		MWDCommand(TCHAR* commandLine) {
			if (!commandLine)
			{
				return;
			}

			TCHAR* pNextToken = NULL;
			TCHAR* token = MWDStrtok(commandLine, _T(" "), &pNextToken);
			while (token)
			{
				MWDString Command(token);
				MWDString Value;
				if (Command.GetString(Value, '=', 1, false))
				{
					MWDString Key;
					Command.GetString(Key, '=', 1);
					m_CommandMap.AddElement(Key, Value);
				}
				else
				{
					m_CommandMap.AddElement(Command, Value);
				}

				token = MWDStrtok(NULL, _T(" "), &pNextToken);
			}
		};
		~MWDCommand() {

		};

		bool GetName(const TCHAR* name) {
			if (m_CommandMap.Find(name) != m_CommandMap.GetNum())
			{
				return true;
			}

			return false;
		};
		bool GetInteger(const TCHAR* name, int& value) {
			unsigned int i = m_CommandMap.Find(name);
			if (i != m_CommandMap.GetNum())
			{
				value = MWDString::StringToInt(m_CommandMap[i].Value);
				return true;
			}

			return false;
		};
		bool GetReal(const TCHAR* name, MWDREAL& value) {
			unsigned int i = m_CommandMap.Find(name);
			if (i != m_CommandMap.GetNum())
			{
				value = MWDString::StringToReal(m_CommandMap[i].Value);
				return true;
			}
			return false;
		};
		bool GetString(const TCHAR* name, MWDString& value) {
			unsigned int i = m_CommandMap.Find(name);
			if (i != m_CommandMap.GetNum())
			{
				value = m_CommandMap[i].Value;
				return true;
			}

			return false;
		};


	protected:

		MWDMap<MWDString, MWDString> m_CommandMap;
	};
}
