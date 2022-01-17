#pragma once
#include "MWDMath.h"
#include "MWDVector2.h"
namespace MWDEngine {
	class MWDMATH_API MWDRect2
	{
	public:
		MWDVector2 m_Min;
		MWDVector2 m_Max;
		MWDRect2(void) {
			
		};
		MWDRect2(const MWDVector2& Min, const MWDVector2& Max) {
			m_Min = Min;
			m_Max = Max;
		};

	};
}
