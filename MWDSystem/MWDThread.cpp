#include "pch.h"
#include "MWDThread.h"

namespace MWDEngine {
	MWDThread::MWDThread(const TCHAR* name, unsigned int _stackSize)
	{
		m_priority = MWDThread::Priority::Normal;
		m_stackSize = _stackSize;
		m_ThreadName = *name;
		m_ThreadID = ::GetCurrentThreadId();
		m_ThreadState = TS_SUSPEND;

		MWDMAC_ASSERT(!IsRunning());
		//获取线程HANDLE
		m_Thread = ::CreateThread(0, m_stackSize, ThreadProc, this, CREATE_SUSPENDED, NULL);
		MWDMAC_ASSERT(m_Thread);

		SetPriority(m_priority);
	};

	MWDThread::~MWDThread()
	{
		//关闭线程之后，其他属性会全部被delete掉
		if (IsRunning())
		{
			TerminateThread(m_Thread, 0);
		}
	}

	void MWDThread::SetPriority(Priority p)
	{
		int nPriority = THREAD_PRIORITY_NORMAL;

		if (p == Low)
			nPriority = THREAD_PRIORITY_BELOW_NORMAL;
		else if (p == Normal)
			nPriority = THREAD_PRIORITY_NORMAL;
		else if (p == High)
			nPriority = THREAD_PRIORITY_ABOVE_NORMAL;

		::SetThreadPriority(this->m_Thread, nPriority);
		m_priority = p;
	}

	void MWDThread::Start()
	{
		if (m_ThreadState == TS_SUSPEND)
		{
			DWORD ret =  ResumeThread(m_Thread);
			if (ret == 0xFFFFFFFF) {
				cout << "唤醒失败" << endl;
			}
			else {
				cout << "唤醒成功" << endl;
				m_ThreadState = TS_START;
			}
		}
	}

	void MWDThread::Suspend()
	{
		if (m_ThreadState == TS_START)
		{
			DWORD ret = SuspendThread(m_Thread);
			if (ret == 0xFFFFFFFF) {
				cout << "挂起失败" << endl;
			}
			else {
				cout << "挂起成功" << endl;
				m_ThreadState = TS_SUSPEND;
			}
		}
	}

	void MWDThread::Sleep(DWORD dwMillseconds)
	{
		if (m_ThreadState == TS_START)
		{
			::Sleep(dwMillseconds);
		}
	}

	void MWDThread::Stop()
	{
		if (m_ThreadState == TS_START)
		{
			MWDMAC_ASSERT(this->IsRunning());
			MWDMAC_ASSERT(NULL != m_Thread);

			m_ThreadState = TS_STOP;
			WaitForSingleObject(m_Thread, INFINITE);
			CloseHandle(m_Thread);
			m_Thread = NULL;
		}
	}

	bool MWDThread::IsRunning() const
	{
		if (NULL != m_Thread)
		{
			DWORD exitCode = 0;
			if (GetExitCodeThread(m_Thread, &exitCode))
			{
				if (STILL_ACTIVE == exitCode)
				{
					return true;
				}
			}
		}

		return false;
	}

	bool MWDThread::IsStopped() const
	{
		if (m_ThreadState == TS_STOP) {
			return true;
		}
		else
		{
			return false;
		}
	}

	void MWDThread::SetThreadName(DWORD _ThreadID, const TCHAR* _name)
	{
		struct THREADNAME_INFO
		{
			DWORD dwType;     // must be 0x1000
			const TCHAR* szName;    // pointer to name (in user address space)
			DWORD dwThreadID; // thread ID (-1 = caller thread)
			DWORD dwFlags;    // reserved for future use, must be zero
		};

		THREADNAME_INFO info;
		info.dwType = 0x1000;
		info.szName = _name;
		info.dwThreadID = _ThreadID;
		info.dwFlags = 0;
		__try
		{
			RaiseException(0x406D1388, 0, sizeof(info) / sizeof(DWORD), (ULONG_PTR*)&info);
		}
		__except (EXCEPTION_CONTINUE_EXECUTION)
		{

		}
	}

}


