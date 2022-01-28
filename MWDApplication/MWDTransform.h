#pragma once
#include "MWDComponent.h"
namespace MWDEngine {
	class MWDEntity;
	//transform组件负责维护三维GameObject的position，scale，rotation
	class  MWDTransform:public MWDComponent
	{
		DECLARE_CLASS_FUNCTION(MWDTransform)
		DECLARE_RTTI(MWDTransform, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDTransform)
	private:
		//动态趋势
		MWDVector3 m_fScale;
		MWDMatrix3X3 m_mRotate;
		MWDVector3 m_vTranslate;

		//世界坐标(静态坐标)
		MWDVector3 m_WorldPosition;
		MWDVector3 m_WorldScale;
		MWDVector3 m_WorldEulerAngles;
		MWDQuat    m_WorldRotation;

		//世界坐标系下的三个方向轴(只读:通过update来更新)
		MWDVector3 m_WorldUp;
		MWDVector3 m_WorldRight;
		MWDVector3 m_WorldForward;

		//相对于父节点（如果是根节点则跟上面一样）
		MWDVector3 m_LocalPosition;
		MWDVector3 m_LocalScale;
		MWDVector3 m_LocalEulerAngles;
		MWDQuat	   m_LocalRotation;
		
		//拓扑
		MWDTransform* m_Parent;
		MWDTransform* m_Root;
		unsigned int m_ChildCount;

		//(只读:通过update来更新)
		MWDMatrix3X3W m_WorldToLocalMatrix;
		MWDMatrix3X3W m_LocalToWorldMatrix;
		
	public:
		//组件类型
		static inline unsigned int m_uiType = MWDComponent::TRANSFORM;
		unsigned int GetComponentType() {
			return TRANSFORM;
		};
		virtual void ComponentInfoLog()const;
		void SetEntity(MWDEntity* entity);
		MWDTransform(MWDEntity* Owner = NULL){
			m_Name = MWDName(_T("Transform"));
			SetEntity(Owner);
			m_bEnable = true;
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
			//动态趋势
			m_fScale = trans.m_fScale;
			m_mRotate = trans.m_mRotate;
			m_vTranslate = trans.m_vTranslate;

			//世界坐标
			m_WorldPosition = trans.m_WorldPosition;
			m_WorldScale = trans.m_WorldScale;
			m_WorldEulerAngles = trans.m_WorldEulerAngles;
			m_WorldRotation = trans.m_WorldRotation;

			//世界坐标系下的三个方向轴(只读:通过update来更新)
			m_WorldUp = trans.m_WorldUp;
			m_WorldRight = trans.m_WorldRight;
			m_WorldForward = trans.m_WorldForward;

			//相对于父节点（如果是根节点则跟上面一样）
			m_LocalPosition = trans.m_LocalPosition;
			m_LocalScale = trans.m_LocalScale;
			m_LocalEulerAngles = trans.m_LocalEulerAngles;
			m_LocalRotation = trans.m_LocalRotation;

			//拓扑
			m_Parent = trans.m_Parent;
			m_Root = trans.m_Root;
			m_ChildCount = trans.m_ChildCount;

			//(只读:通过update来更新)
			m_WorldToLocalMatrix = trans.m_WorldToLocalMatrix;
			m_LocalToWorldMatrix = trans.m_LocalToWorldMatrix;
			return *this;
		}
		//未实现方法
		void upDate() {
			//更新
			//m_WorldToLocalMatrix,m_LocalToWorldMatrix,m_WorldPosition,m_LocalPosition
			//m_WorldUp，m_WorldRight，m_WorldForward;
		}
		
		//动态趋势
		MWDVector3 GetScale()const {
			return m_fScale;
		} ;
		//动态趋势
		MWDMatrix3X3 GetRotate()const {
			return m_mRotate;
		} 
		//动态趋势
		MWDVector3 GetTranslate()const {
			return m_vTranslate;
		} 

		FORCEINLINE void GetDir(MWDVector3& Dir, MWDVector3& Up, MWDVector3& Right)const {
			Right.x = m_mRotate._00;
			Right.y = m_mRotate._01;
			Right.z = m_mRotate._02;

			Up.x = m_mRotate._10;
			Up.y = m_mRotate._11;
			Up.z = m_mRotate._12;

			Dir.x = m_mRotate._20;
			Dir.y = m_mRotate._21;
			Dir.z = m_mRotate._22;
		};
		FORCEINLINE void SetScale(const MWDVector3& fScale) {
			m_fScale = fScale;
		};
		FORCEINLINE void SetTranslate(const MWDVector3& Translate) {
			m_vTranslate = Translate;
		};
		FORCEINLINE void SetRotate(const MWDMatrix3X3& Rotate) {
			m_mRotate = Rotate;
		};

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

		void SetMatrix(const MWDMatrix3X3W& MWDMat) {
			m_mRotate = MWDMat.Get3X3();
			m_vTranslate = MWDMat.GetTranslation();
			m_mRotate.GetScaleAndRotated(m_fScale);
		}

	};

}

