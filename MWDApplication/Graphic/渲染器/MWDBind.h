#pragma once
#include <../MWDApplication/Graphic/Core/MWDGraphic.h>
#include <../MWDApplication/Graphic/Core/MWDProperty.h>
#include "../场景管理/MWDWorld.h"
#include <MWDCompute.h>
namespace MWDEngine
{
	//所有API层资源的父类
	class MWDOpenglResource
	{
	public:
		virtual ~MWDOpenglResource() {/**/ }
		MWDName GetName() const {
			return m_Name;
		}
		void SetName(const TCHAR* name) {
			m_Name = MWDName(name);
		}
	protected:
		MWDName m_Name;
		MWDOpenglResource() {/**/ }
	};

	//所有引擎渲染资源的父类，提供所有渲染资源的通用接口:添加opengl资源，
	class MWDBind : public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDBind, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBind)
	public:
		enum	//Lock Flag
		{
			LF_NOOVERWRITE,			//不允许重写数据
			LF_DISCARD,				//丢弃所有数据
			LF_READONLY,			//只能读取数据
			LF_MAX
		};
		enum	//Mem Type
		{
			MT_BOTH,				//保留内存和显存的数据
			MT_RAM,					//保留内存数据
			MT_VRAM,				//保留显存数据
			MT_MAX
		};
		//当前资源状态
		enum	// MemType Clear State
		{
			MCS_NONE,				//初始未操作状态
			MCS_READY,				//准备状态
			MCS_DONE,				//完成状态
			MCS_MAX
		};
		MWDBind() {};
		//默认是静态的
		MWDBind(unsigned int uiMemType) {
			m_resourceArray.Clear();
			m_bIsStatic = 1;
			m_uiSwapChainNum = 1;
			m_uiCurID = 0;
			m_uiLockFlag = LF_NOOVERWRITE;
			m_pUser = NULL;
			m_uiMemType = uiMemType;
			m_uiClearState = MCS_NONE;
			GetStaticBindArray().AddElement(this);
			//cout << GetStaticBindArray().GetNum() << endl;
		};
		virtual ~MWDBind() {
			m_resourceArray.Destroy();
		};

		static MWDArray<MWDBind*>& GetDynamicTwoBindArray() {
			return ms_DynamicTwoBindArray;
		}
		static MWDArray<MWDBind*>& GetStaticBindArray() {
			return ms_StaticBindArray;
		}

	protected:
		virtual void ClearInfo() {};
		void AddOpenglResource(MWDOpenglResource* pOpenglResource) {
			if (!pOpenglResource)
				return;
			m_resourceArray.AddElement(pOpenglResource);
		};

		static inline MWDArray<MWDBind*> ms_DynamicTwoBindArray ;
		static inline MWDArray<MWDBind*> ms_StaticBindArray ;

		//当前保存的渲染资源列表
		MWDArray<MWDOpenglResource*> m_resourceArray;

		//多缓冲数量
		unsigned int m_uiSwapChainNum;
		//当前缓冲ID
		unsigned int m_uiCurID;
		//是否是静态单缓冲资源
		bool m_bIsStatic;
		//维护一个资源使用者
		MWDRenderer* m_pUser;
		//锁定标志
		unsigned int m_uiLockFlag;
		//内存类型
		unsigned int m_uiMemType;
		//清除状态
		unsigned int m_uiClearState;

	public:
		//未实现方法
		virtual	bool LoadResource(MWDRenderer* pRender) {
			if (!pRender)
				return 0;
			if (m_uiMemType == MT_RAM)
			{
				return 1;
			}
			if (m_uiSwapChainNum == m_resourceArray.GetNum())
				return 1;
			else
			{
				m_pUser = pRender;
				for (unsigned int i = 0; i < m_uiSwapChainNum; i++)
				{
					MWDOpenglResource* pID = NULL;
					if (!OnLoadResource(pID))
						return 0;
					if (!pID)
						return 0;
					AddOpenglResource(pID);
				}
				/*if (!MWDResourceManager::ms_bRenderThread)
				{
					ClearInfo();
				}*/
				return 1;

			}
		};
		virtual bool ReleaseResource() {
			for (unsigned int i = 0; i < m_resourceArray.GetNum(); i++)
			{
				MWDOpenglResource* res = m_resourceArray[i];
				if (!OnReleaseResource(res))
					return 0;
			}
			m_resourceArray.Clear();
			return 1;
		};

		MWDOpenglResource* GetOpenglResource() {
			if (!m_resourceArray.GetNum())
			{
				return NULL;
			}
			MWDOpenglResource* pID = NULL;
			pID = m_resourceArray[m_uiCurID];
			return pID;
		};
		unsigned int GetOpenglResourceNum() const {
			return m_resourceArray.GetNum();
		};
		bool IsStatic()const { return m_bIsStatic; }
		//设置为静态：只维护一个opengl资源   设置为动态：动态双缓冲，维护两个opengl资源，用SwitchResource()切换资源。
		void SetStatic(bool bIsStatic, bool bMulThreadUse) {
			if (!m_bIsStatic && m_uiSwapChainNum == 2)
			{
				//如果在动态数组里找到当前资源，就删掉，
				MWDArray<MWDBind*> ms_DynamicTwoBindArray = GetDynamicTwoBindArray();
				for (unsigned int i = 0; i < ms_DynamicTwoBindArray.GetNum(); i++)
				{
					if (ms_DynamicTwoBindArray[i] == this)
					{
						ms_DynamicTwoBindArray.Erase(i, i);
						break;
					}
				}
			}
			m_bIsStatic = bIsStatic;

			if (m_bIsStatic)
			{
				m_uiSwapChainNum = 1;
			}
			else
			{
				/*if (MWDResourceManager::ms_bRenderThread && bMulThreadUse && MWDRenderer::ms_pRenderer->IsSupportMulBufferSwtich())
				{

					m_uiSwapChainNum = 2;
					ms_DynamicTwoBindArray.AddElement(this);

				}
				else
				{
					m_uiSwapChainNum = 1;
				}*/
				m_uiSwapChainNum = 1;
			}

			m_uiCurID = 0;
		};
		//切换缓冲
		void SwitchResource() {
			if (!m_bIsStatic && m_uiSwapChainNum == 2)
			{
				m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
			}
		};
		virtual void ASYNClearInfo() {
			if (m_uiMemType == MT_VRAM)
			{
				if (m_uiSwapChainNum == m_resourceArray.GetNum())
				{
					if (m_uiClearState == MCS_NONE)
					{
						m_uiClearState = MCS_READY;
						return;
					}
					else if (m_uiClearState == MCS_READY)
					{
						m_uiClearState = MCS_DONE;
						ClearInfo();
						return;
					}
				}
			}
		};
		virtual unsigned int GetByteSize()const {
			return 0;
		};
		FORCEINLINE void SetLockFlag(unsigned int uiLockFlag)
		{
			if (uiLockFlag > LF_MAX)
			{
				uiLockFlag = LF_NOOVERWRITE;
			}
			m_uiLockFlag = uiLockFlag;
		}
		FORCEINLINE unsigned int GetLockFlag()const
		{
			return m_uiLockFlag;
		}
		FORCEINLINE void SetMemType(unsigned int uiMemType)
		{
			if (uiMemType < MT_MAX)
			{
				m_uiMemType = uiMemType;
			}
		}
		FORCEINLINE unsigned int GetMemType()const
		{
			return m_uiMemType;
		}
		FORCEINLINE unsigned int GetSwapChainNum()const
		{
			return m_uiSwapChainNum;
		}
	protected:
		virtual bool OnLoadResource(MWDOpenglResource*& pID) { return true; };
		virtual bool OnReleaseResource(MWDOpenglResource* pID) { return true; };
	};
	DECLARE_Ptr(MWDBind);
	MWDTYPE_MARCO(MWDBind);

}



