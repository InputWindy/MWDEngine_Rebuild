#pragma once
#include <../MWDApplication/Graphic/Core/MWDGraphic.h>
#include <../MWDApplication/Graphic/Core/MWDObject.h>
#include <MWDCompute.h>
namespace MWDEngine {
	class MWDEntity;
	//虚基类：维护
	class MWDComponent :public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDComponent, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDComponent)
	public:
		~MWDComponent() {};
		enum {
			TRANSFORM,
			SCRIPT,
			MESH,
			MATERIAL,
			DirectionalLight,
			SpotLight,
			AreaLight,
			PointLight,
			COMPONENT_MAX
		};
		FORCEINLINE void SetEnable(bool en) {
			m_bEnable = en;
		}
		FORCEINLINE bool GetEnable()const {
			return  m_bEnable ;
		}
		virtual unsigned int GetComponentType() { return COMPONENT_MAX; };
		void SetEntity(MWDEntity* entity);
		MWDEntity* GetEntity()const;
		FORCEINLINE MWDName GetName()const {
			return m_Name;
		}
		FORCEINLINE void SetName(const MWDName& name) {
			m_Name = name;
		}
		virtual void ComponentInfoLog()const {
			
		}
		
	protected:
		MWDComponent() {};
		bool m_bEnable;

		//所属实体
		MWDEntity* m_pEntity;

		//组件名字
		MWDName m_Name;
	};
}

