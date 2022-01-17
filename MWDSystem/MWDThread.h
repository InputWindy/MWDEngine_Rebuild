#pragma once
#include <iostream>
#include "MWDSystem.h"
#include "MWDSynchronize.h"
using namespace std;
namespace MWDEngine {
	class MWDSYSTEM_API MWDThread
	{
		private:
			//�̻߳ص�����,Start()֮��ᴥ���������,����ֻ������һ��,�̻߳ص�����������static��
			static void SetThreadName(DWORD _ThreadID, const TCHAR* _name);	
			static DWORD THREAD_CALLBACK ThreadProc(void* t) {
				MWDThread* pThread = (MWDThread*)t;
				SetThreadName(pThread->m_ThreadID,pThread->GetThreadName());
				pThread->Run();
				return 0;
			};		

			//������ֵ����
			MWDThread(const MWDThread&) {};							
			/*MWDThread& operator=(const MWDThread&) {
				return (MWDThread)0;
			};*/				

		//public�Ƕ���ӿ�
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

			//�̻߳�������
			void Start();											
			void Suspend();											
			void Sleep(DWORD dwMillseconds);						
			void Stop();											

			bool IsRunning() const;									
			bool IsStopped() const;									
		//private�Ƕ��ڽӿ�
		private:
			//�ⲿ���ܷ��ʵ�����
			HANDLE		m_Thread;									
			DWORD		m_ThreadID;									
			//�ⲿ���Է��ʵ�����
			Priority	m_priority;		//�ɶ���д					
			unsigned int m_stackSize;	//ֻ��						
			TCHAR		m_ThreadName;	//ֻ��						
			ThreadState m_ThreadState;	//�ɶ���д					
			
		protected:
			//�߳�ִ������
			virtual void Run() = 0;
			//�߳̾��о����id
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


