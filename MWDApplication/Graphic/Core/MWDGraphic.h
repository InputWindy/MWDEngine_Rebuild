#pragma once
#include <tchar.h>
#include <MWDMemManager.h>
#include <MWDBitArray.h>
#define MWDGRAPHIC_EXPORTS
#ifdef MWDGRAPHIC_EXPORTS
#define   __declspec(dllexport) 
#else
#define   __declspec(dllimport)
#endif
#define PROFILER

	#pragma region Graphic_Macro

	#pragma region 声明智能指针
//声明一个指向ClassName的智能指针   ##ClassName##Ptr	(在类外使用)
#define DECLARE_Ptr(ClassName)\
	class ClassName;\
	typedef shared_ptr<ClassName> ##ClassName##Ptr;
#pragma endregion

	#pragma region RTTI
//声明一个static MWDRtti的包,顺便声明一个MWDPriority的包（在类内使用）
//static CreateObjectFun FactoryFunc需要自己在cpp里实现并手动注册
#define DECLARE_RTTI(classname,baseclassname) \
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),baseclassname::ms_Type,classname::FactoryFunc);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

//声明MWDRtti和MWDPriority（在类内使用）
#define DECLARE_RTTI_NoCreateFun(classname,baseclassname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),baseclassname::ms_Type,NULL);\
public:\
	static inline MWDPriority* ms_Priority = new MWDPriority();	

//声明MWDRtti和MWDPriority（在类内使用）
//static CreateObjectFun FactoryFunc需要自己在cpp里实现并手动注册
#define DECLARE_RTTI_NoParent(classname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),NULL,classname::FactoryFunc);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

//声明MWDRtti和MWDPriority（在类内使用）
#define DECLARE_RTTI_NoParent_NoCreateFun(classname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),NULL,NULL);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

#pragma endregion

	#pragma region Priority
//在当前类维护一个静态的MWDPriority信息（在类内使用）
#define DECLARE_PRIORITY \
public:\
	static inline  MWDPriority ms_Priority = *new MWDPriority();

//把别的类添加进自己的依赖列表中（initalBegin和End之间写）
#define ADD_PRIORITY(classname)\
	if(!ms_Priority->AddPriorityThan(classname::ms_Priority))\
		return 0;
#pragma endregion

	#pragma region DeclareInitial

	#pragma region DECLARE_CLASS_FUNCTION
//================================================
// 功能：给当前类添加工厂函数
// 这里只是声明了CLASS_FUNCTION，需要在BEGIN、END之间手动添加以下一句：
//		ADD_INITIAL_FUNCTION(InitialClassFactory)
//================================================
#define DECLARE_CLASS_FUNCTION(classname)\
public: \
	static inline MWDName* ms_Name = new MWDName(_T(#classname));\
	static bool InitialClassFactory() {\
		ms_ClassFactory.AddElement(*ms_Name, FactoryFunc); \
		return 1; \
}; \
static MWDObject* FactoryFunc() {\
		\
			return new classname(); \
	}; 

#pragma endregion 

	#pragma region DECLARE_INITIAL_WITH_INIT_TERMINAL

//在类内使用，适用于需要属性反射跟函数反射的类（还有注册初始化函数的功能）
#define DECLARE_INITIAL_WITH_INIT_TERMINAL(classname)\
public: \
	static bool RegisterMainFactory();\
	static inline bool ms_bRegisterMainFactory; \
	static inline bool InitialProperty(MWDRtti*); \
	static inline bool TerminalProperty();\
//===========================================================
// 实现原理：
//		实现工厂函数（用于实例化自己）
//		实现注册工厂函数（用于工厂函数类内注册），
//		实现注册函数（），
//PS:要使用初始化的类，需要自己维护一个工厂函数列表ms_ClassFactory
//		（在类外使用）
//===========================================================
#define INITIAL_WITH_INIT_TERMINAL_BEGIN(classname) \
	bool classname::RegisterMainFactory()\
	{ \
		if (!ms_bRegisterMainFactory)\
		{

#define INITIAL_WITH_INIT_TERMINAL_END(classname) \
			MWDMain::AddInitialPropertyFuntion(classname::InitialProperty); \
			MWDMain::AddTerminalPropertyFuntion(classname::TerminalProperty); \
			ms_bRegisterMainFactory = true; \
		} \
		return ms_bRegisterMainFactory; \
	}\
static bool gs_bStreamRegistered_##classname = classname::RegisterMainFactory(); 

#pragma endregion

	#pragma region DECLARE_INITIAL_ONLY
	//适用于不使用属性反射、函数反射的类（只有初始化注册功能）
	#define DECLARE_INITIAL_ONLY(classname)\
	public: \
		static bool RegisterMainFactory(); \
		static inline bool ms_bRegisterMainFactory; 

	#define INITIAL_ONLY_BEGIN(classname)	\
		bool classname::RegisterMainFactory() \
		{ \
			if (!ms_bRegisterMainFactory) \
			{ 

	#define INITIAL_ONLY_END(classname) \
				ms_bRegisterMainFactory = true; \
			} \
			return ms_bRegisterMainFactory; \
		}\
	static bool gs_bStreamRegistered_##classname= classname::RegisterMainFactory (); 

	#pragma endregion

	#pragma region 函数注册
	//这个宏在begin和end之间写
	#define	ADD_INITIAL_FUNCTION(function_name)\
		MWDMain::AddInitialFuntion(function_name); 
	//这个宏在begin和end之间写(只能添加static inline类函数、普通函数)
	#define	ADD_INITIAL_FUNCTION_WITH_PRIORITY(function_name)\
		MWDMain::AddInitialFuntion(function_name,ms_Priority); 
	//这个宏在begin和end之间写
	#define	ADD_TERMINAL_FUNCTION(function_name)\
		MWDMain::AddTerminalFuntion(function_name);
	//这个宏在begin和end之间写(只能添加static inline类函数、普通函数)
	#define	ADD_TERMINAL_FUNCTION_WITH_PRIORITY(function_name)\
		MWDMain::AddTerminalFuntion(function_name,ms_Priority);	

	#pragma endregion

	#pragma region 类属性反射
#define ADD_NO_PROPERTY(classname) \
	bool classname::InitialProperty(MWDRtti *) \
	{ \
		return true; \
	} \
	bool classname::TerminalProperty() \
	{ \
		ms_Type.ClearProperty(); \
		return true; \
	}

//注册属性
#define BEGIN_ADD_PROPERTY(classname) \
	bool classname::TerminalProperty()\
	{\
		ms_Type->ClearProperty();\
		return true;\
	}\
	bool classname::InitialProperty(MWDRtti * pRtti)\
	{\
		classname* dummyPtr = NULL;\
		MWDProperty* activeProperty = NULL;\
		MWDRtti* pRttiTemp = pRtti;\
	if (!pRtti)\
	{\
		pRtti = ms_Type;\
	}

#define END_ADD_PROPERTY \
		return true; \
	}

//注册单值属性（属性类型，属性名称，UI绑定名称，标志）
#define REGISTER_PROPERTY(AttributeName,UIName,flag) \
	activeProperty = (MWDProperty*)MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->AttributeName).CreateProperty(\
	_T(#UIName), *pRtti, (size_t)((char*)&(dummyPtr->AttributeName) - (char*)dummyPtr), flag);\
	pRtti->AddProperty(activeProperty);

//注册范围属性（属性名称，UI绑定名称，最大值，最小值，步长，标志）
#define REGISTER_PROPERTY_RANGE(AttributeName,UIName,High,Low,Step,flag) \
	activeProperty = (MWDProperty*)MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->AttributeName).CreateProperty(\
	_T(#UIName), *pRtti, (size_t)((char*)&(dummyPtr->AttributeName) - (char*)dummyPtr),High,Low,Step,flag);\
	pRtti->AddProperty(activeProperty);
#pragma endregion

	#pragma region 类函数反射

	#pragma region 类函数反射工具宏（无用）
	#define REGISTER_FUNCTION_PROPERTY(reflectName) \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->reflectName).CreateFunctionProperty(\
		_T(#reflectName), *pFun, (size_t)((char*)&(dummyPtr->reflectName) - (char*)dummyPtr), 0); \
		pFun->AddProperty(activeProperty);

	#define  Get_FUN_PROPERTY_VALUE(Num) \
		pFun->GetProperty(Num)->GetValueAddress(para)

	//执行一个函数（可变参数宏，省略号部分全部都会填进  ##__VA_ARGS__）
	#define  RETURN_FUN(classname,FunctionName,...) \
		((classname *)p)->FunctionName(##__VA_ARGS__);
	#pragma endregion

	#pragma region 反射0输入0输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_VOID_FUNCTION_NOPARAMETER(classname, FunctionName, uiFlag)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			RETURN_FUN(classname, FunctionName, )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(0);\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		rtti.AddFunction(pFun); \
		};\
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射0输入1输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_RETURN_FUNCTION_NOPARAMETER(classname, FunctionName, uiFlag, Retype)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			* ((Retype*)ret) = RETURN_FUN(classname, FunctionName, )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(0);\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty* activeProperty = NULL; \
		Retype Temp = Retype(); \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(Temp).CreateFunctionProperty(\
			_T("Return"), *pFun, 0, 0); \
		pFun->SetReturnType(activeProperty); \
		rtti.AddFunction(pFun); \
		} \
	}; \
	Template_##FunctionName _Template_##FunctionName(*classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射1输入0输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_VOID_FUNCTION_ONEPARAMETER(classname, FunctionName, uiFlag,ValType1,ValName1)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			RETURN_FUN(classname, FunctionName, In##ValName1 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		rtti.AddFunction(pFun); \
		} \
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射1输入1输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_RETURN_FUNCTION_ONEPARAMETER(classname, FunctionName, uiFlag,Retype,ValType1,ValName1)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			* ((Retype*)ret) = RETURN_FUN(classname, FunctionName,In##ValName1 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		Retype Temp = Retype(); \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(Temp).CreateFunctionProperty(\
			_T("Return"), *pFun, 0, 0); \
		pFun->SetReturnType(activeProperty); \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		rtti.AddFunction(pFun); \
		} \
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射2输入0输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_VOID_FUNCTION_TWOPARAMETER(classname, FunctionName, uiFlag,ValType1,ValName1,ValType2,ValName2)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType2 *)Get_FUN_PROPERTY_VALUE(1)); \
			RETURN_FUN(classname, FunctionName, In##ValName1, In##ValName2 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		rtti.AddFunction(pFun); \
		} \
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射2输入1输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_RETURN_FUNCTION_TWOPARAMETER(classname, FunctionName, uiFlag,Retype,ValType1,ValName1,ValType2,ValName2)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType2 *)Get_FUN_PROPERTY_VALUE(1)); \
			* ((Retype*)ret) = RETURN_FUN(classname, FunctionName,In##ValName1,In##ValName2 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		Retype Temp = Retype(); \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(Temp).CreateFunctionProperty(\
			_T("Return"), *pFun, 0, 0); \
		pFun->SetReturnType(activeProperty); \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		rtti.AddFunction(pFun); \
		} \
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射3输入0输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_VOID_FUNCTION_THREEPARAMETER(classname, FunctionName, uiFlag,ValType1,ValName1,ValType2,ValName2,ValType3,ValName3)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
			ValType3 In##ValName3; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(1)); \
			ValType3 In##ValName3 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(2)); \
			RETURN_FUN(classname, FunctionName, In##ValName1,In##ValName2,In##ValName3 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		REGISTER_FUNCTION_PROPERTY(In##ValName3) \
		rtti.AddFunction(pFun); \
		};\
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射3输入1输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_RETURN_FUNCTION_THREEPARAMETER(classname, FunctionName, uiFlag,Retype,ValType1,ValName1,ValType2,ValName2,ValType3,ValName3)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
			ValType3 In##ValName3; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(1)); \
			ValType3 In##ValName3 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(2)); \
			* ((Retype*)ret) = RETURN_FUN(classname, FunctionName,In##ValName1,In##ValName2,In##ValName3 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		Retype Temp = Retype(); \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(Temp).CreateFunctionProperty(\
			_T("Return"), *pFun, 0, 0); \
		pFun->SetReturnType(activeProperty); \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		REGISTER_FUNCTION_PROPERTY(In##ValName3) \
		rtti.AddFunction(pFun); \
		};\
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射4输入0输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_VOID_FUNCTION_FOURPARAMETER(classname, FunctionName, uiFlag,ValType1,ValName1,ValType2,ValName2,ValType3,ValName3,ValType4,ValName4)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
			ValType3 In##ValName3; \
			ValType4 In##ValName4; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(1)); \
			ValType3 In##ValName3 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(2)); \
			ValType4 In##ValName4 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(3)); \
			RETURN_FUN(classname, FunctionName, In##ValName1,In##ValName2,In##ValName3,In##ValName4 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		REGISTER_FUNCTION_PROPERTY(In##ValName3) \
		REGISTER_FUNCTION_PROPERTY(In##ValName4) \
		rtti.AddFunction(pFun); \
		};\
	}; \
	Template_##FunctionName _Template_##FunctionName(classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma region 反射4输入1输出函数
	//这个宏写在cpp里（只能反射public的函数）
	#define REGISTER_RETURN_FUNCTION_FOURPARAMETER(classname, FunctionName, uiFlag,Retype,ValType1,ValName1,ValType2,ValName2,ValType3,ValName3,ValType4,ValName4)\
	class Template_##FunctionName \
	{ \
	public: \
		~Template_##FunctionName()\
		{ \
		} \
		struct Name \
		{ \
			ValType1 In##ValName1; \
			ValType2 In##ValName2; \
			ValType3 In##ValName3; \
			ValType4 In##ValName4; \
		}; \
		static inline void FunctionTemplate_Temp(MWDObject* p, MWDFunction* pFun, void* para, void* ret) \
		{ \
			ValType1 In##ValName1 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(0)); \
			ValType2 In##ValName2 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(1)); \
			ValType3 In##ValName3 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(2)); \
			ValType4 In##ValName4 = *((ValType1 *)Get_FUN_PROPERTY_VALUE(3)); \
			* ((Retype*)ret) = RETURN_FUN(classname, FunctionName,In##ValName1,In##ValName2,In##ValName3,In##ValName4 )\
		} \
	Template_##FunctionName(MWDRtti& rtti, unsigned int Flag)\
	{ \
		MWDFunction* pFun = new MWDFunction(rtti, _T(#FunctionName), Flag);\
		pFun->SetParaSize(sizeof(Name));\
		pFun->ObjectFun = FunctionTemplate_Temp; \
		MWDProperty * activeProperty = NULL; \
		Name * dummyPtr = NULL; \
		Retype Temp = Retype(); \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(Temp).CreateFunctionProperty(\
			_T("Return"), *pFun, 0, 0); \
		pFun->SetReturnType(activeProperty); \
		REGISTER_FUNCTION_PROPERTY(In##ValName1) \
		REGISTER_FUNCTION_PROPERTY(In##ValName2) \
		REGISTER_FUNCTION_PROPERTY(In##ValName3) \
		REGISTER_FUNCTION_PROPERTY(In##ValName4) \
		rtti.AddFunction(pFun); \
		};\
	}; \
	Template_##FunctionName _Template_##FunctionName(*classname##::ms_Type, uiFlag);
	#pragma endregion

	#pragma endregion

	#pragma endregion

	#pragma endregion

namespace MWDEngine
{	
	//允许16层纹理
	#define TEXLEVEL 16 

	class  MWDEngineFlag
	{
	public:
		//是否打开编辑器
		static inline bool IsRunEditor = false;
		//CLOD是terrainLOD
		static inline bool EnableCLODMesh = true;
	};
}

