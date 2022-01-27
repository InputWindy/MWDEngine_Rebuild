#pragma once
#include "MWDComponent.h"
#include "MWDTransform.h"
namespace MWDEngine {
	class  MWDLightComponent:public MWDComponent
	{
	public:
		enum LightType{
			Diretional,
			Spot,
			Point,
			Area,
			Sky,
			LT_MAX
		};
		enum ShadowType {
			Soft,
			Hard,
			None,
			ST_MAX
		};
		virtual void SetEntity(MWDEntity* entity) {};
		~MWDLightComponent() {

		}
		double GetIntensity()const {
			return m_Intensity;
		}
		void SetIntensity(double intensity) {
			m_Intensity = intensity;
		}
		MWDVector3 GetDirection()const {
			return m_Direction;
		}
		void SetDirection(const MWDVector3& dir) {
			m_Direction = dir;
		}
		MWDColorRGBA GetColor()const {
			return m_Color;
		}
		void SetColor(const MWDColorRGBA& color) {
			m_Color = color;
		}

		virtual void ComponentInfoLog() const {
			
		}
	protected:
		MWDLightComponent() {

		}
		double m_Intensity;
		MWDVector3 m_Direction;
		MWDColorRGBA m_Color;

		//MWDShadowMap* shadowMap;
	};

	//方向光没有位置,没有衰减
	class  MWDDirectionalLightComponent :public MWDLightComponent
	{
		DECLARE_CLASS_FUNCTION(MWDDirectionalLightComponent)
		DECLARE_RTTI(MWDDirectionalLightComponent, MWDLightComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDDirectionalLightComponent)
	private:
		unsigned int m_ShadowType;
		//光照类型
		static inline unsigned int m_LightType = Diretional;
		
	public:
		//组件类型
		static inline unsigned int m_uiType = DirectionalLight;
		virtual void SetEntity(MWDEntity* entity);
		unsigned int GetComponentType() {
			return DirectionalLight;
		};
		virtual void ComponentInfoLog() const {
			cout << "=============DirectionalLightComponentInfoLog_Tittle=================" << endl;
			cout << "名称：" << endl;
			GetName().Print();
			cout << "光强：" << endl;
			cout<<m_Intensity<<endl;
			/*cout << "方向：" << endl;
			m_Direction.Print();*/
			cout << "颜色：" << endl;
			m_Color.Print();
			cout << "阴影类型：" << endl;
			if (m_ShadowType == Hard)
				cout << "硬阴影" << endl;
			if (m_ShadowType == Soft)
				cout << "软阴影" << endl;
			if (m_ShadowType == None)
				cout << "无阴影" << endl;
			cout << "=============DirectionalLightComponentInfoLog_Tittle=================" << endl;
		}
		MWDDirectionalLightComponent() {
			m_Name = MWDName(_T("DirectionalLightComponent"));
			m_ShadowType = None;
			m_Intensity = 1.0f;
			m_Direction = MWDVector3(1,1,1);
			m_Color = MWDColorRGBA((unsigned char)0, (unsigned char)0, (unsigned char)0);
		}
		~MWDDirectionalLightComponent() {

		}
		static unsigned int GetLightType(){
			return m_LightType;
		}
		unsigned int GetShadowType() {
			return m_ShadowType;
		}
		void SetShadowType(unsigned int shadowType){
			m_ShadowType = shadowType;
		}
	};

	//点光源没有方向，只有作用范围和衰减函数,初始作用范围10米
	class  MWDPointLightComponent :public MWDLightComponent
	{
		DECLARE_CLASS_FUNCTION(MWDPointLightComponent)
		DECLARE_RTTI(MWDPointLightComponent, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDPointLightComponent)
	private:
		unsigned int m_ShadowType;
		static inline unsigned int m_LightType = Point;
		
		double m_Range;
	public:
		static inline unsigned int m_uiType = PointLight;
		unsigned int GetComponentType() {
			return PointLight;
		};
		virtual void SetEntity(MWDEntity* entity);
		MWDPointLightComponent() {
			m_Name = MWDName(_T("PointLightComponent"));
			m_ShadowType = None;
			m_Intensity = 1.0f;
			m_Range = 10.0f;
			m_Color =MWDColorRGBA((unsigned char)0, (unsigned char)0, (unsigned char)0);
		}
		~MWDPointLightComponent() {

		}
		virtual void ComponentInfoLog() const {
			cout << "=============PointLightComponentInfoLog_Tittle=================" << endl;
			cout << "名称：" << endl;
			GetName().Print();
			cout << "光强：" << endl;
			cout << m_Intensity << endl;
			cout << "方向：" << endl;
			m_Direction.Print();
			cout << "颜色：" << endl;
			m_Color.Print();
			cout << "阴影类型：" << endl;
			if (m_ShadowType == Hard)
				cout << "硬阴影" << endl;
			if (m_ShadowType == Soft)
				cout << "软阴影" << endl;
			if (m_ShadowType == None)
				cout << "无阴影" << endl;
			cout << "光照范围：" << endl;
			cout << m_Range << endl;
			cout << "=============PointLightComponentInfoLog_Tittle=================" << endl;
		}
		static unsigned int GetLightType() {
			return m_LightType;
		}
		unsigned int GetShadowType() {
			return m_ShadowType;
		}
		void SetShadowType(unsigned int shadowType) {
			m_ShadowType = shadowType;
		}
		double GetRange()const {
			return m_Range;
		}
		void SetRange(double range) {
			m_Range = range;
		}
		float ComputeAttenuation(double distance) {
			if (distance > m_Range) {
				return 0;
			}
			int level = distance / 7;	
			float att1 = 0.7f / level;
			float att2 = 1.8f / (level*level);
			return 1.0f/(1.0f + (att1 * distance) + (att2 * distance * distance));
		}
	};

	//面光源衰减速度很快
	class  MWDAreaLightComponent :public MWDLightComponent
	{
		DECLARE_CLASS_FUNCTION(MWDAreaLightComponent)
		DECLARE_RTTI(MWDAreaLightComponent, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDAreaLightComponent)
	private:
		unsigned int m_ShadowType;
		static inline unsigned int m_LightType = Area;
		
		MWDRectangle3 m_LightArea;
	public:
		static inline unsigned int m_uiType = AreaLight;
		unsigned int GetComponentType() {
			return AreaLight;
		};
		virtual void SetEntity(MWDEntity* entity);
		MWDAreaLightComponent() {
			m_Name = MWDName(_T("AreaLightComponent"));
			m_ShadowType = None;
			m_Intensity = 1.0f;
			m_Direction = MWDVector3(1, 1, 1);
			m_Color = MWDColorRGBA((unsigned char)0, (unsigned char)0, (unsigned char)0);
		}
		~MWDAreaLightComponent() {

		}
		virtual void ComponentInfoLog() const {
			cout << "=============AreaLightComponentInfoLog_Tittle=================" << endl;
			cout << "名称：" << endl;
			GetName().Print();
			cout << "光强：" << endl;
			cout << m_Intensity << endl;
			cout << "方向：" << endl;
			m_Direction.Print();
			cout << "颜色：" << endl;
			m_Color.Print();
			cout << "阴影类型：" << endl;
			if (m_ShadowType == Hard)
				cout << "硬阴影" << endl;
			if (m_ShadowType == Soft)
				cout << "软阴影" << endl;
			if (m_ShadowType == None)
				cout << "无阴影" << endl;
			cout << "光源面四角坐标：" << endl;
			MWDVector3 vec3_4[4];
			m_LightArea.GetPoint(vec3_4);
			vec3_4[0].Print();
			vec3_4[1].Print();
			vec3_4[2].Print();
			vec3_4[3].Print();
			cout << "=============AreaLightComponentInfoLog_Tittle=================" << endl;
		}
		static unsigned int GetLightType() {
			return m_LightType;
		}
		unsigned int GetShadowType() {
			return m_ShadowType;
		}
		void SetShadowType(unsigned int shadowType) {
			m_ShadowType = shadowType;
		}
		MWDRectangle3 GetLightArea()const {
			return m_LightArea;
		}
		void SetLightArea(const MWDRectangle3& area) {
			m_LightArea = area;
		}
		float ComputeAttenuation(double distance) {
			float Attenuation = 1.0f / EXP(distance);
			if (Attenuation < 0.1f) {
				return 0;
			}
			return Attenuation;
		}

	};


	//聚光灯：有方向，有张角(初始45°)，有衰减
	class  MWDSpotLightComponent :public MWDLightComponent
	{
		DECLARE_CLASS_FUNCTION(MWDSpotLightComponent)
		DECLARE_RTTI(MWDSpotLightComponent, MWDComponent)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDSpotLightComponent)
	private:
		unsigned int m_ShadowType;
		static inline unsigned int m_LightType = Spot;
		
		float m_AngleRange;
	public:
		static inline unsigned int m_uiType = SpotLight;
		unsigned int GetComponentType() {
			return SpotLight;
		};
		virtual void SetEntity(MWDEntity* entity);
		MWDSpotLightComponent() {
			m_Name = MWDName(_T("SpotLightComponent"));
			m_ShadowType = None;
			m_AngleRange = 45.0f;
			m_Intensity = 1.0f;
			m_Direction = MWDVector3(1, 1, 1);
			m_Color = MWDColorRGBA((unsigned char)0, (unsigned char)0, (unsigned char)0);
		}
		~MWDSpotLightComponent() {

		}
		virtual void ComponentInfoLog() const {
			cout << "=============SpotLightComponentInfoLog_Tittle=================" << endl;
			cout << "名称：" << endl;
			GetName().Print();
			cout << "光强：" << endl;
			cout << m_Intensity << endl;
			cout << "方向：" << endl;
			m_Direction.Print();
			cout << "颜色：" << endl;
			m_Color.Print();
			cout << "阴影类型：" << endl;
			if (m_ShadowType == Hard)
				cout << "硬阴影" << endl;
			if (m_ShadowType == Soft)
				cout << "软阴影" << endl;
			if (m_ShadowType == None)
				cout << "无阴影" << endl;
			cout << "聚光灯张角：" << endl;
			cout << m_AngleRange << endl;
			cout << "=============SpotLightComponentInfoLog_Tittle=================" << endl;
		}
		static unsigned int GetLightType() {
			return m_LightType;
		}
		unsigned int GetShadowType() {
			return m_ShadowType;
		}
		void SetShadowType(unsigned int shadowType) {
			m_ShadowType = shadowType;
		}
		float GetAngleRange() const{
			return m_AngleRange;
		}
		void SetAngleRange(float angleRange) {
			m_AngleRange = angleRange;
		}
		float ComputeAttenuation(double distance) {
			float Attenuation = 1.0f / EXP(distance);
			if (Attenuation < 0.1f) {
				return 0;
			}
			return Attenuation;
		}

	};

}


