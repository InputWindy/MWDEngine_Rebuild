#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class MWDMATH_API MWDCylinder3
	{
	public:
		MWDCylinder3(MWDVector3 Center, MWDVector3 Dir, MWDREAL fRadius, MWDREAL fHalfHeight) {
			m_Center = Center;
			m_Dir = Dir;
			m_fRadius = fRadius;
			m_fHalfHeight = fHalfHeight;
		};
		~MWDCylinder3() {
			
		};
		MWDVector3	m_Center;
		MWDVector3	m_Dir;
		MWDREAL		m_fRadius;
		MWDREAL		m_fHalfHeight;
	};
}
