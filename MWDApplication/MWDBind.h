#pragma once
#include <../MWDApplication/Graphic/Core/MWDGraphic.h>
#include <../MWDApplication/Graphic/Core/MWDObject.h>
#include <../MWDApplication/Graphic/Core/MWDProperty.h>
#include <../MWDApplication/Graphic/Core/MWDName.h>
#include <MWDCompute.h>
namespace MWDEngine
{
	//所有API层资源的父类
	class MWDOpenglResource
	{
	public:
		virtual ~MWDOpenglResource() {/**/ }
	protected:
		MWDOpenglResource() {/**/ }
	};

	//所有引擎渲染资源的父类，提供所有渲染资源的通用接口:添加opengl资源，
	class MWDBind : public MWDObject
	{
		DECLARE_RTTI_NoCreateFun(MWDBind, MWDObject)
		DECLARE_INITIAL_WITH_INIT_TERMINAL(MWDBind)
	public:
		enum	//Lock State
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
		enum	// Clear State
		{
			MCS_NONE,				//初始未操作状态
			MCS_READY,				//准备状态
			MCS_DONE,				//完成状态
			MCS_MAX
		};
		//默认是静态的
		MWDBind(unsigned int uiSwapChainNum = 3,unsigned int uiMemType = MT_BOTH, unsigned int uiClearState = MCS_NONE, unsigned int uiLockFlag = LF_READONLY) {
			m_uiSwapChainNum = uiSwapChainNum;
			m_uiCurID = 0;
			m_uiLockFlag = uiLockFlag;
			m_uiMemType = uiMemType;
			m_uiClearState = uiClearState;
		};
		virtual ~MWDBind() {
			m_resourceArray.Destroy();
		};

	protected:
		//当前保存的渲染资源列表
		MWDArray<MWDOpenglResource*> m_resourceArray;
		//多缓冲数量
		unsigned int m_uiSwapChainNum;        //get,set
		//当前缓冲ID
		unsigned int m_uiCurID;

		//锁定标志
		unsigned int m_uiLockFlag;           //get,set
		//内存类型
		unsigned int m_uiMemType;            //get,set
		//清除状态
		unsigned int m_uiClearState;         //get,set
	public:
		//获取当前(M_CurID)的opengl资源，LoadResource中使用
		virtual MWDOpenglResource* GetResource() {
			if (!m_resourceArray.GetNum())
			{
				return NULL;
			}
			MWDOpenglResource* pID = NULL;
			pID = m_resourceArray[m_uiCurID];
			SwitchResource();
			return pID;
		};
	protected:
		//在子类的构造函数里使用!
		void AddResource(MWDOpenglResource* resource) {
			if (!resource) {
				return;
			}
			m_resourceArray.AddElement(resource);
		};
		
		//只保留显存数据的话，需要调用这个清空内存数据
		virtual void ClearInfo() {
			
		};        
	public:
		friend class MWDRenderer;
		virtual void LoadResource(MWDRenderer* pRenderer) {};   //向Renderer提交opengl资源

		unsigned int GetOpenglResourceNum() const {
			return m_resourceArray.GetNum();
		};

		//切换缓冲
		void SwitchResource() {
			m_uiCurID = (m_uiCurID + 1) % m_uiSwapChainNum;
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
	};
	DECLARE_Ptr(MWDBind);
	MWDTYPE_MARCO(MWDBind);

}



