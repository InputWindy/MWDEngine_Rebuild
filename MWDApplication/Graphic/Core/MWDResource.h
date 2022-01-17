#pragma once
#include "MWDGraphic.h"
#include "MWDName.h"
#include <MWDMap.h>
#include <MWDMemManager.h>
#include "MWDObject.h"
#include <iostream>
using namespace std;
namespace MWDEngine {
	class MWDStream;
	class  MWDResource
	{
	public:
		enum	//Resource Type
		{
			RT_TEXTURE,
			RT_SKELECTON_MODEL,
			RT_STATIC_MODEL,
			RT_ACTION,
			RT_MATERIAL,
			RT_POSTEFFECT,
			RT_SHADER,
			RT_ANIMTREE,
			RT_MORPHTREE,
			RT_TERRAIN,
			RT_FSM,
			RT_ACTOR,
			RT_MAP,
			RT_FONTTYPE,
			RT_MAX
		};
		MWDResource() {};
		virtual ~MWDResource() {};
		FORCEINLINE const MWDName& GetResourceName()const
		{
			return m_ResourceName;
		}
		FORCEINLINE void SetResourceName(const MWDName& ResourceName)
		{
			m_ResourceName = ResourceName;
		}
		virtual unsigned int GetResourceType()const {
			return RT_MAX;
		};
		static const MWDString& GetFileSuffix(unsigned int uiFileSuffix) {
			if (uiFileSuffix > RT_MAX)
				return ms_FileSuffix[RT_MAX];
			return ms_FileSuffix[uiFileSuffix];
		};
	protected:
		//��Դ����
		MWDName	m_ResourceName;
		//�ļ���׺��
		static inline MWDString ms_FileSuffix[] = {
			_T("TEXTURE"),
			_T("SKMODEL"),
			_T("STMODEL"),
			_T("ACTION"),
			_T("MATERIAL"),
			_T("POSTEFFECT"),
			_T("SHADER"),
			_T("ANIMTREE"),
			_T("MORPHTREE"),
			_T("TERRAIN"),
			_T("FSM"),
			_T("ACTOR"),
			_T("MAP"),
			_T("FONT"),
			_T("")
		};
	};

	//��Դ������ࣨ������Դ����,ά��һ������map����Դ���ƣ�
	class  MWDResourceProxyBase : public MWDReference, public MWDMemObject
	{
	public:
		MWDResourceProxyBase() {};
		virtual ~MWDResourceProxyBase() {};
		virtual bool IsEndableASYNLoad() = 0;
		virtual bool IsEndableGC() = 0;
		virtual bool IsLoaded() = 0;
		virtual const MWDName& GetResourceName() = 0;
		virtual unsigned int GetResourceType() = 0;
		//��������loadMap������֮�����map
		void LoadEvent()
		{
			m_bIsLoaded = true;
			for (unsigned int i = 0; i < m_LoadedEventObject.GetNum(); i++)
			{
				//ÿ��obj��Ҫά���Լ��ļ����߼�
				m_LoadedEventObject[i].Key->LoadedEvent(this, m_LoadedEventObject[i].Value);
			}
			m_LoadedEventObject.Destroy();
		}
		//���һ����Ҫ���ص�obj
		void AddLoadEventObject(MWDObject* pObject, int Data = 0)
		{
			if (!pObject)
			{
				return;
			}
			//����Ѿ����ù���LoadEvent()����ֱ�Ӽ������obj��
			if (m_bIsLoaded == false)
			{
				m_LoadedEventObject.AddElement(pObject, Data);
			}
			else
			{
				pObject->LoadedEvent(this);
			}
		}
	protected:
		MWDMap<MWDObjectPtr, int> m_LoadedEventObject;
		bool	m_bIsLoaded;
		MWDName	m_ResourceName;
		void SetResourceName(const MWDName& ResourceName)
		{
			m_ResourceName = ResourceName;
		}
	};
	DECLARE_Ptr(MWDResourceProxyBase);

	template<class T>
	class  MWDResourceProxy : public MWDResourceProxyBase
	{
	public:
		//ÿһ����Դ����Ҫά��һ��GetDefault()�������Լ���Ĭ��ʵ��
		MWDResourceProxy() {
			m_bIsLoaded = false;
			m_pPreResource = (T*)T::GetDefalut();
			m_pResource = NULL;
		};
		virtual ~MWDResourceProxy() {
			m_pPreResource = NULL;
			m_pResource = NULL;
		};
		T* GetResource() {
			if (m_bIsLoaded)
			{
				return m_pResource;
			}
			else
			{
				return m_pPreResource;
			}
		};
		virtual const MWDName& GetResourceName() {
			return m_ResourceName;
		};
		friend class MWDTextureRJob;
		friend class MWDMaterialRJob;
		friend class MWDAnimRJob;
		friend class MWDRefPosRJob;
		friend class MWDStaticMeshRJob;
		friend class MWDSkelectonMeshRJob;
		friend class MWDResourceManager;
		friend class MWDMWDhaderRJob;
		friend class MWDPShaderRJob;
		friend class MWDAnimTreeRJob;
		friend class MWDMorphTreeRJob;
		friend class MWDActorRJob;
		friend class MWDSceneMapRJob;
		friend class MWDFontRJob;
		virtual bool IsLoaded() { return m_bIsLoaded; };
		virtual bool IsEndableASYNLoad() { return T::ms_bIsEnableASYNLoader; };
		virtual bool IsEndableGC() { return T::ms_bIsEnableGC; };
		virtual unsigned int GetResourceType() { return GetResource()->GetResourceType(); };
		void SetNewResource(MWDPointer<T> pResource) {
			m_pResource = pResource;
			m_pResource->SetResourceName(m_ResourceName);
		};
		static inline MWDResourceProxy<T>* Create(T* pResource) {
			MWDResourceProxy<T>* pProxy = new MWDResourceProxy<T>();
			pProxy->SetResourceName(pResource->GetResourceName());
			pProxy->SetNewResource(pResource);
			pProxy->LoadEvent();
			return pProxy;
		};
	protected:
		MWDPointer<T> m_pResource;
		MWDPointer<T> m_pPreResource;
	};

	//����ClassName���͵���Դ��ָ��ClassName������Դ��ֻ��ָ��   ##ClassName##R��##ClassName##RPtr
#define DECLARE_Proxy(ClassName)	\
	typedef MWDResourceProxy<ClassName> ##ClassName##R;\
	typedef MWDPointer<MWDResourceProxy<ClassName>> ##ClassName##RPtr;

}

