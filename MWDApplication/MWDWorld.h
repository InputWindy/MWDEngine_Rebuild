#pragma once
#include "MWDScene.h"
namespace MWDEngine {
	class  MWDWorld:public MWDObject
	{
		DECLARE_CLASS_FUNCTION(MWDWorld)
		DECLARE_RTTI(MWDWorld, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDWorld)
	public:
		friend class MWDScene;
		MWDWorld() {
			CreateNewScene();
		}
		MWDWorld(MWDName& name) {
			CreateNewScene();
			m_WorldName = name;
		}
		MWDWorld(const TCHAR* name) {
			CreateNewScene();
			m_WorldName = MWDName(name);
		}
		~MWDWorld() {

		}

		MWDName GetWorldName()const {
			return m_WorldName;
		}
		void SetWorldName(MWDName& name) {
			m_WorldName = name;
		}
		void SetWorldName(const TCHAR* name) {
			m_WorldName = MWDName(name);
		}

		MWDArray<MWDScene*> GetSceneArray() const{
			return m_SceneArray;
		}
		MWDScene* GetSceneByName(MWDName& name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return m_SceneArray[i];
				}
			}
			return NULL;
		}
		MWDScene* GetSceneByName(const TCHAR* name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return m_SceneArray[i];
				}
			}
			return NULL;
		}

		MWDArray<MWDEntity*> GetSceneEntityArray() const {
			return m_SceneAllEntity;
		}
		MWDEntity* GetSceneEntityByName(MWDName& name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return m_SceneAllEntity[i];
				}
			}
			return NULL;
		}
		MWDEntity* GetSceneEntityByName(const TCHAR* name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return m_SceneAllEntity[i];
				}
			}
			return NULL;
		}

		unsigned int GetSceneNum()const {
			return m_SceneArray.GetNum();
		}
		unsigned int GetSceneEntityNum()const {
			return m_SceneAllEntity.GetNum();
		}
		bool FindScene(MWDName& name ) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return true;
				}
			}
			return false;
		}
		bool FindScene(const TCHAR* name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					return true;
				}
			}
			return false;
		}
		void CreateNewScene() {
			MWDScene* newScene = new MWDScene(_T("default_scene"));
			newScene->m_World = this;
			m_SceneArray.AddElement(newScene);
		}
		void CreateNewScene(MWDName& name) {
			if (FindScene(name)) {
				return;
			}
			MWDScene* newScene = new MWDScene(name);
			newScene->m_World = this;
			m_SceneArray.AddElement(newScene);
		}
		void CreateNewScene(const TCHAR* name) {
			if (FindScene(name)) {
				return;
			}
			MWDScene* newScene = new MWDScene(name);
			newScene->m_World = this;
			m_SceneArray.AddElement(newScene);
		}
		void DeleteSceneByName(MWDName& name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					m_SceneArray.Erase(i, i);
					return;
				}
			}

		}
		void DeleteSceneByName(const TCHAR* name) {
			for (int i = 0; i < GetSceneNum(); i++) {
				if (m_SceneArray[i]->GetSceneName() == name) {
					m_SceneArray.Erase(i, i);
					return;
				}
			}
		}
		void WorldInfoLog()const {
			cout << "============WorldInfoLog_Tittle=============" << endl;
			cout << "世界名称：" << endl;
			m_WorldName.Print();
			cout << "场景数量：" << endl;
			cout << GetSceneNum() << endl;
			cout << "实体数量：" << endl;
			cout << GetSceneEntityNum() << endl;
			cout << "场景名称：" << endl;
			for (int i = 0; i < GetSceneNum(); i++) {
				m_SceneArray[i]->GetSceneName().Print();
			}
			cout << "实体名称：" << endl;
			for (int i = 0; i < GetSceneEntityNum(); i++) {
				m_SceneAllEntity[i]->GetName().Print();
			}
			cout << "============WorldInfoLog_Bottom=============" << endl;
		}
		static MWDWorld* GetMainWorld() {
			if (!ms_MainWorld) {
				ms_MainWorld = new MWDWorld((TCHAR*)_T("MainWorld"));
			}
			return ms_MainWorld;
		}
	private:
		MWDName m_WorldName;
		MWDArray<MWDScene*> m_SceneArray;
		MWDArray<MWDEntity*> m_SceneAllEntity;

		static inline MWDWorld* ms_MainWorld = NULL; 
	};
}


