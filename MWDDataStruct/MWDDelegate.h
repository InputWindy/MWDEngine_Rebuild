#pragma once
#include "../MWDSystem/MWDSystem.h"
#include "MWDContainer.h"
#include <iostream>

//==========================================================================
// ��������ʵ��ԭ��
//		�����⺯����ͳһд��һ�ָ�ʽ�ĺ�������һ����������ά������ͳһ��ʽ
//		
//		ʹ�ú�������Ŀ���ǰ�ͬһ�����͵ĺ�����ͳһ�ú���������ִ�У�
//		�����������ʵ�֡�ע�ắ�������ȰѺ���ע�������
//		֮�����ֶ������Ƿ�ִ��
//==========================================================================

using namespace std;
namespace MWDEngine {
//������ֵ��1���ֵ�ĺ�����m_p�Ǵ�����࣬m_f��ͳһ�ĺ�������
	#pragma region �޲δ���
	#define DECLARE_DELEGATE_ZERO(TypeName) \
		template<typename ReturnType> \
		class TypeName \
		{ \
		public: \
			TypeName() \
			{\
				m_p = NULL;\
				m_F = NULL;\
			}\
			~TypeName()\
			{ \
			} \
		protected:\
			typedef ReturnType(* F)(void * p);\
			void * m_p;\
			F m_F;\
			template<class T,ReturnType (T::*FunName)()>\
			static ReturnType MethodStub(void * p)\
			{\
				T * Ap = (T *)p;\
				return (Ap->*FunName)();\
			}\
			template<ReturnType(*FunName)()>\
			static ReturnType FunStub(void * p)\
			{\
				return (FunName)();\
			}\
			void Create(void * p , F f)\
			{\
				m_F = f;\
				m_p = p;\
			}\
			public:\
			template<class T,ReturnType (T::*FunName)()>\
			void FromMethod( T * p)\
			{\
				Create((void*)p, &MethodStub<T,FunName>);\
			}\
			template<ReturnType(*FunName)()>\
			void FromFun()\
			{\
				Create(NULL, &FunStub<FunName>);\
			}\
			ReturnType Excute()\
			{\
				return (*m_F)(m_p);\
			}\
	}; 
	#pragma endregion

	#pragma region 1��������
	#define DECLARE_DELEGATE_ONE(TypeName) \
		template<typename ReturnType,typename A1> \
		class TypeName \
		{ \
		public: \
			TypeName() \
			{\
				m_p = NULL;\
				m_F = NULL;\
			}\
			~TypeName()\
			{ \
			} \
		protected:\
			typedef ReturnType(* F)(void * p,A1 a1);\
			void * m_p;\
			F m_F;\
			template<class T,ReturnType (T::*FunName)(A1)>\
			static ReturnType MethodStub(void * p,A1 a1)\
			{\
				T * Ap = (T *)p;\
				return (Ap->*FunName)(a1);\
			}\
			template<ReturnType(*FunName)(A1)>\
			static ReturnType FunStub(void * p,A1 a1)\
			{\
				return (FunName)(a1);\
			}\
			void Create(void * p , F f)\
			{\
				m_F = f;\
				m_p = p;\
			}\
			public:\
			template<class T,ReturnType (T::*FunName)(A1)>\
			void FromMethod( T * p)\
			{\
				Create((void*)p, &MethodStub<T,FunName>);\
			}\
			template<ReturnType(*FunName)(A1)>\
			void FromFun()\
			{\
				Create(NULL, &FunStub<FunName>);\
			}\
			ReturnType Excute(A1 a1)\
			{\
				cout<<m_F<<endl<<m_p<<endl;\
				return (*m_F)(m_p,a1);\
			}\
	}; 
	#pragma endregion

	#pragma region 2��������
	#define DECLARE_DELEGATE_TWO(TypeName) \
		template<typename ReturnType,typename A1,typename A2> \
		class TypeName \
		{ \
		public: \
			TypeName() \
			{\
				m_p = NULL;\
				m_F = NULL;\
			}\
			~TypeName()\
			{ \
			} \
		protected:\
			typedef ReturnType(* F)(void * p,A1 a1,A2 a2);\
			void * m_p;\
			F m_F;\
			template<class T,ReturnType (T::*FunName)(A1,A2)>\
			static ReturnType MethodStub(void * p,A1 a1,A2 a2)\
			{\
				T * Ap = (T *)p;\
				return (Ap->*FunName)(a1,a2);\
			}\
			template<ReturnType(*FunName)(A1,A2)>\
			static ReturnType FunStub(void * p,A1 a1,A2 a2)\
			{\
				return (FunName)(a1,a2);\
			}\
			void Create(void * p , F f)\
			{\
				m_F = f;\
				m_p = p;\
			}\
			public:\
			template<class T,ReturnType (T::*FunName)(A1,A2)>\
			void FromMethod( T * p)\
			{\
				Create((void*)p, &MethodStub<T,FunName>);\
			}\
			template<ReturnType(*FunName)(A1,A2)>\
			void FromFun()\
			{\
				Create(NULL, &FunStub<FunName>);\
			}\
			ReturnType Excute(A1 a1,A2 a2)\
			{\
				cout<<m_F<<endl<<m_p<<endl;\
				return (*m_F)(m_p,a1,a2);\
			}\
	}; 
	#pragma endregion

	#pragma region 3��������
	#define DECLARE_DELEGATE_THREEE(TypeName) \
		template<typename ReturnType,typename A1,typename A2,typename A3> \
		class TypeName \
		{ \
		public: \
			TypeName() \
			{\
				m_p = NULL;\
				m_F = NULL;\
			}\
			~TypeName()\
			{ \
			} \
		protected:\
			typedef ReturnType(* F)(void * p,A1 a1,A2 a2,A3 a3);\
			void * m_p;\
			F m_F;\
			template<class T,ReturnType (T::*FunName)(A1,A2,A3)>\
			static ReturnType MethodStub(void * p,A1 a1,A2 a2,A3 a3)\
			{\
				T * Ap = (T *)p;\
				return (Ap->*FunName)(a1,a2,a3);\
			}\
			template<ReturnType(*FunName)(A1,A2,A3)>\
			static ReturnType FunStub(void * p,A1 a1,A2 a2,A3 a3)\
			{\
				return (FunName)(a1,a2,a3);\
			}\
			void Create(void * p , F f)\
			{\
				m_F = f;\
				m_p = p;\
			}\
			public:\
			template<class T,ReturnType (T::*FunName)(A1,A2,A3)>\
			void FromMethod( T * p)\
			{\
				Create((void*)p, &MethodStub<T,FunName>);\
			}\
			template<ReturnType(*FunName)(A1,A2,A3)>\
			void FromFun()\
			{\
				Create(NULL, &FunStub<FunName>);\
			}\
			ReturnType Excute(A1 a1,A2 a2,A3 a3)\
			{\
				cout<<m_F<<endl<<m_p<<endl;\
				return (*m_F)(m_p,a1,a2,a3);\
			}\
	}; 
	#pragma endregion

}


