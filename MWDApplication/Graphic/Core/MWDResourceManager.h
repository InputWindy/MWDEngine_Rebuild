#pragma once
#include "MWDGraphic.h"
#include "MWDPriority.h"
#include "MWDRtti.h"
#include <MWDArray.h>
#include "MWDPointer.h"
#include "MWDResource.h"
#include <MWDDelegate.h>
#include <MWDDelegateList.h>
#include <MWDSynchronize.h>
#include "MWDObject.h"

namespace MWDEngine {
	class  MWDResouceControll
	{
	public:
		MWDResouceControll(unsigned int uiGCMaxTimeCount) {
			m_uiGCMaxTimeCount = uiGCMaxTimeCount;
		};
		virtual ~MWDResouceControll() {
			
		};
		virtual void GCResource() {
			
		};
	protected:
		MWDArray<unsigned int> m_TimeCount;
		unsigned int m_uiGCMaxTimeCount;
	};

	template<class T>
	class MWDResourceArrayControll : public MWDResouceControll
	{
	public:
		MWDResourceArrayControll(unsigned int uiGCMaxTimeCount = 5000) :MWDResouceControll(uiGCMaxTimeCount) {};
		~MWDResourceArrayControll() {};
		bool AddResource(const T& R);
		bool DeleteResource(const T& R);
		//启动GC：遍历所有资源，如果资源只被1个智能指针指向，就开始计时
		virtual void GCResource();
		FORCEINLINE bool IsRealeseAll() { return m_ResourceArray.GetNum() == 0; };
		FORCEINLINE void ClearAll() { m_ResourceArray.Clear(); };
		T  GetResource(unsigned int i);
		FORCEINLINE void ClearTimeCount(unsigned int i) { m_TimeCount[i] = 0; };
		FORCEINLINE unsigned int GetResourceNum() { return m_ResourceArray.GetNum(); };
	protected:
		MWDArrayOrder<T> m_ResourceArray;
	};

	template<class KEY, class VALUE>
	class  MWDResourceSet : public MWDResouceControll
	{
	protected:
		MWDMapOrder<KEY, VALUE> m_Resource;
	public:
		MWDResourceSet(unsigned int uiGCMaxTimeCount = 5000)
			: MWDResouceControll(uiGCMaxTimeCount) 
		{
			m_Resource.Clear();
		};
		~MWDResourceSet() {
			m_Resource.Clear();
		};
		VALUE  CheckIsHaveTheResource(const KEY& Key) {
			unsigned int i = m_Resource.Find(Key);
			if (i == m_Resource.GetNum())
				return NULL;
			m_TimeCount[i] = 0;
			return m_Resource[i].Value;
		};

		bool AddResource(const KEY& Key, VALUE pObject) {
			if (!pObject)
				return 0;


			m_Resource.AddElement(Key, pObject);
			m_TimeCount.AddElement(0);

			return 1;
		};
		bool DeleteResource(const KEY& Key) {
			unsigned int i = m_Resource.Find(Key);
			if (i == m_Resource.GetNum())
				return false;
			m_Resource.Erase(i,i);
			m_TimeCount.Erase(i,i);
			return true;
		};

		FORCEINLINE unsigned int GetResourceNum() { return m_Resource.GetNum(); };

		FORCEINLINE bool IsRealeseAll() { return !m_Resource.GetNum(); };
		FORCEINLINE void ClearAll() { m_Resource.Clear(); };
		const MWDMapElement<KEY, VALUE>* GetResource(unsigned int i) {
			MWDMAC_ASSERT(i < m_Resource.GetNum());
			if (i >= m_Resource.GetNum())
			{
				return NULL;
			}
			return &m_Resource[i];
		};
		void GCResource() {
			unsigned int i = 0;
			while (i < m_Resource.GetNum())
			{
				const MWDMapElement<KEY, VALUE>& Resource = m_Resource[i];
				if (Resource.Value->GetRef() == 1)
				{
					m_TimeCount[i]++;
					if (m_TimeCount[i] > m_uiGCMaxTimeCount)
					{
						m_Resource.Erase(i);
						m_TimeCount.Erase(i);
						break;
					}
					else
					{
						i++;
					}
				}
				else
				{
					m_TimeCount[i] = 0;
					i++;
				}
			}
		};
		unsigned int GetResourceIndexByKey(const KEY& Key)const {
			unsigned int i = m_Resource.Find(Key);
			return i;
		};

	};

	template<class KEY, class VALUE>
	class MWDProxyResourceSet : public MWDResourceSet<KEY, VALUE>
	{
	public:
		MWDProxyResourceSet(unsigned int uiGCMaxTimeCount = 5000)
			:MWDResourceSet<KEY, VALUE>(uiGCMaxTimeCount)
		{
			
		};
		~MWDProxyResourceSet() {};
		void GCResource() {
			unsigned int i = 0;
			while (i < MWDResourceSet<KEY, VALUE>::m_Resource.GetNum())
			{
				const MWDMapElement<KEY, VALUE>& Resource = MWDResourceSet<KEY, VALUE>::m_Resource[i];
				if (Resource.Value->IsEndableGC() && Resource.Value->IsLoaded() && Resource.Value->GetRef() == 1)
				{
					MWDResourceSet<KEY, VALUE>::m_TimeCount[i]++;
					if (MWDResourceSet<KEY, VALUE>::m_TimeCount[i] > MWDResourceSet<KEY, VALUE>::m_uiGCMaxTimeCount)
					{
						MWDResourceSet<KEY, VALUE>::m_Resource.Erase(i);
						MWDResourceSet<KEY, VALUE>::m_TimeCount.Erase(i);
					}
					else
					{
						i++;
					}
				}
				else
				{
					MWDResourceSet<KEY, VALUE>::m_TimeCount[i] = 0;
					i++;
				}
			}
		};
	};

	typedef MWDDelegateEvent0<void> DelayUpdatObjectOneFrame;
	typedef MWDDelegateEvent1<void, MWDREAL> DelayUpdatObject;

	class  MWDResourceManager
	{
		//PRIORITY
		DECLARE_PRIORITY
	public:
		MWDResourceManager() {};
		~MWDResourceManager() {};

		//暂未实现
		static void LoadDefaultResource(unsigned int RenderTypeAPI) {};

		static bool IsReleaseAll();
		static void CacheResource();
		static void GC();

		static MWDResourceProxyBase* LoadResource(const TCHAR* pFileName, bool IsAsyn);

		static void DeleteMapResource(const TCHAR* pFileName);
		static void DeleteAllMapResource();
		DECLARE_INITIAL_ONLY(MWDResourceManager)
	private:

		//暂未实现
		static bool InitialDefaultState() {
			InitConfig();
			/*ms_ResourceRtti.AddElement(&MWDAnimTree::ms_Type);
			ms_ResourceRtti.AddElement(&MWDAnim::ms_Type);
			ms_ResourceRtti.AddElement(&MWDMorphTree::ms_Type);
			ms_ResourceRtti.AddElement(&MWDMaterialInstance::ms_Type);
			ms_ResourceRtti.AddElement(&MWDShader::ms_Type);
			ms_ResourceRtti.AddElement(&MWDMeshNode::ms_Type);
			ms_ResourceRtti.AddElement(&MWDPostEffectSet::ms_Type);
			ms_ResourceRtti.AddElement(&MWDTexAllState::ms_Type);
			ms_ResourceRtti.AddElement(&MWDFont::ms_Type);*/
			return 1;
		};
		static bool TerminalDefaultState() { return true; };

		static bool CacheName() { return true; };

		static void InitConfig() {};

		static bool IsResourceType(MWDRtti& Rtti)
		{
			for (unsigned int i = 0; i < ms_ResourceRtti.GetNum(); i++)
			{
				if (Rtti.IsDerived(*ms_ResourceRtti[i]))
				{
					return true;
				}
			}
			return false;
		}
	protected:
		static inline MWDCriticalSection ms_TextureCri;
		static inline MWDCriticalSection ms_MaterialCri;
		static inline MWDCriticalSection ms_AnimCri;
		static inline MWDCriticalSection ms_StaticMeshCri;
		static inline MWDCriticalSection ms_SkeletonMeshCri;
		static inline MWDCriticalSection ms_MWDhaderCri;
		static inline MWDCriticalSection ms_PShaderCri;
		static inline MWDCriticalSection ms_GShaderCri;
		static inline MWDCriticalSection ms_PostEffectCri;
		static inline MWDCriticalSection ms_NameCri;
		static inline MWDCriticalSection ms_AnimTreeCri;
		static inline MWDCriticalSection ms_MorphTreeCri;
		static inline MWDCriticalSection ms_ActorCri;
		static inline MWDCriticalSection ms_MapCri;
		static inline MWDCriticalSection ms_FontCri;
		static inline MWDArray<MWDRtti*> ms_ResourceRtti;
	public:
		//help function
	public:
		static bool ms_bRenderThread;
		static bool ms_bUpdateThread;

		static unsigned int ms_CurRenderAPIType;
	protected:
		static unsigned int ms_uiGpuSkinBoneNum;
		static bool ms_bDrawSkelecton;
		static bool ms_bActiveDebugDraw;

		static bool ms_bIsCacheShader;
		static bool ms_bIsCacheName;
		static bool ms_bIsCacheVertexFormat;
	public:
		static MWDString ms_ActorPath;
		static MWDString ms_MapPath;
		static MWDString ms_FontPath;
		static MWDString ms_TexturePath;
		static MWDString ms_AnimPath;
		static MWDString ms_MaterialPath;
		static MWDString ms_PostEffectPath;
		static MWDString ms_SkelectionMeshPath;
		static MWDString ms_MorphPath;
		static MWDString ms_OutputShaderCodePath;
		static MWDString ms_OutPutLogPath;
		static MWDString ms_ShaderPath;
		static MWDString ms_StaticMeshPath;
		static MWDString ms_TerrainPath;
		static MWDString ms_ResourcePath;
		static bool ms_EnableAsynLoad;
	protected:
		static inline MWDArray<MWDObject*> ms_pRootObject;
		static inline MWDArrayOrder<MWDObject*> ms_pGCObject;
	public:
		static DelayUpdatObjectOneFrame& GetDelayUpdateObjectOneFrame()
		{
			static DelayUpdatObjectOneFrame s_DelayUpdateObjectOneFrame;
			return s_DelayUpdateObjectOneFrame;
		}
		static DelayUpdatObject& GetDelayUpdateObject()
		{
			static DelayUpdatObject s_DelayUpdateObject;
			return s_DelayUpdateObject;
		}
		static void DelayUpdate(MWDREAL fTime)
		{
			GetDelayUpdateObjectOneFrame()();
			GetDelayUpdateObjectOneFrame().Reset();
			GetDelayUpdateObject()(fTime);
		}
		static void AddRootObject(MWDObject* p)
		{
			p->SetFlag(MWDObject::OF_RootObject);
			ms_pRootObject.AddElement(p);
		}
		
		//删除p
		static void DeleteRootObject(MWDObject* p)
		{
			for (unsigned int i = 0; i < ms_pRootObject.GetNum(); i++)
			{
				if (ms_pRootObject[i] == p)
				{
					p->ClearFlag(MWDObject::OF_RootObject);
					ms_pRootObject.Erase(i);
					break;
				}
			}
		}
		
		//把p加入GC管理
		static void AddGCObject(MWDObject* p)
		{
			ms_pGCObject.AddElement(p);
			p->SetFlag(MWDObject::OF_GCObject);
		}

		static void GCObject() {
				
		};

	protected:
		friend class MWDStream;
		static void RunGCTask();
		static void AddCanGCObject(MWDArray<MWDObject*>& CanGCObject);
	public:
		static void RunAllGCTask();

	};

}