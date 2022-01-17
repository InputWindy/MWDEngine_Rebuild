#include "pch.h"
#include "MWDSynchronize.h"
MWDEngine::MWDCriticalSection g_SafeCS;
namespace MWDEngine{

	unsigned int MWDSynchronize::WaitAll(MWDSynchronize** pSynchronize, unsigned int uiNum, bool bWaitAll, DWORD dwMilliseconds)
	{
		MWDMAC_ASSERT(uiNum >= 1 && uiNum <= MAXIMUM_WAIT_OBJECTS);
		//MAXIMUM_WAIT_OBJECTS = 64
		static HANDLE handle[MAXIMUM_WAIT_OBJECTS];
		for (unsigned int i = 0; i < uiNum; i++)
		{
			handle[i] = (HANDLE)pSynchronize[i]->GetHandle();
		}
		DWORD dw = WaitForMultipleObjects(uiNum, handle, bWaitAll, dwMilliseconds);
		switch (dw)
		{
		case WAIT_FAILED:
			return WF_FAILED;
		case WAIT_TIMEOUT:
			return WF_TIMEOUT;
		case WAIT_OBJECT_0:
			return WF_OBJECT0;
		}
		return WF_FAILED;
	}
	void MWDSynchronize::MWDSafeOutPutDebugString(const TCHAR* pcString, ...)
	{
		g_SafeCS.Lock();
		char* pArgs;
		pArgs = (char*)&pcString + sizeof(pcString);
		_vstprintf_s(MWDSystem::ms_sLogBuffer, LOG_BUFFER_SIZE, pcString, pArgs);
		OutputDebugString(MWDSystem::ms_sLogBuffer);
		g_SafeCS.Unlock();
	}


	MWDSemaphore::MWDSemaphore(unsigned int uiCount, unsigned int MaxCount)
	{
		MWDMAC_ASSERT(uiCount <= MaxCount);
		m_Semaphore = CreateSemaphore(NULL, uiCount, MaxCount, NULL);
		m_uiMaxCount = MaxCount;
		MWDMAC_ASSERT(m_Semaphore);
	}
	MWDSemaphore::~MWDSemaphore()
	{
		BOOL closed = CloseHandle((HANDLE)m_Semaphore);
		m_Semaphore = NULL;
		MWDMAC_ASSERT(closed);
	}
	//等待内核资源，等到了就无限占用
	void MWDSemaphore::Enter()
	{
		//WaitForSingleObject(HANDLE, dwMilliseconds);
		DWORD result = WaitForSingleObject((HANDLE)m_Semaphore, INFINITE);
		MWDMAC_ASSERT(result);
		// result:
		//   WAIT_ABANDONED (0x00000080)
		//   WAIT_OBJECT_0  (0x00000000), signaled
		//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
		//   WAIT_FAILED    (0xFFFFFFFF), not signaled
	}
	void MWDSemaphore::Leave(unsigned int uiReleaseCount)
	{
		BOOL released = ReleaseSemaphore((HANDLE)m_Semaphore, uiReleaseCount, NULL);
		MWDMAC_ASSERT(released);
	}


	MWDMutex::MWDMutex()
	{
		m_Mutex = CreateMutex(NULL, FALSE, NULL);
		MWDMAC_ASSERT(m_Mutex);
	}
	MWDMutex::~MWDMutex()
	{
		BOOL closed = CloseHandle((HANDLE)m_Mutex);
		m_Mutex = NULL;
		MWDMAC_ASSERT(closed);
	}
	void MWDMutex::Enter()
	{
		DWORD result = WaitForSingleObject((HANDLE)m_Mutex, INFINITE);
		MWDMAC_ASSERT(result != WAIT_FAILED);
		// result:
		//   WAIT_ABANDONED (0x00000080)
		//   WAIT_OBJECT_0  (0x00000000), signaled
		//   WAIT_TIMEOUT   (0x00000102), [not possible with INFINITE]
		//   WAIT_FAILED    (0xFFFFFFFF), not signaled
	}
	void MWDMutex::Leave()
	{
		BOOL released = ReleaseMutex((HANDLE)m_Mutex);
		MWDMAC_ASSERT(released);
	}


	MWDEvent::MWDEvent()
	{
		Event = NULL;
	}
	MWDEvent::~MWDEvent(void)
	{
		if (Event != NULL)
		{
			CloseHandle(Event);
		}
	}
	bool MWDEvent::Create(bool bIsManualReset, const TCHAR* InName)
	{
		//因为MWDEvent不一定能够创建成功，所以不能放在构造函数里申请
		Event = CreateEvent(NULL, bIsManualReset, 0, InName);
		return Event != NULL;
	}
	void MWDEvent::Lock(void)
	{
		WaitForSingleObject(Event, INFINITE);
	}
	void MWDEvent::Unlock(void)
	{
		PulseEvent(Event);
	}
	void MWDEvent::Active(void)
	{
		SetEvent(Event);
	}
	void MWDEvent::Negative(void)
	{
		ResetEvent(Event);
	}
	void MWDEvent::Pulse(void)
	{
		PulseEvent(Event);
	}
	bool MWDEvent::Wait(DWORD WaitTime)
	{
		return WaitForSingleObject(Event, WaitTime) == WAIT_OBJECT_0;
	}
	bool MWDEvent::IsTrigger()
	{
		return  Wait(0);
	}

}