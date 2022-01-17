#pragma once
#include "MWDMath.h"
#include "MWDVector3.h"
namespace MWDEngine {
	class MWDMATH_API MWDSurface3 : public MWDMemObject
	{
	public:
		MWDSurface3() {
		
		};
		virtual ~MWDSurface3() {
			
		};
		virtual MWDVector3 GetPoint(MWDREAL U, MWDREAL V) = 0;
		virtual MWDVector3 GetUPartialDerivative(MWDREAL U, MWDREAL V) = 0;
		virtual MWDVector3 GetVPartialDerivative(MWDREAL U, MWDREAL V) = 0;
		//获取法线N
		FORCEINLINE MWDVector3 GetNormal(MWDREAL U, MWDREAL V) {
			MWDVector3 N;
			N.Cross(GetUPartialDerivative(U, V), GetVPartialDerivative(U, V));
			return N;
		};
	};
}
