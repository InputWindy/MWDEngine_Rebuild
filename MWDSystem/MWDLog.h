#pragma once
#include "MWDFile.h"
namespace MWDEngine {
	class MWDSYSTEM_API MWDLog :public MWDFile
	{
	public:
		MWDLog() {};
		~MWDLog() {};
		bool Open(const TCHAR* pFileName) { return MWDFile::Open(pFileName,(TCHAR)"r"); };
		bool WriteInfo(const TCHAR* pcString, ...)const;
	};
}


