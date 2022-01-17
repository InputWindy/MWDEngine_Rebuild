#pragma once
#include "MWDMath.h"
#include "MWDLine3.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class  MWDMATH_API MWDRay3 :public MWDLine3
	{
	public:

		MWDRay3() {
			
		};
		MWDRay3( MWDVector3& Orig,  MWDVector3& Dir)	 {
			MWDLine3::Set(Orig, Dir);
		};
		~MWDRay3() {

		};
		
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fRayParameter)const {
			if (fRayParameter < 0)
				fRayParameter = 0;
			return MWDLine3::GetParameterPoint(fRayParameter);
		};

	}; 
}