#pragma once
#include "MWDMath.h"
#include "MWDPlane3.h"
#include "MWDVector3.h"
/***************************************************************************************

					fTriangleParameter[1]
		   m_V[0]	_________________________m_V[1]
				   \                       /
					\                     /
					 \                   /
					  \                 /
					   \               /
						\             /
fTriangleParameter[2]  \           /  fTriangleParameter[0]
						  \         /
						   \       /
							 \    /
							  \  /
							   \/m_V[2]
***************************************************************************************/
namespace MWDEngine
{
	class MWDMATH_API MWDTriangle3 :public MWDPlane3
	{
		friend class MWD;
	public:
		MWDVector3 m_V[3];
	public:

		MWDTriangle3() {
			
		};
		~MWDTriangle3() {

		};
		MWDTriangle3(const MWDVector3& P0, const MWDVector3& P1, const MWDVector3& P2) {
			Set(P0, P1, P2);
		};
		MWDTriangle3(const MWDVector3 Point[3]) {
			Set(Point);
		};
		
		/*************************************FORCEINLINE************************************************/

		FORCEINLINE void Set(const MWDVector3& P0, const MWDVector3& P1, const MWDVector3& P2) {
			m_V[0] = P0;
			m_V[2] = P2;
			m_V[1] = P1;
			MWDPlane3::Set(m_V);
		};
		FORCEINLINE void Set(const MWDVector3 Point[3]) {
			m_V[0] = Point[0];
			m_V[2] = Point[2];
			m_V[1] = Point[1];
			MWDPlane3::Set(m_V);
		};
		FORCEINLINE void GetPoint(MWDVector3  Point[3])const {
			Point[0] = m_V[0];
			Point[1] = m_V[1];
			Point[2] = m_V[2];
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fTriangleParameter[3])const {
			return (m_V[0] * fTriangleParameter[0] + m_V[1] * fTriangleParameter[1] + m_V[2] * fTriangleParameter[2]);
		};
		FORCEINLINE MWDVector3 GetParameterPoint(MWDREAL fTriangleParameter0,
			MWDREAL fTriangleParameter1, MWDREAL fTriangleParameter2)const {
			return (m_V[0] * fTriangleParameter0 + m_V[1] * fTriangleParameter1 + m_V[2] * fTriangleParameter2);
		};
		
	};

};