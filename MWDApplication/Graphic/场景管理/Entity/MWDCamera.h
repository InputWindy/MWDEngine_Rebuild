#pragma once
#include "MWDEntity.h"
#include "../组件/MWDTransform.h"
#include <MWDCompute.h>
namespace MWDEngine {
	class MWDEntity;
	/// <summary>
	/// 维护VP矩阵、Transform组件。提供操作VP矩阵和Transform的接口
	/// </summary>
	typedef struct MWDVIEWPORT_TYPE
	{
		MWDREAL       XMin;
		MWDREAL       YMin;
		MWDREAL       XMax;
		MWDREAL       YMax;
		MWDREAL		ZMin;
		MWDREAL		ZMax;
		bool		bChangeAspect;
		void Reset()
		{
			XMin = 0.0f;
			YMin = 0.0f;
			XMax = 1.0f;
			YMax = 1.0f;
			ZMin = 0.0f;
			ZMax = 1.0f;
			bChangeAspect = false;
		}
		MWDVIEWPORT_TYPE()
		{
			Reset();
		}
	}MWDViewPort;

	class MWDCamera:public MWDEntity
	{
		DECLARE_CLASS_FUNCTION(MWDCamera)
		DECLARE_RTTI(MWDCamera, MWDEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDCamera)
	public:
		MWDCamera() {
			MWDTransform& trans = *new MWDTransform();
			AddComponent<MWDTransform>(trans);
			m_ViewMat.Identity();
			m_ProjMat.Identity();
			m_Fov = (MWDREAL)0.8;
			m_Aspect = (MWDREAL)(4.0 / 3.0);
			m_ZNear = (MWDREAL)1.0;
			m_ZFar = (MWDREAL)8000.0;
			m_ProjMat.CreatePerspective(m_Fov, m_Aspect, m_ZFar, m_ZNear);
			m_bActive = true;
		};
		MWDCamera(MWDName& name) {
			SetName(name);
			MWDTransform& trans = *new MWDTransform();
			AddComponent(trans);
			m_ViewMat.Identity();
			m_ProjMat.Identity();
			m_Fov = (MWDREAL)0.8;
			m_Aspect = (MWDREAL)(4.0 / 3.0);
			m_ZNear = (MWDREAL)1.0;
			m_ZFar = (MWDREAL)8000.0;
			m_ProjMat.CreatePerspective(m_Fov, m_Aspect, m_ZFar, m_ZNear);
			m_bActive = true;
		};
		MWDCamera(const TCHAR* name) {
			MWDName temp = MWDName(name);
			SetName(temp);
			MWDTransform& trans = *new MWDTransform();
			AddComponent(trans);
			m_ViewMat.Identity();
			m_ProjMat.Identity();
			m_Fov = (MWDREAL)0.8;
			m_Aspect = (MWDREAL)(4.0 / 3.0);
			m_ZNear = (MWDREAL)1.0;
			m_ZFar = (MWDREAL)8000.0;
			m_ProjMat.CreatePerspective(m_Fov, m_Aspect, m_ZFar, m_ZNear);
			m_bActive = true;
		};
		virtual ~MWDCamera() {
			
		};
		enum //Camera Plane
		{
			CP_RIGHT,
			CP_LEFT,
			CP_TOP,
			CP_BOTTOM,
			CP_FAR,
			CP_NEAR,
			CP_MAX = 6
		};
		FORCEINLINE MWDMatrix3X3W GetViewMatrix()const {
			return m_ViewMat;
		};
		FORCEINLINE MWDMatrix3X3W GetProjMatrix()const {
			return m_ProjMat;
		};
		void CreateFromEuler(const MWDVector3& Pos, MWDREAL RotX, MWDREAL RotY, MWDREAL RotZ) {
			m_Transform.SetLocalPosition(Pos);
			MWDQuat    qFrame(0, 0, 0, 1);
			MWD::CreateEuler(qFrame, RotY, RotX, RotZ);
			m_Transform.SetLocalRotation(qFrame);
		};
		void CreateFromLookDir(const MWDVector3& Pos, const MWDVector3& vcDir, const MWDVector3& vcUp = MWDVector3(0, 1, 0)) {
			MWDMatrix3X3W MatTemp;
			MatTemp.CreateFromLookDir(Pos, vcDir, vcUp);
			MWDMatrix3X3 Mat;
			Mat = MatTemp.Get3X3();
			MWDMatrix3X3 MatInv;
			MatInv.InverseOf(Mat);
			m_Transform.SetLocalRotation(MWD::GetQuat(MatInv));
			m_Transform.SetLocalPosition(Pos);
		};
		void CreateFromLookAt(const MWDVector3& vcPos, const MWDVector3& vcLookAt, const MWDVector3& vcUp = MWDVector3(0, 1, 0)) {
			MWDMatrix3X3W MatTemp;
			MatTemp.CreateFromLookAt(vcPos, vcLookAt, vcUp);

			MWDMatrix3X3 Mat;
			Mat = MatTemp.Get3X3();

			MWDMatrix3X3 MatInv;
			MatInv.InverseOf(Mat);

			m_Transform.SetLocalRotation(MWD::GetQuat(MatInv));
			m_Transform.SetLocalPosition(vcPos);
		};
		void CreateFromLookDirWorld(const MWDVector3& Pos, const MWDVector3& vcDir, const MWDVector3& vcUp = MWDVector3(0, 1, 0)) {
			MWDMatrix3X3W MatTemp;
			MatTemp.CreateFromLookDir(Pos, vcDir, vcUp);

			MWDMatrix3X3 Mat;
			Mat = MatTemp.Get3X3();
			MWDMatrix3X3 MatInv;
			MatInv.InverseOf(Mat);

			m_Transform.SetWorldRotation(MWD::GetQuat(MatInv));
			m_Transform.SetWorldPosition(Pos);
		};
		void CreateFromLookAtWorld(const MWDVector3& vcPos, const MWDVector3& vcLookAt, const MWDVector3& vcUp = MWDVector3(0, 1, 0)) {
			MWDMatrix3X3W MatTemp;
			MatTemp.CreateFromLookAt(vcPos, vcLookAt, vcUp);

			MWDMatrix3X3 Mat;
			Mat = MatTemp.Get3X3();

			MWDMatrix3X3 MatInv;
			MatInv.InverseOf(Mat);
			m_Transform.SetWorldRotation(MWD::GetQuat(MatInv));
			m_Transform.SetWorldPosition(vcPos);
		};
		bool SetPerspectiveFov(MWDREAL fFov, MWDREAL Aspect, MWDREAL fZN, MWDREAL fZF) {
			m_Fov = fFov;
			m_Aspect = Aspect;
			m_ZFar = fZF;
			m_ZNear = fZN;
			return m_ProjMat.CreatePerspective(m_Fov, m_Aspect, m_ZNear, m_ZFar);
		};
		bool SetAspect(MWDREAL Aspect) {
			m_Aspect = Aspect;
			return m_ProjMat.CreatePerspective(m_Fov, m_Aspect, m_ZNear, m_ZFar);
		};

		bool SetOrthogonal(MWDREAL fW, MWDREAL fH, MWDREAL fZN, MWDREAL fZF) {
			m_ZFar = fZF;
			m_ZNear = fZN;
			return m_ProjMat.CreateOrthogonal(fW, fH, m_ZNear, m_ZFar);
		};
		
		void GetFrustumPoint(MWDVector3 Point[8]) {
			MWDMatrix3X3W ViewProj = m_ViewMat * m_ProjMat;

			MWDMatrix3X3W ViewProjInv = ViewProj.GetInverse();

			Point[0] = MWDVector3(1, 1, 0);
			Point[1] = MWDVector3(1, -1, 0);
			Point[2] = MWDVector3(-1, 1, 0);
			Point[3] = MWDVector3(-1, -1, 0);

			Point[4] = MWDVector3(1, 1, 1);
			Point[5] = MWDVector3(1, -1, 1);
			Point[6] = MWDVector3(-1, 1, 1);
			Point[7] = MWDVector3(-1, -1, 1);

			for (unsigned int i = 0; i < 8; i++)
			{
				 MWD::GetV3(MWD::Mat3x3WMulVec3(ViewProjInv ,Point[i] ),Point[i]) ;
			}
		};
		MWDAABB3 GetFrustumAABB() {
			MWDVector3 Point[8];
			GetFrustumPoint(Point);

			MWDAABB3 Aabb;
			Aabb.CreateAABB(Point, 8);

			return Aabb;
		};
		void GetPlane(MWDPlane3 Plane[MWDCamera::CP_MAX])const {
			MWDMatrix3X3W ViewProj;
			ViewProj = m_ViewMat * m_ProjMat;

			MWDVector3 N;
			MWDREAL  fD;

			// right plane
			N.x = -(ViewProj._03 - ViewProj._00);
			N.y = -(ViewProj._13 - ViewProj._10);
			N.z = -(ViewProj._23 - ViewProj._20);
			fD = -(ViewProj._33 - ViewProj._30);
			Plane[0].Set(N, fD);

			// left plane

			N.x = -(ViewProj._03 + ViewProj._00);
			N.y = -(ViewProj._13 + ViewProj._10);
			N.z = -(ViewProj._23 + ViewProj._20);
			fD = -(ViewProj._33 + ViewProj._30);
			Plane[1].Set(N, fD);

			// top plane
			N.x = -(ViewProj._03 - ViewProj._01);
			N.y = -(ViewProj._13 - ViewProj._11);
			N.z = -(ViewProj._23 - ViewProj._21);
			fD = -(ViewProj._33 - ViewProj._31);
			Plane[2].Set(N, fD);
			// bottom plane
			N.x = -(ViewProj._03 + ViewProj._01);
			N.y = -(ViewProj._13 + ViewProj._11);
			N.z = -(ViewProj._23 + ViewProj._21);
			fD = -(ViewProj._33 + ViewProj._31);
			Plane[3].Set(N, fD);

			// far plane
			N.x = -(ViewProj._03 - ViewProj._02);
			N.y = -(ViewProj._13 - ViewProj._12);
			N.z = -(ViewProj._23 - ViewProj._22);
			fD = -(ViewProj._33 - ViewProj._32);
			Plane[4].Set(N, fD);

			// near plane
			N.x = -ViewProj._02;
			N.y = -ViewProj._12;
			N.z = -ViewProj._22;
			fD = -ViewProj._32;
			Plane[5].Set(N, fD);
		};
		FORCEINLINE MWDREAL GetZFar()const
		{
			return m_ZFar;
		}
		FORCEINLINE MWDREAL GetZNear()const
		{
			return m_ZNear;
		}
		FORCEINLINE MWDREAL GetAspect()const
		{
			return m_Aspect;
		}
		FORCEINLINE MWDREAL GetFov()const
		{
			return m_Fov;
		}
		FORCEINLINE void ClearProject()
		{
			m_ProjMat.Identity();
		}
		void AddCustomCullPlane(const MWDPlane3& P) {
			m_CustomCullPlane.AddElement(P);
		};
		void ClearCustomCullPlane() {
			m_CustomCullPlane.Clear();
		};
		MWDArray<MWDPlane3> m_CustomCullPlane;

		//未实现方法
		virtual void UpdateCameraState(double dAppTime) {

		};
		static MWDCamera* GetMainCamera() {
			if (!ms_MainCamera) {
				ms_MainCamera = new MWDCamera((TCHAR*)_T("MainCamera"));
			}
			return ms_MainCamera;
		}
	protected:
		MWDMatrix3X3W	m_ViewMat;		
		MWDMatrix3X3W	m_ProjMat;

		MWDTransform	m_Transform;

		static inline MWDCamera* ms_MainCamera = NULL;
		
		MWDREAL		m_Fov;
		MWDREAL		m_Aspect;
		MWDREAL		m_ZFar;
		MWDREAL		m_ZNear;

	public:
		//未实现方法
		virtual void UpdateTransform(double dAppTime) {
			if (m_bIsChanged)
			{
				
			}
			m_bIsChanged = false;
		};

	};

}


