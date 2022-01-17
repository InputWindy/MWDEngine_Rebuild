#pragma once
#include "MWDComponent.h"
namespace MWDEngine {
	class MWDEntity;
	//transform�������ά����άGameObject��position��scale��rotation
	class  MWDTransform:public MWDComponent
	{
		DECLARE_CLASS_FUNCTION(MWDTransform)
		DECLARE_RTTI(MWDTransform, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTransform)
	private:
		
		//��������
		MWDVector3 m_WorldPosition;
		MWDVector3 m_WorldScale;
		MWDVector3 m_WorldEulerAngles;
		MWDQuat    m_WorldRotation;

		//��������ϵ�µ�����������(ֻ��:ͨ��update������)
		MWDVector3 m_WorldUp;
		MWDVector3 m_WorldRight;
		MWDVector3 m_WorldForward;

		//����ڸ��ڵ㣨����Ǹ��ڵ��������һ����
		MWDVector3 m_LocalPosition;
		MWDVector3 m_LocalScale;
		MWDVector3 m_LocalEulerAngles;
		MWDQuat	   m_LocalRotation;
		
		//����
		MWDTransform* m_Parent;
		MWDTransform* m_Root;
		unsigned int m_ChildCount;

		//(ֻ��:ͨ��update������)
		MWDMatrix3X3W m_WorldToLocalMatrix;
		MWDMatrix3X3W m_LocalToWorldMatrix;
		
	public:
		//�������
		static inline unsigned int m_uiType = MWDComponent::TRANSFORM;
		unsigned int GetComponentType() {
			return TRANSFORM;
		};
		virtual void ComponentInfoLog()const;
		MWDTransform(){
			m_bEnable = true;
			m_Name = MWDName(_T("Transform"));
			m_WorldRight = MWDVector3(MWDVector3::Right());
			m_WorldUp = MWDVector3(MWDVector3::Up());
			m_WorldForward = MWDVector3(MWDVector3::Forward());
			m_WorldScale = MWDVector3(1, 1, 1);
			m_LocalScale = MWDVector3(1, 1, 1);
			m_Parent = NULL;
			m_Root   = NULL;
			m_ChildCount = 0;
		}
		~MWDTransform() {
			m_Parent = NULL;
			m_Root = NULL;
			m_ChildCount = 0;
		}
		MWDTransform& operator = (const MWDTransform& trans) {
			//��������
			m_WorldPosition = trans.m_WorldPosition;
			m_WorldScale = trans.m_WorldScale;
			m_WorldEulerAngles = trans.m_WorldEulerAngles;
			m_WorldRotation = trans.m_WorldRotation;

			//��������ϵ�µ�����������(ֻ��:ͨ��update������)
			m_WorldUp = trans.m_WorldUp;
			m_WorldRight = trans.m_WorldRight;
			m_WorldForward = trans.m_WorldForward;

			//����ڸ��ڵ㣨����Ǹ��ڵ��������һ����
			m_LocalPosition = trans.m_LocalPosition;
			m_LocalScale = trans.m_LocalScale;
			m_LocalEulerAngles = trans.m_LocalEulerAngles;
			m_LocalRotation = trans.m_LocalRotation;

			//����
			m_Parent = trans.m_Parent;
			m_Root = trans.m_Root;
			m_ChildCount = trans.m_ChildCount;

			//(ֻ��:ͨ��update������)
			m_WorldToLocalMatrix = trans.m_WorldToLocalMatrix;
			m_LocalToWorldMatrix = trans.m_LocalToWorldMatrix;
			return *this;
		}
		//δʵ�ַ���
		void upDate() {
			//����
			//m_WorldToLocalMatrix,m_LocalToWorldMatrix,m_WorldPosition,m_LocalPosition
			//m_WorldUp��m_WorldRight��m_WorldForward;
		}
		
		MWDVector3 GetWorldPosition()const {
			return m_WorldPosition;
		}
		void SetWorldPosition(const MWDVector3& worldPos) {
			m_WorldPosition = worldPos;
		}
		MWDVector3 GetWorldScale()const {
			return m_WorldScale;
		}
		void SetWorldScale(const MWDVector3& worldScale) {
			m_WorldScale = worldScale;
		}
		MWDVector3 GetWorldEulerAngles()const {
			return m_WorldEulerAngles;
		}
		void SetWorldEulerAngles(const MWDVector3& worldEulerAngles) {
			m_WorldEulerAngles = worldEulerAngles;
		}
		MWDQuat GetWorldRotation()const {
			return m_WorldRotation;
		}
		void SetWorldRotation(const MWDQuat& worldRot) {
			m_WorldRotation = worldRot;
		}

		MWDVector3 GetWorldUp() const{
			return m_WorldUp;
		}
		MWDVector3 GetWorldRight() const {
			return m_WorldRight;
		}
		MWDVector3 GetWorldForward() const {
			return m_WorldForward;
		}

		MWDVector3 GetLocalPosition()const {
			return m_LocalPosition;
		}
		void SetLocalPosition(const MWDVector3& LocalPos) {
			m_LocalPosition = LocalPos;
		}
		MWDVector3 GetLocalScale()const {
			return m_LocalScale;
		}
		void SetLocalScale(const MWDVector3& LocalScale) {
			m_LocalScale = LocalScale;
		}
		MWDVector3 GetLocalEulerAngles()const {
			return m_LocalEulerAngles;
		}
		void SetLocalEulerAngles(const MWDVector3& LocalEulerAngles) {
			m_LocalEulerAngles = LocalEulerAngles;
		}
		MWDQuat GetLocalRotation()const {
			return m_LocalRotation;
		}
		void SetLocalRotation(const MWDQuat& LocalRot) {
			m_LocalRotation = LocalRot;
		}

		MWDTransform* GetParent()const {
			return m_Parent;
		}
		void SetParent(MWDTransform* parent) {
			m_Parent = parent;
		}
		MWDTransform* GetRoot()const {
			return m_Parent;
		}
		void SetRoot(MWDTransform* root) {
			m_Root = root;
		}

		unsigned int GetChildCount()const;

		MWDMatrix3X3W GetWorldToLocalMatrix()const {
			return m_WorldToLocalMatrix;
		}
		MWDMatrix3X3W GetLocalToWorldMatrix()const {
			return m_LocalToWorldMatrix;
		}

		void LookAt(MWDTransform target) {
			
		};
		void LookAt(MWDVector3 worldPosition) {

		};
		void LookAt(MWDVector3 worldPosition, MWDVector3 worldUp = MWDVector3::Up()) {

		};
		void LookAt(MWDTransform target, MWDVector3 worldUp = MWDVector3::Up()) {

		};

		void Rotate(MWDVector3 eulerAngles) {

		};
		void Rotate(float xAngle, float yAngle, float zAngle) {

		};
		void Rotate(MWDTransform target,MWDVector3 eulerAngles) {

		};
		void Rotate(MWDTransform target, float xAngle, float yAngle, float zAngle) {

		};

		void Translate(MWDVector3 translation) {

		};
		void Translate(float x, float y, float z) {

		};


	};

}

