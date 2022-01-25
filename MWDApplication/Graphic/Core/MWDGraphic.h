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

	#pragma region ��������ָ��
//����һ��ָ��ClassName������ָ��   ##ClassName##Ptr	(������ʹ��)
#define DECLARE_Ptr(ClassName)\
	class ClassName;\
	typedef shared_ptr<ClassName> ##ClassName##Ptr;
#pragma endregion

	#pragma region RTTI
//����һ��static MWDRtti�İ�,˳������һ��MWDPriority�İ���������ʹ�ã�
//static CreateObjectFun FactoryFunc��Ҫ�Լ���cpp��ʵ�ֲ��ֶ�ע��
#define DECLARE_RTTI(classname,baseclassname) \
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),baseclassname::ms_Type,classname::FactoryFunc);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

//����MWDRtti��MWDPriority��������ʹ�ã�
#define DECLARE_RTTI_NoCreateFun(classname,baseclassname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),baseclassname::ms_Type,NULL);\
public:\
	static inline MWDPriority* ms_Priority = new MWDPriority();	

//����MWDRtti��MWDPriority��������ʹ�ã�
//static CreateObjectFun FactoryFunc��Ҫ�Լ���cpp��ʵ�ֲ��ֶ�ע��
#define DECLARE_RTTI_NoParent(classname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),NULL,classname::FactoryFunc);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

//����MWDRtti��MWDPriority��������ʹ�ã�
#define DECLARE_RTTI_NoParent_NoCreateFun(classname)\
public:\
	virtual MWDRtti & GetType()const{return *ms_Type;}\
	static inline MWDRtti* ms_Type =new MWDRtti(_T(#classname),NULL,NULL);\
public:\
	static inline  MWDPriority* ms_Priority = new MWDPriority();	

#pragma endregion

	#pragma region Priority
//�ڵ�ǰ��ά��һ����̬��MWDPriority��Ϣ��������ʹ�ã�
#define DECLARE_PRIORITY \
public:\
	static inline  MWDPriority ms_Priority = *new MWDPriority();

//�ѱ������ӽ��Լ��������б��У�initalBegin��End֮��д��
#define ADD_PRIORITY(classname)\
	if(!ms_Priority->AddPriorityThan(classname::ms_Priority))\
		return 0;
#pragma endregion

	#pragma region DeclareInitial

	#pragma region DECLARE_CLASS_FUNCTION
//================================================
// ���ܣ�����ǰ����ӹ�������
// ����ֻ��������CLASS_FUNCTION����Ҫ��BEGIN��END֮���ֶ��������һ�䣺
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

//������ʹ�ã���������Ҫ���Է��������������ࣨ����ע���ʼ�������Ĺ��ܣ�
#define DECLARE_INITIAL_WITH_INIT_TERMINAL(classname)\
public: \
	static bool RegisterMainFactory();\
	static inline bool ms_bRegisterMainFactory; \
	static inline bool InitialProperty(MWDRtti*); \
	static inline bool TerminalProperty();\
//===========================================================
// ʵ��ԭ��
//		ʵ�ֹ�������������ʵ�����Լ���
//		ʵ��ע�Ṥ�����������ڹ�����������ע�ᣩ��
//		ʵ��ע�ắ��������
//PS:Ҫʹ�ó�ʼ�����࣬��Ҫ�Լ�ά��һ�����������б�ms_ClassFactory
//		��������ʹ�ã�
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
	//�����ڲ�ʹ�����Է��䡢����������ֻࣨ�г�ʼ��ע�Ṧ�ܣ�
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

	#pragma region ����ע��
	//�������begin��end֮��д
	#define	ADD_INITIAL_FUNCTION(function_name)\
		MWDMain::AddInitialFuntion(function_name); 
	//�������begin��end֮��д(ֻ�����static inline�ຯ������ͨ����)
	#define	ADD_INITIAL_FUNCTION_WITH_PRIORITY(function_name)\
		MWDMain::AddInitialFuntion(function_name,ms_Priority); 
	//�������begin��end֮��д
	#define	ADD_TERMINAL_FUNCTION(function_name)\
		MWDMain::AddTerminalFuntion(function_name);
	//�������begin��end֮��д(ֻ�����static inline�ຯ������ͨ����)
	#define	ADD_TERMINAL_FUNCTION_WITH_PRIORITY(function_name)\
		MWDMain::AddTerminalFuntion(function_name,ms_Priority);	

	#pragma endregion

	#pragma region �����Է���
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

//ע������
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

//ע�ᵥֵ���ԣ��������ͣ��������ƣ�UI�����ƣ���־��
#define REGISTER_PROPERTY(AttributeName,UIName,flag) \
	activeProperty = (MWDProperty*)MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->AttributeName).CreateProperty(\
	_T(#UIName), *pRtti, (size_t)((char*)&(dummyPtr->AttributeName) - (char*)dummyPtr), flag);\
	pRtti->AddProperty(activeProperty);

//ע�᷶Χ���ԣ��������ƣ�UI�����ƣ����ֵ����Сֵ����������־��
#define REGISTER_PROPERTY_RANGE(AttributeName,UIName,High,Low,Step,flag) \
	activeProperty = (MWDProperty*)MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->AttributeName).CreateProperty(\
	_T(#UIName), *pRtti, (size_t)((char*)&(dummyPtr->AttributeName) - (char*)dummyPtr),High,Low,Step,flag);\
	pRtti->AddProperty(activeProperty);
#pragma endregion

	#pragma region �ຯ������

	#pragma region �ຯ�����乤�ߺ꣨���ã�
	#define REGISTER_FUNCTION_PROPERTY(reflectName) \
		activeProperty = MWDPropertyCreator::GetAutoPropertyCreator(dummyPtr->reflectName).CreateFunctionProperty(\
		_T(#reflectName), *pFun, (size_t)((char*)&(dummyPtr->reflectName) - (char*)dummyPtr), 0); \
		pFun->AddProperty(activeProperty);

	#define  Get_FUN_PROPERTY_VALUE(Num) \
		pFun->GetProperty(Num)->GetValueAddress(para)

	//ִ��һ���������ɱ�����꣬ʡ�ԺŲ���ȫ���������  ##__VA_ARGS__��
	#define  RETURN_FUN(classname,FunctionName,...) \
		((classname *)p)->FunctionName(##__VA_ARGS__);
	#pragma endregion

	#pragma region ����0����0�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����0����1�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����1����0�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����1����1�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����2����0�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����2����1�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����3����0�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����3����1�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����4����0�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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

	#pragma region ����4����1�������
	//�����д��cpp�ֻ�ܷ���public�ĺ�����
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
	//����16������
	#define TEXLEVEL 16 

	class  MWDEngineFlag
	{
	public:
		//�Ƿ�򿪱༭��
		static inline bool IsRunEditor = false;
		//CLOD��terrainLOD
		static inline bool EnableCLODMesh = true;
	};
}

