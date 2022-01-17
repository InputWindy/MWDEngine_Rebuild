#pragma once
#include "MWDMath.h"
#include "MWDRay3.h"
#include "MWDVector3.h"
#include "MWDLine3.h"
namespace MWDEngine {
	class MWDMATH_API MWDSegment3 :public MWDRay3
	{
	private:
		MWDVector3	m_End;
		MWDREAL		m_fLen;
	public:
		MWDSegment3() {
			
		};
		~MWDSegment3() {

		};
		MWDSegment3( MWDVector3& Orig,  MWDVector3& End) {
			Set(Orig, End);
		};
		MWDSegment3( MWDVector3& Orig,  MWDVector3& Dir, MWDREAL fLen) {
			Set(Orig, Dir, fLen);
		};
		
		FORCEINLINE const MWDVector3& GetEnd()const {
			return m_End;
		};
		FORCEINLINE MWDREAL GetLen()const {
			return m_fLen;
		};
		FORCEINLINE void Set( MWDVector3& Orig,  MWDVector3& End) {
			MWDVector3 Dir = End - Orig;
			m_fLen = Dir.GetLength();
			MWDLine3::Set(Orig, Dir);
		};
		FORCEINLINE void SetEnd(  MWDVector3& End) {
			MWDVector3 Dir = End -GetOrig(), Orig = GetOrig();
			m_fLen = Dir.GetLength();
			MWDLine3::Set(Orig, Dir);
		};
		FORCEINLINE void SetLen(MWDREAL fLen) {
			MWDVector3 Dir = GetEnd() -GetOrig(), Orig = GetOrig();
			m_fLen = Dir.GetLength();
			MWDLine3::Set(Orig, Dir);
		};
		FORCEINLINE void Set( MWDVector3& Orig,  MWDVector3& Dir, MWDREAL fLen) {
			MWDLine3::Set(Orig, Dir);
			m_fLen = fLen;
			m_End = Orig + (MWDRay3::m_Dir * m_fLen);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fSegmentParameter)const {
			if (fSegmentParameter > m_fLen)
				fSegmentParameter = m_fLen;
			return MWDRay3::GetParameterPoint(fSegmentParameter);
		};

	};

}
