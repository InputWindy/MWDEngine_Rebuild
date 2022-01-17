#pragma once
#include "MWDPriority.h"
#include <MWDArray.h>
#include "MWDRtti.h"
#include "MWDObject.h"
#include <iostream>
using namespace std;
namespace MWDEngine {

	//需要初始化时执行的函数（无依赖，很大一部分是把类自己的工厂函数注册到自己的static列表里）
	typedef bool (*Function)();

	//需要初始化时执行的函数（有依赖，是各种类实例化自己的函数）
	typedef bool (*FunctionProperty)(MWDRtti*);

	//抽象类，不能构造，只能访问静态接口
	class  MWDMain
	{
	public:

		//往ms_pInitialPropertyArray里add
		static void AddInitialPropertyFuntion(FunctionProperty Func);
		//往ms_pTerminalPropertyArray里add
		static void AddTerminalPropertyFuntion(Function Func);

		//往ms_pInitialArray里add
		//FORCEINLINE static void AddInitialFuntion(Function Func) { AddInitialFuntion(Func, NULL); };
		static void AddInitialFuntion(Function Func, MWDPriority* pPriority = NULL);
		
		//往ms_pTerminalArray里add
		//FORCEINLINE static void AddTerminalFuntion(Function Func) { AddTerminalFuntion(Func, NULL); };
		static void AddTerminalFuntion(Function Func, MWDPriority* pPriority = NULL);

		static bool Initialize();
		static bool Terminate();

	private:
		MWDMain() {};
		~MWDMain() {};

		//Element维护一个注册了的函数（包含其优先级）
		struct Element
		{
		public:
			Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			~Element()
			{
				Func = NULL;
				pPriority = NULL;
			}
			Function		Func;
			MWDPriority* pPriority;

			//没明白实现原理
			bool operator > (const Element& e)const
			{
				static MWDPriority Pr1;
				static MWDPriority Pr2;

				MWDPriority* p1 = NULL;
				MWDPriority* p2 = NULL;
				if (pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if (e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) > (*p2);
			}
			bool operator < (const Element& e)const
			{
				static MWDPriority Pr1;
				static MWDPriority Pr2;

				MWDPriority* p1 = NULL;
				MWDPriority* p2 = NULL;
				if (pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if (e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) < (*p2);
			}
			bool operator == (const Element& e)const
			{
				static MWDPriority Pr1;
				static MWDPriority Pr2;

				MWDPriority* p1 = NULL;
				MWDPriority* p2 = NULL;
				if (pPriority)
					p1 = pPriority;
				else
					p1 = &Pr1;
				if (e.pPriority)
					p2 = e.pPriority;
				else
					p2 = &Pr2;
				return (*p1) == (*p2);
			}
		
		};
		
		//类内注册使用的函数
		static inline MWDArray<FunctionProperty>* ms_pInitialPropertyArray;
		static inline MWDArray<Function>* ms_pTerminalPropertyArray;

		//工厂函数、还有一些在初始化时候执行的类内函数（非工厂函数）
		static inline MWDArray<Element>* ms_pInitialArray;
		static inline MWDArray<Element>* ms_pTerminalArray;

		//obj管理器还有多少剩余obj可用（从MWDObject类获取）
		static inline unsigned int ms_uiInitialObject;

		//需要销毁的obj数量（从MWDObject类获取）
		static inline unsigned int ms_uiTerminalObject;

		//任何一个类重载operator()就可以变成一个可以当实参的处理函数
		class PriorityCompare
		{
		public:
			FORCEINLINE bool operator()(Element& e1, Element& e2)
			{
				static MWDPriority Pr1;
				static MWDPriority Pr2;

				MWDPriority* p1 = NULL;
				MWDPriority* p2 = NULL;
				if (e1.pPriority)
					p1 = e1.pPriority;
				else
					p1 = &Pr1;
				if (e2.pPriority)
					p2 = e2.pPriority;
				else
					p2 = &Pr2;
				return (*p1) <= (*p2);
			}
		};
	};
}

