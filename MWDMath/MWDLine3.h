#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
#include "MWDMatrix3X3W.h"
namespace MWDEngine {
	class MWDMATH_API MWDLine3
	{
	protected:
		MWDVector3	m_Orig;
		MWDVector3	m_Dir;  
	public:
		MWDLine3() {
		
		};
		MWDLine3(const MWDVector3& Orig,const  MWDVector3& Dir) {
			Set(Orig, Dir);
		};
		~MWDLine3() {

		};
		
		FORCEINLINE void Set(const MWDVector3& Orig,const  MWDVector3& Dir) {
			m_Orig = Orig;
			m_Dir = Dir;
			m_Dir.Normalize();
		};
		//°üº¬normalize
		FORCEINLINE void SetDir( MWDVector3& Dir) {
			m_Dir = Dir;
			m_Dir.Normalize();
		};
		FORCEINLINE void SetOrig( MWDVector3& Orig) {
			m_Orig = Orig;
		};
		FORCEINLINE const MWDVector3& GetOrig()const {
			return m_Orig;
		};
		FORCEINLINE const MWDVector3& GetDir()const {
			return m_Dir;
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fLineParameter)const {
			return  (m_Orig + m_Dir * fLineParameter);
		};

	};

}
