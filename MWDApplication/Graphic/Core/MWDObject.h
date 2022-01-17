#pragma once
#include "MWDGraphic.h"
#include <MWDArray.h>
#include <MWDMap.h>
#include "MWDReference.h"
#include "MWDType.h"
#include "MWDRtti.h"
#include "MWDPriority.h"
#include "MWDName.h"
#include "MWDMain.h"
#include <MWDLog.h>
#include <iostream>
using namespace std;

namespace MWDEngine {
	class MWDRtti;
	class MWDObject;
	class MWDStream;
	class MWDResourceProxyBase;
	class MWDRenderer;
	class MWDOpenglResource;
	class MWDPriority;
	typedef MWDObject* (*CreateObjectFun)();
	//MWDObject管理器
	class  MWDFastObjectManager
	{
	public:
		
		enum
		{
			MAX_OBJECT_FLAG = 100000,
			MAX_OBJECT_NUM = MAX_OBJECT_FLAG - 1
		};
	public:
		friend class MWDObject;
		
		MWDFastObjectManager() {
			m_FreeTable.ResetBufferNum(MAX_OBJECT_NUM);
			for (unsigned int i = 0; i < MAX_OBJECT_NUM; i++)
			{
				m_FreeTable[i] = i;
				m_ObjectArray[i] = NULL;
			}
			m_ObjectArray[MAX_OBJECT_NUM] = NULL;
		};
		~MWDFastObjectManager() {
			
		};
		//新建Obj的时候分配一个ID，并且把Obj的指针保存在管理器的列表里
		unsigned int AddObject(MWDObject* p);

		//回收ID，清除指针，并且给被销毁的p的ID设置为标志值，表示暂时已经销毁
		void DeleteObject(MWDObject* p);

		//判断Obj管理器是否已经没有可用的ID了
		FORCEINLINE bool IsClear() { return m_FreeTable.GetNum() == MAX_OBJECT_NUM; };

		//返回Obj管理器已有Obj容量
		FORCEINLINE unsigned int GetObjectNum() { return MAX_OBJECT_NUM - m_FreeTable.GetNum(); };
	protected:
		//这里是保存Obj的表
		MWDObject* m_ObjectArray[MAX_OBJECT_FLAG];

		//这里是给Obj分配ID的对照表
		MWDArray<unsigned int> m_FreeTable;

	};

	//所有的MWDObject都支持智能指针和内存管理（维护一个ID和一个flag,还有一个ms_ClassFactory）
	//MWDObject保存有所有子类的工厂函数，可以通过类名来生成一个子类实例
	//MWDObject管理的Obj都支持垃圾回收
	
	class  MWDObject :public MWDReference, public MWDMemObject
	{
		DECLARE_RTTI_NoParent_NoCreateFun(MWDObject)
		DECLARE_INITIAL_ONLY(MWDObject)
	public:
		friend class MWDStream;
		friend class MWDFastObjectManager;
		
		//无传参则flag为0
		MWDObject() {
			m_uiFlag = 0;
			m_uiObjectID = GetObjectManager().AddObject(this);
		};
		//这样生成的obj只有flag跟传入的一样，但是ID是不一样的
		MWDObject(const MWDObject& object) {
			m_uiFlag = object.m_uiFlag;
			m_uiObjectID = GetObjectManager().AddObject(this);
		};
		virtual ~MWDObject() {
			GetObjectManager().DeleteObject(this);
		};
		//赋值符号也只能传进来flag，ID是不一样的
		MWDObject& operator =(const MWDObject& object) {
			m_uiFlag = object.m_uiFlag;
			return *this;
		};
		bool IsSameType(const MWDObject* pObject)const {
			return pObject && GetType().IsSameType(pObject->GetType());
		};
		bool IsDerived(const MWDObject* pObject)const {
			return pObject && GetType().IsDerived(pObject->GetType());
		};
		bool IsSameType(const MWDRtti& Type)const {
			return GetType().IsSameType(Type);
		};
		bool IsDerived(const MWDRtti& Type)const {
			return GetType().IsDerived(Type);
		};

		static MWDFastObjectManager& GetObjectManager()
		{
			static MWDFastObjectManager ms_ObjectManager;
			return  ms_ObjectManager;
		}

		//获得一个由obj管理器管理的obj
		template<typename T>
		static T* GetInstance()
		{
			return (T*)GetInstance(T::ms_Type);
		}
		static MWDObject* GetInstance(const MWDName& sRttiName) {
			int idx = ms_ClassFactory.Find(sRttiName);
			if (ms_ClassFactory.GetNum() == idx) {
				cout << "没找到执行函数！----MWDObject.h::GetInstance()" << endl;
				return NULL;
			}
			MWDObject* p = (MWDObject*)ms_ClassFactory[idx].Value();
			p->m_uiObjectID = GetObjectManager().AddObject(p);
			return p;
		};
		static MWDObject* GetInstance(const MWDRtti& Rtti) {
			int idx = ms_ClassFactory.Find(Rtti.GetName());
			if (ms_ClassFactory.GetNum() == idx) {
				cout << "没找到执行函数！----MWDObject.h::GetInstance()" << endl;
				return NULL;
			}
			MWDObject* p = (MWDObject*)ms_ClassFactory[idx].Value();
			GetObjectManager().AddObject(p);
			return p;
		};
		
		//获取一个不被obj管理器管理的独立obj
		template<typename T>
		static T* GetNoGCInstance() {
			return (T*)GetNoGCInstance(T::ms_Type.GetName());
		};
		static MWDObject* GetNoGCInstance(const MWDName& sRttiName) {
			unsigned int i = ms_ClassFactory.Find(sRttiName);
			if (i == ms_ClassFactory.GetNum())
				return NULL;

			MWDObject* pObject = ms_ClassFactory[i].Value();
			return pObject;
		};

		//给obj设置GC标志位
		static void PrepareForGC() {
			MWDFastObjectManager& tmp = GetObjectManager();
			for (unsigned int i = 0; i < MWDFastObjectManager::MAX_OBJECT_NUM; i++)
			{
				MWDObject* p = tmp.m_ObjectArray[i];
				if (p)
				{
					p->ClearFlag(MWDObject::OF_REACH);
					p->SetFlag(MWDObject::OF_UNREACH);
				}
			}
		};

		FORCEINLINE unsigned int GetFlag() const {
			return m_uiFlag;
		}
		FORCEINLINE unsigned int GetObjID() const {
			return m_uiObjectID;
		}
		FORCEINLINE void SetFlag(unsigned int uiFlag)
		{

			m_uiFlag |= uiFlag;
		}
		FORCEINLINE void ClearFlag(unsigned int uiFlag)
		{
			m_uiFlag &= ~uiFlag;
		}
		FORCEINLINE bool IsHasFlag(unsigned int uiFlag)
		{
			return (m_uiFlag & uiFlag) != 0;
		}

		#pragma region CallFunction
		bool Process(MWDName& FunName, void* para, void* ret = NULL, int ParaNum = -1);
		void CallVoidFunNoPara(MWDName& FunName)
		{
			Process(FunName, NULL);
		}

		template<class Type1>
		void CallVoidFunOnePara(MWDName& FunName, Type1& t1)
		{
			struct MyStruct
			{
				Type1 t1;
			}Temp;
			Temp.t1 = t1;
			Process(FunName, (void*)&Temp, NULL, 1);
		}

		template<class Type1, class Type2>
		void CallVoidFunTwoPara(MWDName& FunName, Type1& t1, Type2& t2)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Process(FunName, (void*)&Temp, NULL, 2);
		}

		template<class Type1, class Type2, class Type3>
		void CallVoidFunThreePara(MWDName& FunName, Type1& t1, Type2& t2, Type3& t3)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
				Type3 t3;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Temp.t3 = t3;
			Process(FunName, (void*)&Temp, NULL, 3);
		}

		template<class Type1, class Type2, class Type3, class Type4>
		void CallVoidFunFourPara(MWDName& FunName, Type1& t1, Type2& t2, Type3& t3, Type4& t4)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
				Type3 t3;
				Type4 t4;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Temp.t3 = t3;
			Temp.t4 = t4;
			Process(FunName, (void*)&Temp, NULL, 4);
		}

		template<class ReturnType>
		void CallFunNoPara(MWDName& FunName, ReturnType& ReturnValue)
		{
			Process(FunName, NULL, (void*)&ReturnValue);
		}

		template<class ReturnType, class Type1>
		void CallFunOnePara(MWDName& FunName, ReturnType& ReturnValue, Type1& t1)
		{
			struct MyStruct
			{
				Type1 t1;
			}Temp;
			Temp.t1 = t1;
			Process(FunName, (void*)&Temp, (void*)&ReturnValue, 1);
		}

		template<class ReturnType, class Type1, class Type2>
		void CallFunTwoPara(MWDName& FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Process(FunName, (void*)&Temp, (void*)&ReturnValue, 2);
		}

		template<class ReturnType, class Type1, class Type2, class Type3>
		void CallFunThreePara(MWDName& FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2, Type3& t3)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
				Type3 t3;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Temp.t3 = t3;
			Process(FunName, (void*)&Temp, (void*)&ReturnValue, 3);
		}

		template<class ReturnType, class Type1, class Type2, class Type3, class Type4>
		void CallFunFourPara(MWDName& FunName, ReturnType& ReturnValue, Type1& t1, Type2& t2, Type3& t3, Type4& t4)
		{
			struct MyStruct
			{
				Type1 t1;
				Type2 t2;
				Type3 t3;
				Type4 t4;
			}Temp;
			Temp.t1 = t1;
			Temp.t2 = t2;
			Temp.t3 = t3;
			Temp.t4 = t4;
			Process(FunName, (void*)&Temp, (void*)&ReturnValue, 4);
		}
		#pragma endregion
		
	protected:
		unsigned int m_uiObjectID;
		//所有Obj都可以访问到，这里保存了所有Obj的工厂函数
		static inline MWDMap<MWDName, CreateObjectFun> ms_ClassFactory;
		enum
		{
			OF_REACH = 0x01,
			OF_UNREACH = 0x02,
			OF_PendingKill = 0x04,
			OF_GCObject = 0x08,
			OF_RootObject = 0x10,
			OF_MAX
		};
		unsigned int m_uiFlag;
	};

	DECLARE_Ptr(MWDObject);
	MWDTYPE_MARCO(MWDObject);

	#pragma region 强制类型转换
//静态类型转换（所有MWDObject的类型之间都可以转）
	template <class T>
	T* StaticCast(MWDObject* pkObj)
	{
		return (T*)pkObj;
	}

	//静态类型转换（所有MWDObject的类型之间都可以转）
	template <class T>
	const T* StaticCast(const MWDObject* pkObj)
	{
		return (const T*)pkObj;
	}

	//只能父类转子类
	template<class T>
	T* DynamicCast(MWDObject* pObj)
	{
		return pObj && pObj->IsDerived(T::ms_Type) ? (T*)pObj : 0;
	}

	//只能父类转子类
	template<class T>
	const T* DynamicCast(const MWDObject* pObj)
	{
		return pObj && pObj->IsDerived(T::ms_Type) ? (const T*)pObj : 0;
	}
#pragma endregion

}


