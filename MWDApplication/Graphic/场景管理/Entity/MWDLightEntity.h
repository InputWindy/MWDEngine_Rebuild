#pragma once
#include "MWDEntity.h"
#include "../组件/MWDLight.h"
#include "../组件/MWDTransform.h"
namespace MWDEngine {
	class  MWDLightEntity:public MWDEntity
	{
		DECLARE_RTTI_NoCreateFun(MWDLightEntity, MWDEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDLightEntity)
	public:
		enum {
				DirectionalLightEntity,
				PointLightEntity,
				SpotLightEntity,
				AreaLightEntity,
				LE_MAX
		};
		virtual ~MWDLightEntity() {
			
		}
		virtual unsigned int GetLightEntityType() {
			return LE_MAX;
		};
		
		
	protected:
		MWDLightEntity() {
			
		}
	};

	class  MWDDirectionalLightEntity :public MWDLightEntity
	{
		DECLARE_CLASS_FUNCTION(MWDDirectionalLightEntity)
		DECLARE_RTTI(MWDDirectionalLightEntity, MWDLightEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDirectionalLightEntity)
	public:
		MWDDirectionalLightEntity() {
			MWDTransform& trans = *new MWDTransform();
			MWDDirectionalLightComponent& lightComponent = *new MWDDirectionalLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDDirectionalLightComponent>(lightComponent);
			m_bActive = true;
		}
		MWDDirectionalLightEntity(const TCHAR* name) {
			MWDName tmp = MWDName(name);
			SetName(tmp);
			MWDTransform& trans = *new MWDTransform();
			MWDDirectionalLightComponent& lightComponent = *new MWDDirectionalLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDDirectionalLightComponent>(lightComponent);
			m_bActive = true;
		}
		virtual ~MWDDirectionalLightEntity() {

		}
		static inline unsigned int ms_LightEntityType = DirectionalLightEntity;
		virtual unsigned int GetLightEntityType() {
			return DirectionalLightEntity;
		};
		static MWDDirectionalLightEntity* GetDefaultSunLight() {
			if (!ms_DefaultSunLight) {
				ms_DefaultSunLight = new MWDDirectionalLightEntity((TCHAR*)_T("SunLight"));
			}
			return ms_DefaultSunLight;
		}
	protected:
		static inline MWDDirectionalLightEntity* ms_DefaultSunLight = NULL;
		
	};

	class  MWDPointLightEntity :public MWDLightEntity
	{
		DECLARE_CLASS_FUNCTION(MWDPointLightEntity)
		DECLARE_RTTI(MWDPointLightEntity, MWDLightEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDPointLightEntity)
	public:
		MWDPointLightEntity() {
			MWDTransform& trans = *new MWDTransform();
			MWDPointLightComponent& lightComponent = *new MWDPointLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDPointLightComponent>(lightComponent);
			m_bActive = true;
		}
		MWDPointLightEntity(const TCHAR* name) {
			MWDName tmp = MWDName(name);
			SetName(tmp);
			MWDTransform& trans = *new MWDTransform();
			MWDPointLightComponent& lightComponent = *new MWDPointLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDPointLightComponent>(lightComponent);
			m_bActive = true;
		}
		virtual ~MWDPointLightEntity() {

		}
		static inline unsigned int ms_LightEntityType = PointLightEntity;
		virtual unsigned int GetLightEntityType() {
			return PointLightEntity;
		};
	protected:
	};


	class  MWDSpotLightEntity :public MWDLightEntity
	{
		DECLARE_CLASS_FUNCTION(MWDSpotLightEntity)
		DECLARE_RTTI(MWDSpotLightEntity, MWDLightEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDSpotLightEntity)
	public:
		MWDSpotLightEntity() {
			MWDTransform& trans = *new MWDTransform();
			MWDSpotLightComponent& lightComponent = *new MWDSpotLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDSpotLightComponent>(lightComponent);
			m_bActive = true;
		}
		MWDSpotLightEntity(const TCHAR* name) {
			MWDName tmp = MWDName(name);
			SetName(tmp);
			MWDTransform& trans = *new MWDTransform();
			MWDSpotLightComponent& lightComponent = *new MWDSpotLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDSpotLightComponent>(lightComponent);
			m_bActive = true;
		}
		virtual ~MWDSpotLightEntity() {

		}
		static inline unsigned int ms_LightEntityType = SpotLightEntity;
		virtual unsigned int GetLightEntityType() {
			return SpotLightEntity;
		};
	protected:
	};


	class  MWDAreaLightEntity :public MWDLightEntity
	{
		DECLARE_CLASS_FUNCTION(MWDAreaLightEntity)
		DECLARE_RTTI(MWDAreaLightEntity, MWDLightEntity)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDAreaLightEntity)
	public:
		MWDAreaLightEntity() {
			MWDTransform& trans = *new MWDTransform();
			MWDAreaLightComponent& lightComponent = *new MWDAreaLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDAreaLightComponent>(lightComponent);
			m_bActive = true;
		}
		MWDAreaLightEntity(const TCHAR* name) {
			MWDName tmp = MWDName(name);
			SetName(tmp);
			MWDTransform& trans = *new MWDTransform();
			MWDAreaLightComponent& lightComponent = *new MWDAreaLightComponent();
			AddComponent<MWDTransform>(trans);
			AddComponent<MWDAreaLightComponent>(lightComponent);
			m_bActive = true;
		}
		virtual ~MWDAreaLightEntity() {

		}
		static inline unsigned int ms_LightEntityType = AreaLightEntity;
		virtual unsigned int GetLightEntityType() {
			return AreaLightEntity;
		};
	protected:
	};

	class  LightPriority
	{
	public:
		LightPriority() {}
		~LightPriority() {}
		bool operator()(MWDLightEntity* pLight1, MWDLightEntity* pLight2) {
			return 1;
		};
	};




}


