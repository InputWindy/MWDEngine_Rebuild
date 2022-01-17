#pragma once
#include "MWDSystem.h"
namespace MWDEngine{

	#pragma region MWDCriticalSection
	//=============================================================
	//CriticalSection是临界区的意思
	//具体知识点👇
	//https://baike.baidu.com/item/critical%20section/7448983
	//每个线程中访问临界资源的那段程序称为临界区（Critical Section）
	//（临界资源是一次仅允许一个线程使用的共享资源）。
	// 多个线程中涉及到同一个临界资源的临界区称为相关临界区。
	// 
	// 如果某个类需要涉及多线程操作，就维护一个MWDCriticalSection，
	// 某一个线程使用这个类的时候，就MWDCriticalSection.lock(),
	// 别的线程此时就不能使用这个类。用完之后就MWDCriticalSection.unlock(),
	// MWDCriticalSection没有Get()，因为它不是内核资源
	//=============================================================
	class MWDSYSTEM_API MWDCriticalSection
	{
		public:
			FORCEINLINE MWDCriticalSection(void)
			{
				InitializeCriticalSection(&m_CriticalSection);
				//旋转锁4000
				SetCriticalSectionSpinCount(&m_CriticalSection, 4000);
			}

			FORCEINLINE ~MWDCriticalSection(void)
			{
				DeleteCriticalSection(&m_CriticalSection);
			}
			//Lock之后如果不解开会一直锁死
			FORCEINLINE void Lock(void)
			{
				EnterCriticalSection(&m_CriticalSection);//进入临界区
			}

			FORCEINLINE void Unlock(void)
			{
				LeaveCriticalSection(&m_CriticalSection);//退出临界区
			}
		private:
			CRITICAL_SECTION m_CriticalSection;
	};
	#pragma endregion

	#pragma region MWDSynchronize(线程基类)
		class MWDSYSTEM_API MWDSynchronize
		{
		public:
			MWDSynchronize()
			{

			}

			virtual ~MWDSynchronize()
			{

			}

			virtual void* GetHandle() = 0;

			enum
			{
				WF_OBJECT0 = 0,
				WF_TIMEOUT = 256,
				WF_FAILED = 0xFFFFFFFF
			};
			//当前线程等待所有内核资源（可以选择等其中随便一个好，或者全部好）
			static unsigned int WaitAll(MWDSynchronize** pSynchronize, unsigned int uiNum, bool bWaitAll, DWORD dwMilliseconds = (DWORD)-1);
			//日志输出
			static void MWDSafeOutPutDebugString(const TCHAR* pcString, ...);
		};
	#pragma endregion

	
	#pragma region MWDSemaphore
		//MWDSemaphore维护一个内核资源
		class MWDSYSTEM_API MWDSemaphore : public MWDSynchronize
		{
			public:
				MWDSemaphore(unsigned int uiCount, unsigned int MaxCount);
				virtual ~MWDSemaphore();

				virtual void Enter();
				virtual void Leave(unsigned int uiReleaseCount);
				virtual HANDLE GetHandle()
				{
					return m_Semaphore;
				}
			protected:
				HANDLE m_Semaphore;
				unsigned int m_uiMaxCount;
		};
	#pragma endregion

	
	#pragma region MWDMutex
		class MWDSYSTEM_API MWDMutex : public MWDSynchronize
		{
			public:

				MWDMutex();
				virtual ~MWDMutex();

				virtual void Enter();
				virtual void Leave();
				virtual HANDLE GetHandle()
				{
					return m_Mutex;
				}
			protected:
				HANDLE m_Mutex;
		};
	#pragma endregion

	#pragma region MWDEvent
	class MWDSYSTEM_API MWDEvent : public MWDSynchronize
	{
		//Event会被继承，所以是private
		private:
			HANDLE Event;

		protected:
			FORCEINLINE virtual HANDLE GetHandler() {
				MWDMAC_ASSERT(Event)
					return Event;
			}

		public:
			MWDEvent();
			virtual ~MWDEvent(void);
			FORCEINLINE virtual bool Create(bool bIsManualReset = FALSE, const TCHAR* InName = NULL);
			FORCEINLINE virtual void Lock(void);			//监听者API
			FORCEINLINE virtual void Unlock(void);			//监听者API
			FORCEINLINE virtual bool Wait(DWORD WaitTime = (DWORD)-1);	//监听者API
			FORCEINLINE virtual bool IsTrigger();			//监听者API

			FORCEINLINE virtual void Active(void);			//发布者API
			FORCEINLINE virtual void Negative(void);		//发布者API
			FORCEINLINE virtual void Pulse(void);			//发布者API
	};
#pragma endregion

}


