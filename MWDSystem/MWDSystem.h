#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <tchar.h>
#include <memory.h>
#include <assert.h>
#include <sys/stat.h>
#include <windows.h>
#include <iostream>
#pragma warning(disable:4251)
#pragma warning(disable:4595)
#pragma warning(disable:4091)
#define MWDSYSTEM_EXPORTS
//导出宏（dll文件都是dllexport，exe和lib文件才用dllimport）
#ifdef MWDSYSTEM_EXPORTS
#define MWDSYSTEM_API __declspec(dllexport) 
#else 
#define MWDSYSTEM_API __declspec(dllimport) 
#endif
using namespace std;
namespace  MWDEngine
{
	/*
	typedef char            TCHAR;
	typedef char*			PTCHAR;
	typedef unsigned char   TBYTE;
	typedef unsigned char*  PTBYTE;
	*/
	#pragma region DefWindows
	#ifdef WIN32
		#define SYSTEM_BIT_WIDTH 32
	#endif

	#ifdef WIN64
		#define SYSTEM_BIT_WIDTH 64
	#endif

	#define THREAD_CALLBACK __stdcall

	#ifdef WIN32 || WIN64
	#define WINDOWS
	#endif
	#pragma endregion

	#define LOG_BUFFER_SIZE 65536	 

	class MWDSYSTEM_API MWDSystem {
	public:
		MWDSystem() {};
		~MWDSystem() {};
		static inline DWORD ms_dwMainThreadID ;//主线程ID
		static inline TCHAR ms_sLogBuffer[LOG_BUFFER_SIZE];//日志输出
	};
	#pragma region 库函数封装
#define LogBuffer(buf,size) \
		for (int i = 0; i < size; ++i) {\
			cout << ((int*)buf)[i] << " ";\
		}\
		cout << endl;
		

	FORCEINLINE bool MWDMemcpy(void* pDest, const void* pSrc, unsigned int uiCountSize, unsigned int uiDestBufferSize = 0)
		{
			if (!uiDestBufferSize)
			{
				uiDestBufferSize = uiCountSize;
			}
	#ifdef WINDOWS
			return (memcpy_s(pDest, uiDestBufferSize, pSrc, uiCountSize) == 0);
	#else
			return false;
	#endif

		}
		FORCEINLINE unsigned int MWDStrLen(const TCHAR* pStr)
		{
	#ifdef WINDOWS
			return (unsigned int)_tcslen(pStr);
	#else
			return 0;
	#endif
		}
		FORCEINLINE void MWDLockedIncrement(long* pRefCount)
		{
	#ifdef WINDOWS
			_InterlockedIncrement(pRefCount);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDLockedDecrement(long* pRefCount)
		{
	#ifdef WINDOWS
			_InterlockedDecrement(pRefCount);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDMemset(void* pDest, int iC, unsigned int uiCount)
		{
	#ifdef WINDOWS
			memset(pDest, iC, uiCount);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDStrcat(TCHAR* pDest, unsigned int uiCount, const TCHAR* pSource)
		{
	#ifdef WINDOWS
			_tcscat_s(pDest, uiCount, pSource);
	#else
			return;
	#endif
		}
		//Find the next token in a string.
		FORCEINLINE TCHAR* MWDStrtok(TCHAR* strToken, const TCHAR* strDelimit, TCHAR** pContext)
		{
	#ifdef WINDOWS
			return _tcstok_s(strToken, strDelimit, pContext);
	#else
			return NULL;
	#endif
		}
		//Scan a string for the last occurrence of a character.
		FORCEINLINE const TCHAR* MWDCsrchr(const TCHAR* pString, int c)
		{
	#ifdef WINDOWS
			return _tcsrchr(pString, c);
	#else
			return NULL;
	#endif
		}
		FORCEINLINE void MWDOutPutDebugString(const TCHAR* pcString, ...)
		{
	#ifdef WINDOWS
			char* pArgs;
			pArgs = (char*)&pcString + sizeof(pcString);
			_vstprintf_s(MWDSystem::ms_sLogBuffer, LOG_BUFFER_SIZE, pcString, pArgs);
			OutputDebugString(MWDSystem::ms_sLogBuffer);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDSprintf(TCHAR* _DstBuf, unsigned int _SizeInBytes, const TCHAR* _Format, ...)
		{
	#ifdef WINDOWS
			char* pArgs;
			pArgs = (char*)&_Format + sizeof(_Format);
			_vstprintf_s(_DstBuf, _SizeInBytes, _Format, pArgs);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDSprintf(TCHAR* _DstBuf, unsigned int _SizeInBytes, const TCHAR* _Format, va_list pArgs)
		{
	#ifdef WINDOWS
			_vstprintf_s(_DstBuf, _SizeInBytes, _Format, pArgs);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDScanf(TCHAR* Buf, const TCHAR* _Format, va_list pArgs)
		{
	#ifdef WINDOWS
			_stscanf_s(Buf, _Format, pArgs);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDStrCopy(TCHAR* pDest, unsigned int uiCount, const TCHAR* pSource)
		{
	#ifdef WINDOWS
			_tcscpy_s(pDest, uiCount, pSource);
	#else
			return;
	#endif
		}
		FORCEINLINE int MWDStrCmp(const TCHAR* String1, const TCHAR* String2)
		{
	#ifdef WINDOWS
			return _tcscmp(String1, String2);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDMbsToWcs(wchar_t* Dest, unsigned int uiSizeInWord, const char* Source, unsigned int uiSizeInByte)
		{
	#ifdef WINDOWS
			mbstowcs_s(0, Dest, uiSizeInWord, Source, uiSizeInByte);
	#else
			return;
	#endif
		}
		FORCEINLINE void MWDWcsToMbs(char* Dest, unsigned int uiSizeInByte, const wchar_t* Source, unsigned int uiSizeInWord)
		{
	#ifdef WINDOWS
			wcstombs_s(0, Dest, uiSizeInByte, Source, uiSizeInWord);
	#else
			return;
	#endif
		}

		//SYSTEM_INFO SyetemInfo;
		//GetSystemInfo(&SyetemInfo);
		FORCEINLINE unsigned int MWDGetCpuNum()
		{
	#ifdef WINDOWS
			SYSTEM_INFO SyetemInfo;
			GetSystemInfo(&SyetemInfo);
			return SyetemInfo.dwNumberOfProcessors;
	#else
			return 1;
	#endif
		}

		//GetCurrentThreadId()
		FORCEINLINE bool MWDIsMainThread()
		{
	#ifdef WINDOWS
			return MWDSystem::ms_dwMainThreadID == GetCurrentThreadId();
	#else
			return false;
	#endif
		}

		FORCEINLINE void MWDInitSystem()
		{
	#ifdef WINDOWS
			MWDSystem::ms_dwMainThreadID = GetCurrentThreadId();
	#else
			return;
	#endif
		}
	#pragma endregion

	#pragma region MyRegion
#define MWDMAC_ASSERT(Expression)\
	{\
		 assert(Expression);\
	}

		template<typename T>
		FORCEINLINE T ABS(T t)
		{
			// 	if (t < 0) 
			// 		return -t; 
			return t < 0 ? -t : t;
			/*return t; */
		}
		template<typename T>
		FORCEINLINE T Min(T t0, T t1)
		{
			return t0 < t1 ? t0 : t1;
		}
		template<typename T>
		FORCEINLINE T Max(T t0, T t1)
		{
			return t0 > t1 ? t0 : t1;
		}
		//范围限制
		template<typename T>
		FORCEINLINE T Clamp(T Value, T Max, T Min)
		{
			if (Value >= Max)
			{
				return Max;
			}
			if (Value <= Min)
			{
				return Min;
			}
			return Value;
		}
		template <class T>
		FORCEINLINE void Swap(T& t1, T& t2)
		{
			T temp;
			temp = t1;
			t1 = t2;
			t2 = temp;
		}
#define BIT(i) (1 << i)
#pragma endregion



	
}