#pragma once
#include "../MWDApplication/Graphic/Core/MWDGraphic.h"
#include "../MWDApplication/Graphic/Core/MWDObject.h"
#include <MWDCompute.h>
namespace MWDEngine {
	//虚基类：节点类的父类
	class MWDNode :public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDNode, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDNode)
	protected:
		bool m_bEnable;
		bool m_bIsChanged;
		bool m_bIsStatic;
		MWDNode() {

		}
		//树节点拓扑结构
		MWDNode* m_pParent;
		MWDArray<MWDNode*> m_pChildren;

		MWDName m_NodeName;

		MWDTransform m_World;
		MWDTransform m_Local;

		MWDAABB3 m_WorldBV;
	public:
		bool m_bInheritScale;
		bool m_bInheritRotate;
		bool m_bInheritTranlate;

		virtual void UpdateTransform(double dAppTime) {};//更新变换信息 
		virtual void UpdateWorldBound(double dAppTime) {};//更新边界体 

		FORCEINLINE  MWDVector3 GetLocalScale()const {
			return m_Local.GetScale();
		};
		FORCEINLINE MWDVector3 GetLocalTranslate()const {
			return m_Local.GetTranslate();
		};
		FORCEINLINE MWDMatrix3X3 GetLocalRotate()const {
			return m_Local.GetRotate();
		};
		FORCEINLINE void GetLocalDir(MWDVector3& Dir, MWDVector3& Up, MWDVector3& Right)const {
			m_Local.GetDir(Dir, Up, Right);
		};
		FORCEINLINE const MWDTransform& GetLocalTransform() {
			return m_Local;
		};

		MWDVector3 GetWorldScale() {
			return m_World.GetScale();
		};
		MWDVector3 GetWorldTranslate() {
			return m_World.GetTranslate();
		};
		MWDMatrix3X3 GetWorldRotate() {
			return m_World.GetRotate();
		};
		void GetWorldDir(MWDVector3& Dir, MWDVector3& Up, MWDVector3& Right) {
			m_World.GetDir(Dir, Up, Right);
		};
		MWDTransform GetWorldTransform() {
			return m_World;
		};

		virtual void SetWorldScale(const MWDVector3& fScale) {

		};
		virtual void SetWorldTranslate(const MWDVector3& Translate) {

		};
		virtual void SetWorldRotate(const MWDMatrix3X3& Rotate) {

		};
		virtual void SetWorldTransform(const MWDTransform& LocalTransform) {

		};
		virtual void SetWorldMat(const MWDMatrix3X3W MWDMat) {

		};

		virtual void SetLocalScale(const MWDVector3& fScale) {

		};
		virtual void SetLocalTranslate(const MWDVector3& Translate) {

		};
		virtual void SetLocalRotate(const MWDMatrix3X3& Rotate) {

		};
		virtual void SetLocalTransform(const MWDTransform& LocalTransform) {

		};
		virtual void SetLocalMat(const MWDMatrix3X3W MWDMat) {

		};

	public:
		virtual ~MWDNode() {
			ClearAllChild();
		};
		bool IsEnable() {
			return m_bEnable;
		}
		void SetEnable(bool en){
			m_bEnable = en;
		}
		bool IsChanged() {
			return m_bIsChanged;
		}
		MWDName GetNodeName() const{
			return m_NodeName;
		}
		void SetNodeName(const TCHAR* name) {
			m_NodeName = MWDName(name);
		}
		FORCEINLINE void SetParent(MWDNode* pParent) {
			m_pParent = pParent;
			m_bIsChanged = true;
		};
		FORCEINLINE MWDNode* GetParent()const {
			return m_pParent;
		};

		FORCEINLINE MWDArray<MWDNode*> GetChildren()const {
			return m_pChildren;
		}
		FORCEINLINE unsigned int GetChildrenNum()const {
			return m_pChildren.GetNum();
		}
		void AddChild(MWDNode* child_node) {
			m_pChildren.AddElement(child_node);
			m_bIsChanged = true;
		}
		void DeleteChild(MWDNode* child_node) {
			int num = m_pChildren.GetNum();
			for (int i = 0; i < num; i++) {
				if (child_node == m_pChildren[i]) {
					m_pChildren.Erase(i, i);
					break;
				}
			}
			m_bIsChanged = true;
		}
		void DeleteChild(unsigned int index) {
			m_pChildren.Erase(index, index);
			m_bIsChanged = true;
		}
		//清空整棵树
		void ClearAllChild() {
			for (int i = 0; i < GetChildrenNum(); i++) {
				m_pChildren[i]->ClearAllChild();
			}
			m_pChildren.Destroy();
			m_bIsChanged = true;
		}
		void upDate() {
			m_bIsChanged = false;
		}

		MWDAABB3 GetWorldBV()const {
			return m_WorldBV;
		}
		FORCEINLINE void SetDynamic(bool bIsDynamic);
		FORCEINLINE bool IsDynamic()const { return !m_bIsStatic; }
	
	};
}

