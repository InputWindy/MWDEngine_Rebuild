#include "pch.h"
#include "MWDLog.h"
namespace MWDEngine {
	bool MWDLog::WriteInfo(const TCHAR* pcString, ...) const
	{
		char* pArgs;

		pArgs = (char*)&pcString + sizeof(pcString);
		_vstprintf_s(MWDSystem::ms_sLogBuffer, LOG_BUFFER_SIZE, pcString, pArgs);
		_ftprintf(m_File, MWDSystem::ms_sLogBuffer);
		return 1;
	}
}
