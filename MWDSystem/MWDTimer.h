#pragma once
#include "MWDSystem.h"
#include <mmsystem.h>
namespace MWDEngine {
	class MWDSYSTEM_API MWDTimer
	{
	private:
		bool m_bUseLargeTime;										//使用大时间
		__int64 m_int64OneSecondTicks;  
		__int64 m_int64TimeTickStartCounts; 
		unsigned long m_ulTimeStart;								//开始时间
		int m_iFrameCount;											//帧数计数
		double m_fFPS;												//FPS
		double m_fTime, m_fLastFPSTime, m_fTimeSlice;				//计算FPS的中间变量
		double m_fDetTime, m_fLastTime;								//计算时间
		void InitGameTime() {
			m_iFrameCount = 0;
			m_fFPS = 0;
			m_fTime = 0;
			m_fLastFPSTime = 0;
			m_fTimeSlice = 0;
			m_fLastTime = 0;
			m_fDetTime = 0;
			//查询是否可以使用大时间
			if (QueryPerformanceFrequency((LARGE_INTEGER*)&m_int64OneSecondTicks))
			{
				m_bUseLargeTime = true;
				QueryPerformanceCounter((LARGE_INTEGER*)&m_int64TimeTickStartCounts);
			}
			else
			{
				m_bUseLargeTime = false;
				//系统时间timeGetTime();
				m_ulTimeStart = timeGetTime();
			}
		};
	public:
		static inline MWDTimer* ms_pTimer = NULL;
		MWDTimer() {
			InitGameTime();
			if(!ms_pTimer)
				ms_pTimer = this;
		};
		~MWDTimer() {};

		//返回引擎开启总时长
		double GetGamePlayTime() {
			__int64 int64TimeCurrentCounts;
			if (m_bUseLargeTime)
			{
				QueryPerformanceCounter((LARGE_INTEGER*)&int64TimeCurrentCounts);
				return ((int64TimeCurrentCounts - m_int64TimeTickStartCounts) * (1.0 / m_int64OneSecondTicks) * 1000.0);
			}
			else
			{
				return ((timeGetTime() - m_ulTimeStart)); 
			}
		};
		void UpdateFPS() {
			m_fTime = GetGamePlayTime() * 0.001;
			m_fDetTime = m_fTime - m_fLastTime;
			m_fLastTime = m_fTime;
			if (m_fTime - m_fLastFPSTime > 1.0f)
			{
				m_fLastFPSTime = m_fTime;
				m_fFPS = m_iFrameCount;
				m_iFrameCount = 0;
			}
			else
			{
				m_iFrameCount++;
			}
		};
		FORCEINLINE double GetFPS() { return m_fFPS; }
		
		double GetDetTime() { return m_fDetTime; }
		int GetRandSeed() { return ((LARGE_INTEGER*)&m_int64TimeTickStartCounts)->LowPart; };
	};
}


