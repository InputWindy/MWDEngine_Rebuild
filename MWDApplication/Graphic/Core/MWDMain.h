#pragma once
#include "MWDPriority.h"
#include <MWDArray.h>
#include "MWDRtti.h"
#include "MWDObject.h"
#include <iostream>
using namespace std;
namespace MWDEngine {

	//��Ҫ��ʼ��ʱִ�еĺ��������������ܴ�һ�����ǰ����Լ��Ĺ�������ע�ᵽ�Լ���static�б��
	typedef bool (*Function)();

	//��Ҫ��ʼ��ʱִ�еĺ��������������Ǹ�����ʵ�����Լ��ĺ�����
	typedef bool (*FunctionProperty)(MWDRtti*);

	//�����࣬���ܹ��죬ֻ�ܷ��ʾ�̬�ӿ�
	class  MWDMain
	{
	public:

		//��ms_pInitialPropertyArray��add
		static void AddInitialPropertyFuntion(FunctionProperty Func);
		//��ms_pTerminalPropertyArray��add
		static void AddTerminalPropertyFuntion(Function Func);

		//��ms_pInitialArray��add
		//FORCEINLINE static void AddInitialFuntion(Function Func) { AddInitialFuntion(Func, NULL); };
		static void AddInitialFuntion(Function Func, MWDPriority* pPriority = NULL);
		
		//��ms_pTerminalArray��add
		//FORCEINLINE static void AddTerminalFuntion(Function Func) { AddTerminalFuntion(Func, NULL); };
		static void AddTerminalFuntion(Function Func, MWDPriority* pPriority = NULL);

		static bool Initialize();
		static bool Terminate();

	private:
		MWDMain() {};
		~MWDMain() {};

		//Elementά��һ��ע���˵ĺ��������������ȼ���
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

			//û����ʵ��ԭ��
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
		
		//����ע��ʹ�õĺ���
		static inline MWDArray<FunctionProperty>* ms_pInitialPropertyArray;
		static inline MWDArray<Function>* ms_pTerminalPropertyArray;

		//��������������һЩ�ڳ�ʼ��ʱ��ִ�е����ں������ǹ���������
		static inline MWDArray<Element>* ms_pInitialArray;
		static inline MWDArray<Element>* ms_pTerminalArray;

		//obj���������ж���ʣ��obj���ã���MWDObject���ȡ��
		static inline unsigned int ms_uiInitialObject;

		//��Ҫ���ٵ�obj��������MWDObject���ȡ��
		static inline unsigned int ms_uiTerminalObject;

		//�κ�һ��������operator()�Ϳ��Ա��һ�����Ե�ʵ�εĴ�����
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

