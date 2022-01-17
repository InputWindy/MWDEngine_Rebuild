#pragma once
#include <MWDCompute.h>
#include "../场景管理/Entity/MWDCamera.h"
#include "../场景管理//Entity/MWDLightEntity.h"
namespace MWDEngine {
	class MWDWorld;
	class  MWDScene :public MWDObject
	{
		friend class MWDWorld;
		DECLARE_CLASS_FUNCTION(MWDScene)
		DECLARE_RTTI(MWDScene, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDScene)
	public:
		MWDScene() {
			m_World = NULL;
			m_bIsBuild = false;
			m_bEnable = true;
		};
		MWDScene(const TCHAR* name) {
			m_World = NULL;
			SetSceneName((TCHAR*)name);
			m_bIsBuild = false;
			m_bEnable = true;
		};
		MWDScene(MWDName& name) {
			m_World = NULL;
			SetSceneName(name);
			m_bIsBuild = false;
			m_bEnable = true;
		};
		~MWDScene() {};
		//只读函数
		FORCEINLINE MWDArray<MWDLightEntity*> GetAllLight()const {
			MWDArray<MWDLightEntity*> ret;
			for (int i = 0; i < GetDirectionalLightNum(); i++) {
				ret.AddElement((MWDLightEntity*)(m_pDirectionalLight[i]));
			}
			for (int i = 0; i < GetPointLightNum(); i++) {
				ret.AddElement((MWDLightEntity*)(m_pPointLight[i]));
			}
			for (int i = 0; i < GetSpotLightNum(); i++) {
				ret.AddElement((MWDLightEntity*)(m_pSpotLight[i]));
			}
			for (int i = 0; i < GetAreaLightNum(); i++) {
				ret.AddElement((MWDLightEntity*)(m_pAreaLight[i]));
			}
			return ret;
		};
		FORCEINLINE MWDArray<MWDDirectionalLightEntity*> GetDirectionalLights()const {
			return m_pDirectionalLight;
		};
		FORCEINLINE MWDArray<MWDPointLightEntity*> GetPointLights()const {
			return m_pPointLight;
		};
		FORCEINLINE MWDArray<MWDSpotLightEntity*> GetSpotLights()const {
			return m_pSpotLight;
		};
		FORCEINLINE MWDArray<MWDAreaLightEntity*> GetAreaLights()const {
			return m_pAreaLight;
		};
		FORCEINLINE unsigned int GetAllLightNum()const {
			return m_pAreaLight.GetNum() + m_pDirectionalLight.GetNum() + m_pSpotLight.GetNum() + m_pPointLight.GetNum();
		};
		FORCEINLINE unsigned int GetDirectionalLightNum()const {
			return m_pDirectionalLight.GetNum();
		};
		FORCEINLINE unsigned int GetPointLightNum()const {
			return m_pPointLight.GetNum();
		};
		FORCEINLINE unsigned int GetSpotLightNum()const {
			return m_pSpotLight.GetNum();
		};
		FORCEINLINE unsigned int GetAreaLightNum()const {
			return m_pAreaLight.GetNum();
		};

		FORCEINLINE MWDWorld* GetWorld();

		FORCEINLINE unsigned int GetCameraNum()const {
			return m_pAllCamera.GetNum();
		};
		FORCEINLINE MWDArray<MWDCamera*> GetAllCamera()const {
			return m_pAllCamera;
		}

		FORCEINLINE unsigned int GetStaticEntityNum()const {
			return m_pStatic.GetNum();
		};
		FORCEINLINE MWDArray<MWDEntity*> GetAllStaticEntity()const {
			return m_pStatic;
		}

		FORCEINLINE unsigned int GetDynamicEntityNum()const {
			return m_pDynamic.GetNum();
		};
		FORCEINLINE MWDArray<MWDEntity*> GetAllDynamicEntity()const {
			return m_pDynamic;
		}

		FORCEINLINE bool IsBuild()const {
			return m_bIsBuild;
		}
		FORCEINLINE bool IsEnable()const {
			return m_bEnable;
		}
		FORCEINLINE void SetEnable(bool en) {
			m_bEnable = en;
		}

		FORCEINLINE MWDName GetSceneName()const {
			return m_SceneName;
		}
		FORCEINLINE void SetSceneName(MWDName& name) {
			m_SceneName = name;
		}
		FORCEINLINE void SetSceneName(const TCHAR* name) {
			m_SceneName = MWDName((TCHAR*)name);
		}
		//添加实体没有重名限制，但是不应该重名
		void AddLightEntity(MWDLightEntity* lightEntity) {
			lightEntity->SetScene(this);
			if (lightEntity->GetLightEntityType()== MWDLightEntity::DirectionalLightEntity) {
				m_pDirectionalLight.AddElement((MWDDirectionalLightEntity*)lightEntity);
				return;
			}
			if (lightEntity->GetLightEntityType() == MWDLightEntity::PointLightEntity) {
				m_pPointLight.AddElement((MWDPointLightEntity*)lightEntity);
				return;
			}
			if (lightEntity->GetLightEntityType() == MWDLightEntity::SpotLightEntity) {
				m_pSpotLight.AddElement((MWDSpotLightEntity*)lightEntity);
				return;
			}
			if (lightEntity->GetLightEntityType() == MWDLightEntity::AreaLightEntity) {
				m_pAreaLight.AddElement((MWDAreaLightEntity*)lightEntity);
				return;
			}
			AddWorldEntityArray(lightEntity);
		}
		void DeleteLightEntityByName(const MWDName& name) {
			for (int i = 0; i < GetDirectionalLightNum(); i++) {
				if (m_pDirectionalLight[i]->GetName() == name) {
					m_pDirectionalLight.Erase(i, i);
					DeleteWorldEntityArray(m_pDirectionalLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetSpotLightNum(); i++) {
				if (m_pSpotLight[i]->GetName() == name) {
					m_pSpotLight.Erase(i, i);
					DeleteWorldEntityArray(m_pSpotLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetPointLightNum(); i++) {
				if (m_pPointLight[i]->GetName() == name) {
					m_pPointLight.Erase(i, i);
					DeleteWorldEntityArray(m_pPointLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetAreaLightNum(); i++) {
				if (m_pAreaLight[i]->GetName() == name) {
					m_pAreaLight.Erase(i, i);
					DeleteWorldEntityArray(m_pAreaLight[i]);
					return;
				}
			}
		}
		void DeleteLightEntityByName(const TCHAR* name) {
			MWDName tmp(name);
			for (int i = 0; i < GetDirectionalLightNum(); i++) {
				if (m_pDirectionalLight[i]->GetName() == tmp) {
					m_pDirectionalLight.Erase(i, i);
					DeleteWorldEntityArray(m_pDirectionalLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetSpotLightNum(); i++) {
				if (m_pSpotLight[i]->GetName() == tmp) {
					m_pSpotLight.Erase(i, i);
					DeleteWorldEntityArray(m_pSpotLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetPointLightNum(); i++) {
				if (m_pPointLight[i]->GetName() == tmp) {
					m_pPointLight.Erase(i, i);
					DeleteWorldEntityArray(m_pPointLight[i]);
					return;
				}
			}
			for (int i = 0; i < GetAreaLightNum(); i++) {
				if (m_pAreaLight[i]->GetName() == tmp) {
					m_pAreaLight.Erase(i, i);
					DeleteWorldEntityArray(m_pAreaLight[i]);
					return;
				}
			}
		}
		//添加实体没有重名限制，但是不应该重名
		void AddCamera(MWDCamera* camera) {
			camera->SetScene(this);
			m_pAllCamera.AddElement(camera);
			AddWorldEntityArray(camera);
		}
		void DeleteCameraByName(const MWDName& name) {
			for (int i = 0; i < GetCameraNum(); i++) {
				if (m_pAllCamera[i]->GetName() == name) {
					m_pAllCamera.Erase(i, i);
					DeleteWorldEntityArray(m_pAllCamera[i]);
					return;
				}
			}
		}
		void DeleteCameraByName(const TCHAR* name) {
			MWDName tmp(name);
			for (int i = 0; i < GetCameraNum(); i++) {
				if (m_pAllCamera[i]->GetName() == tmp) {
					m_pAllCamera.Erase(i, i);
					DeleteWorldEntityArray(m_pAllCamera[i]);
					return;
				}
			}
		}

		//添加实体没有重名限制，但是不应该重名
		void AddDynamicEntity(MWDEntity* dEntity) {
			dEntity->SetScene(this);
			m_pDynamic.AddElement(dEntity);
			AddWorldEntityArray(dEntity);
		}
		//添加实体没有重名限制，但是不应该重名
		template<typename TYPE>
		void AddDynamicEntity(const TCHAR* name = _T("")) {
			MWDEntity* tmp = new TYPE(name);
			AddDynamicEntity(tmp);
		}
		void DeleteDynamicEntityByName(const MWDName& name) {
			for (int i = 0; i < GetDynamicEntityNum(); i++) {
				if (m_pDynamic[i]->GetName() == name) {
					DeleteWorldEntityArray(m_pDynamic[i]);
					m_pDynamic.Erase(i, i);
					return;
				}
			}
		}
		void DeleteDynamicEntityByName(const TCHAR* name) {
			MWDName tmp(name);
			for (int i = 0; i < GetDynamicEntityNum(); i++) {
				if (m_pDynamic[i]->GetName() == tmp) {
					DeleteWorldEntityArray(m_pDynamic[i]);
					m_pDynamic.Erase(i, i);
					return;
				}
			}
		}
		//添加实体没有重名限制，但是不应该重名
		void AddStaticEntity(MWDEntity* dEntity) {
			dEntity->SetScene(this);
			m_pStatic.AddElement(dEntity);
			AddWorldEntityArray(dEntity);
		}
		//添加实体没有重名限制，但是不应该重名
		template<typename TYPE>
		void AddStaticEntity(const TCHAR* name = _T("")) {
			MWDEntity* tmp = new TYPE(name);
			AddStaticEntity(tmp);
		}
		void DeleteStaticEntityByName(const MWDName& name) {
			for (int i = 0; i < GetStaticEntityNum(); i++) {
				if (m_pStatic[i]->GetName() == name) {
					DeleteWorldEntityArray(m_pStatic[i]);
					m_pStatic.Erase(i, i);
					return;
				}
			}
		}
		void DeleteStaticEntityByName(const TCHAR* name) {
			MWDName tmp(name);
			for (int i = 0; i < GetStaticEntityNum(); i++) {
				if (m_pStatic[i]->GetName() == tmp) {
					DeleteWorldEntityArray(m_pStatic[i]);
					m_pStatic.Erase(i, i);
					return;
				}
			}
		}

		void SceneInfoLog() const{
			cout << "==========SceneInfoLog_Tittle=======" << endl;
			cout << "场景名称:" << endl;
			GetSceneName().Print();
			if (m_bIsBuild) {
				cout << "IsBuild:" <<"true" << endl;
			}
			else {
				cout << "IsBuild:" << "false" << endl;
			}
			if (m_bEnable) {
				cout << "m_bEnable:" << "true" << endl;
			}
			else {
				cout << "m_bEnable:" << "false" << endl;
			}
			cout << "光源名称(直接光)：" << endl;
			for (int i = 0; i < GetDirectionalLightNum(); i++) {
				m_pDirectionalLight[i]->GetName().Print();
			}
			cout << "光源名称(聚光灯)：" << endl;
			for (int i = 0; i < GetSpotLightNum(); i++) {
				m_pSpotLight[i]->GetName().Print();
			}
			cout << "光源名称(点光源)：" << endl;
			for (int i = 0; i < GetPointLightNum(); i++) {
				m_pPointLight[i]->GetName().Print();
			}
			cout << "光源名称(面光源)：" << endl;
			for (int i = 0; i < GetAreaLightNum(); i++) {
				m_pAreaLight[i]->GetName().Print();
			}
			cout << "摄像机名称：" << endl;
			for (int i = 0; i < GetCameraNum(); i++) {
				m_pAllCamera[i]->GetName().Print();
			}
			cout << "静态实体名称：" << endl;
			for (int i = 0; i < GetStaticEntityNum(); i++) {
				m_pStatic[i]->GetName().Print();
			}
			cout << "动态实体名称：" << endl;
			for (int i = 0; i < GetDynamicEntityNum(); i++) {
				m_pDynamic[i]->GetName().Print();
			}
			cout << "==========SceneInfoLog_Bottom=======" << endl;
		}
		static MWDScene* GetMainScene() {
			if (!ms_MainScene) {
				ms_MainScene = new MWDScene((TCHAR*)_T("MainScene"));
			}
			return ms_MainScene;
		}
	protected:
		MWDArray<MWDDirectionalLightEntity*> m_pDirectionalLight;
		MWDArray<MWDPointLightEntity*> m_pPointLight;
		MWDArray<MWDSpotLightEntity*> m_pSpotLight;
		MWDArray<MWDAreaLightEntity*> m_pAreaLight;
		MWDArray<MWDCamera*> m_pAllCamera;

		MWDArray<MWDEntity*> m_pDynamic;
		MWDArray<MWDEntity*> m_pStatic;
		bool m_bIsBuild;
		bool m_bEnable;

		MWDName m_SceneName;
		MWDWorld* m_World;

		static inline  MWDScene* ms_MainScene = NULL; 
	private:
		void AddWorldEntityArray(MWDEntity* entity);
		void DeleteWorldEntityArray(MWDEntity* entity);
	};
	DECLARE_Ptr(MWDScene);
	MWDTYPE_MARCO(MWDScene);
}


