#pragma once
#include <iostream>
#include "MWDSystem.h"
#include "MWDSynchronize.h"
using namespace std;
namespace MWDEngine {
	class MWDSYSTEM_API MWDThread
	{
		private:
			//线程回调函数,Start()之后会触发这个函数,但是只触发了一次,线程回调函数必须是static的
			static void SetThreadName(DWORD _ThreadID, const TCHAR* _name);	
			static DWORD THREAD_CALLBACK ThreadProc(void* t) {
				MWDThread* pThread = (MWDThread*)t;
				SetThreadName(pThread->m_ThreadID,pThread->GetThreadName());
				pThread->Run();
				return 0;
			};		

			//不允许赋值构造
			MWDThread(const MWDThread&) {};							
			/*MWDThread& operator=(const MWDThread&) {
				return (MWDThread)0;
			};*/				

		//public是对外接口
		public:
			enum Priority
			{
				Low,
				Normal,
				High,
			};									
			enum ThreadState
			{
				TS_START,
				TS_SUSPEND,
				TS_STOP,
			};								
			MWDThread(const TCHAR* name , unsigned int _stackSize); 
			virtual ~MWDThread();									

			void SetPriority(Priority p);							

			FORCEINLINE Priority GetPriority() const {				
				MWDMAC_ASSERT(m_priority)
				return m_priority;
			};	    
			FORCEINLINE unsigned int GetStackSize() const {
				MWDMAC_ASSERT(m_stackSize>=0)
				return m_stackSize;
			};	
			FORCEINLINE ThreadState GetThreadState()const
			{
				return m_ThreadState;
			}	
			FORCEINLINE const TCHAR* GetThreadName()const {
				return &m_ThreadName;
			};	

			//线程基本操作
			void Start();											
			void Suspend();											
			void Sleep(DWORD dwMillseconds);						
			void Stop();											

			bool IsRunning() const;									
			bool IsStopped() const;									
		//private是对内接口
		private:
			//外部不能访问的属性
			HANDLE		m_Thread;									
			DWORD		m_ThreadID;									
			//外部可以访问的属性
			Priority	m_priority;		//可读可写					
			unsigned int m_stackSize;	//只读						
			TCHAR		m_ThreadName;	//只读						
			ThreadState m_ThreadState;	//可读可写					
			
		protected:
			//线程执行内容
			virtual void Run() = 0;
			//线程具有句柄和id
			FORCEINLINE HANDLE GetHandle() const {
				MWDMAC_ASSERT(m_Thread)
					return m_Thread;
			}			
			FORCEINLINE DWORD GetThreadID() const {
				MWDMAC_ASSERT(m_ThreadID == ::GetCurrentThreadId())
					return m_ThreadID;
			}			

				
	};
}


