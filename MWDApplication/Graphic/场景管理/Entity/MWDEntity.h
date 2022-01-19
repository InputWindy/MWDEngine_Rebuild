#pragma once
#include "../../Core/MWDGraphic.h"
#include "../../Core/MWDObject.h"
#include "../���/MWDComponent.h"
#include <MWDCompute.h>
namespace MWDEngine {
	class MWDScene;
	//Entity��ҲӦ���Ǹ�����࣬�����ɸ��ֲ�ͬ�ľ���ʵ��,ÿ�־����ʵ����ʵ������ʱ��Ҫ��������AABB3��
	class  MWDEntity:public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDEntity, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDEntity)
	public:
		~MWDEntity();
		void SetName(MWDName& name) {
			m_Name = name;
			m_bIsChanged = true;
		}
		void SetName(const TCHAR* name) {
			m_Name = MWDName(name);
			m_bIsChanged = true;
		}
		MWDName GetName()const {
			return m_Name;
		}
		
		MWDAABB3 GetAABB()const {
			return m_AABB3;
		}
		void SetAABB(const MWDAABB3& aabb3) {
			m_AABB3 = aabb3;
		}

		void SetID(unsigned int ID) {
			m_ID = ID;
			m_bIsChanged = true;
		}
		unsigned int GetID()const {
			return m_ID;
		}

		MWDScene* GetScene()const;
		void SetScene(MWDScene* scene);

		void SetParent(MWDEntity* parent) {
			m_pParent = parent;
			parent->AddChild(this);
			m_bIsChanged = true;
		};
		MWDEntity* GetParent()const {
			return m_pParent;
		};

		void AddChild(MWDEntity* child) {
			child->m_pParent = this;
			if (!FindChild(child)) {
				m_pChildren.AddElement(child);
			}
			m_bIsChanged = true;
		};
		void DeleteChildByName(MWDName& name) {
			m_bIsChanged = true;
			for (int j = 0; j < GetChildCountByName(name); j++) {
				for (int i = 0; i < m_pChildren.GetNum(); i++) {
					if (m_pChildren[i]->GetName() == name) {
						m_pChildren.Erase(i, i);
					}
				}
			}
		};
		void DeleteChildByID(unsigned int ID) {
			m_bIsChanged = true;
			for (int j = 0; j < GetChildCountByID(ID); j++) {
				for (int i = 0; i < m_pChildren.GetNum(); i++) {
					if (m_pChildren[i]->GetID() == ID) {
						m_pChildren.Erase(i, i);
					}
				}
			}
		};
		//ֻ��ȡ�Լ����ӽڵ�
		MWDArray<MWDEntity*> GetChildren() const {
			return m_pChildren;
		};
		//��ȡ�Լ�֮��������ӽڵ�
		MWDArray<MWDEntity*> GetAllChildren() const {
			MWDArray<MWDEntity*> ret = m_pChildren;
			for (int i = 0; i < m_pChildren.GetNum(); i++) {
				ret+= m_pChildren[i]->GetAllChildren();
			}
			return ret;
		};
		//ֻ��ȡ�Լ����ӽڵ���
		unsigned int GetChildCount()const {
			return m_pChildren.GetNum();
		};
		//��ȡ�Լ�֮��������ӽڵ���
		unsigned int GetAllChildCount()const {
			int cnt = GetChildCount();
			for (int i = 0; i < m_pChildren.GetNum(); i++) {
				cnt += m_pChildren[i]->GetAllChildCount();
			}
			return cnt;
		};
		unsigned int GetChildCountByName(MWDName& name)const {
			int cnt = 0;
			for (int i = 0; i < GetChildCount(); i++) {
				if (m_pChildren[i]->GetName() == name) {
					++cnt;
				}
			}
			return cnt;
		};
		unsigned int GetChildCountByID(unsigned int ID)const {
			int cnt = 0;
			for (int i = 0; i < GetChildCount(); i++) {
				if (m_pChildren[i]->GetID() == ID) {
					++cnt;
				}
			}
			return cnt;
		};
		
		MWDArray<MWDComponent*> GetComponents()const {
			return  m_pComponent;
		}
		MWDComponent* GetComponentByName(const MWDName& name) {
			for (int i = 0; i < GetComponentNum(); i++) {
				//cout << m_pComponent[i]->GetName().GetID()<<" "<<name.GetID()<<endl;
				if (m_pComponent[i]->GetName() == name) {
					return m_pComponent[i];
				}
			}
		}
		//������������⣬ֻ�᷵�ص�һ������ĵ�ַ��ԭ������
		template<typename T>
		T* GetComponentByType() {
			for (int i = 0; i < GetComponentNum(); i++) {
				if ((m_pComponent[i])->GetComponentType() == T::m_uiType) {
					return (T*)m_pComponent[i];
				}
			}
			return NULL;
		}
		
		//���˽ű�����������������������Ψһ��
		template<typename T>
		void AddComponent(T& component) {
			if (T::m_uiType != MWDComponent::SCRIPT) {
				T* tmp = GetComponentByType<T>();
				if (tmp) {
					return;
				}
			}
			m_pComponent.AddElement(&component);
			m_bIsChanged = true;
		}
		void DeleteComponentByName(const MWDName& name) {
			unsigned int idx = m_pComponent.FindElement(&(*GetComponentByName(name)));
			//cout << "idx="<<idx << endl;
			m_pComponent.Erase(idx, idx);
			m_bIsChanged = true;
		}
		
		//Transform�������ɾ��������ֻ��һ��
		template<typename T>
		void DeleteComponentByType() {
			if (T::m_uiType == MWDComponent::TRANSFORM) {
				return;
			}
			if (T::m_uiType != MWDComponent::SCRIPT) {
				unsigned int idx = m_pComponent.FindElement(&(*GetComponentByType<T>()));
				m_pComponent.Erase(idx,idx);
				m_bIsChanged = true;
			}
			return;
		}
		unsigned int GetComponentNum()const {
			return m_pComponent.GetNum();
		}
		bool FindChild(MWDEntity* child) const {
			for (int i = 0; i < GetChildCount(); i++) {
				if (m_pChildren[i] == child) {
					return true;
				}
			}
			return false;
		}


		void PrintAllComponentsName() const{
			cout << "===============AllComponentsName_Tittle=================" << endl;
			for (int i = 0; i < GetComponentNum(); i++) {
				GetComponents()[i]->GetName().Print();
			}
			cout << "===============AllComponentsName_Bottom=================" << endl;
		}
		
	protected:
		MWDEntity() {};

		bool m_bActive;
		bool m_bIsChanged;

		MWDEntity* m_pParent;
		MWDArray<MWDEntity*> m_pChildren;

		MWDName m_Name;
		unsigned int m_ID;

		MWDScene* m_Scene;

		MWDArray<MWDComponent*> m_pComponent;

		MWDAABB3 m_AABB3;
	};
	DECLARE_Ptr(MWDEntity);
	MWDTYPE_MARCO(MWDEntity);
}


