//=====================================================================
// MWDDelegate实现的是一个函数的代理
// MWDDelegateEvent维护的是一个函数代理数组，实现了注册函数
// 并且触发函数的功能
// 
// 一个函数代理就是一个Handler，负责一个函数的启动，并保存这个函数的信息
//=====================================================================

#if DELEGATE_PARAM_COUNT > 0
#define DELEGATE_SEPARATOR ,
#else
#define DELEGATE_SEPARATOR
#endif
#define DELEGATE_DO_JOIN(X,Y) X##Y
#define	DELEGATE_JOIN_MACRO(X,Y) DELEGATE_DO_JOIN(X,Y)

#ifdef DELEGATE_PREFERRED_SYNTAX
	#define DELEGATE_CLASS_NAME MWDDelegate
	#define EVENT_CLASS_NAME	MWDDelegateEvent
#else
	#define DELEGATE_CLASS_NAME DELEGATE_JOIN_MACRO(MWDDelegate,DELEGATE_PARAM_COUNT)
	#define EVENT_CLASS_NAME DELEGATE_JOIN_MACRO(MWDDelegateEvent,DELEGATE_PARAM_COUNT)
#endif

template<class RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE>
#ifdef DELEGATE_PREFERRED_SYNTAX
class MWDDATASTRUCT_API DELEGATE_CLASS_NAME<RETUREN_TYPE(DELEGATE_TYPE)>
#else
class MWDDATASTRUCT_API DELEGATE_CLASS_NAME
#endif
{
public:
	DELEGATE_CLASS_NAME()
	{
		m_p = NULL;
		m_F = NULL;
	}
	~DELEGATE_CLASS_NAME()
	{

	}
protected:
	typedef RETUREN_TYPE(*F)(void* p DELEGATE_SEPARATOR DELEGATE_TYPE);

	void* m_p;
	F m_F;
	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)>
	static RETUREN_TYPE MethodStub(void* p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		T* Ap = (T*)p;
		return (Ap->*FunName)(DELEGATE_VALUE);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)const>
	static RETUREN_TYPE ConstMethodStub(void* p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		T* Ap = (T*)p;
		return (Ap->*FunName)(DELEGATE_VALUE);
	}

	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	static RETUREN_TYPE FunStub(void* p DELEGATE_SEPARATOR DELEGATE_TYPE_VALUE)
	{
		return (FunName)(DELEGATE_VALUE);
	}

	void Create(void* p, F f)
	{
		m_F = f;
		m_p = p;
	}
public:
	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)const>
	void  FromConstMethod(T* p)
	{
		Create((void*)p, &ConstMethodStub<T, FunName>);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)>
	void  FromMethod(T* p)
	{
		Create((void*)p, &MethodStub<T, FunName>);
	}

	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	void FromFun()
	{
		Create(NULL, &FunStub<FunName>);
	}



	RETUREN_TYPE Excute(DELEGATE_TYPE_VALUE)
	{
		return (*m_F)(m_p DELEGATE_SEPARATOR DELEGATE_VALUE);
	}
	RETUREN_TYPE operator()(DELEGATE_TYPE_VALUE) const
	{
		return (*m_F)(m_p DELEGATE_SEPARATOR DELEGATE_VALUE);
	}

	operator bool() const
	{
		return m_F != NULL;
	}

	bool operator!() const
	{
		return !(operator bool());
	}

	bool operator==(const DELEGATE_CLASS_NAME& rhs) const
	{
		return (m_p == rhs.m_p && m_F == rhs.m_F);
	}
};


template <class RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE>
#ifdef DELEGATE_PREFERRED_SYNTAX
class MWDDATASTRUCT_API EVENT_CLASS_NAME<RETUREN_TYPE(DELEGATE_TYPE)>
#else
class MWDDATASTRUCT_API EVENT_CLASS_NAME
#endif
{
public:
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef DELEGATE_CLASS_NAME<RETUREN_TYPE(DELEGATE_TEMPLATE_TYPE)> Handler;
#else
	typedef DELEGATE_CLASS_NAME<RETUREN_TYPE DELEGATE_SEPARATOR DELEGATE_TEMPLATE_TYPE> Handler;
#endif
public:
	EVENT_CLASS_NAME() {}
	//+=用于添加Handler
	void operator+=(const Handler& handler)
	{
		this->Add(handler);
	}
	void Add(const Handler& handler)
	{
		//函数不能重复注册，否则报错弹窗
		assert(!this->Has(handler));

		m_handlers.AddElement(handler);
	}

	//AddFun用于直接注册函数
	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	void AddFun()
	{
		cout << "MWDDelegateTemplate.h:执行到AddFun里了" << endl;
		Handler* handler = new Handler();
		handler->FromFun<FunName>();
		Add(*handler);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)const>
	void AddMethod(T* p)
	{
		cout << "MWDDelegateTemplate.h:执行到AddMethod里了" << endl;
		Handler* handler = new Handler();
		handler->FromMethod<T, FunName>(p);
		Add(*handler);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)>
	void AddConstMethod(T* p)
	{
		cout << "MWDDelegateTemplate.h:执行到AddConstMethod里了" << endl;
		Handler* handler = new Handler();
		handler->FromConstMethod<T, FunName>(p);
		Add(*handler);
	}


	void operator-=(const Handler& handler)
	{
		this->Remove(handler);
	}
	void Remove(const Handler& handler)
	{
		unsigned int i = m_handlers.FindElement(handler);

		if (i < m_handlers.GetNum())
		{
			m_handlers.Erase(i);
		}
	}
	template<RETUREN_TYPE(*FunName)(DELEGATE_TYPE)>
	void RemoveFun()
	{
		cout << "MWDDelegateTemplate.h:执行到RemoveFun里了" << endl;
		Handler* handler = new Handler();
		handler->FromFun<FunName>();
		Remove(*handler);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)const>
	void RemoveMethod(T* p)
	{
		cout << "MWDDelegateTemplate.h:执行到RemoveMethod里了" << endl;
		Handler* handler = new Handler();
		handler->FromMethod<T, FunName>(p);
		Remove(*handler);
	}

	template<class T, RETUREN_TYPE(T::* FunName)(DELEGATE_TYPE)>
	void RemoveConstMethod(T* p)
	{
		cout << "MWDDelegateTemplate.h:执行到RemoveConstMethod里了" << endl;
		Handler* handler = new Handler();
		handler->FromConstMethod<T, FunName>(p);
		Remove(*handler);
	}
	bool Has(const Handler& handler)
	{
		unsigned int i = m_handlers.FindElement(handler);
		return i != -1;
	}
	bool IsValid() const
	{
		return m_handlers.GetNum() > 0;
	}

	void Reset()
	{
		m_handlers.Clear();
	}


	void operator()(DELEGATE_TYPE_VALUE) const
	{
		this->Invoke(DELEGATE_VALUE);
	}
	//Invoke会让所有注册过的函数用同一套输入参数执行
	void Invoke(DELEGATE_TYPE_VALUE) const
	{
		for (unsigned int i = 0; i < m_handlers.GetNum(); ++i)
		{
			m_handlers[i](DELEGATE_VALUE);
		}
	}

	//去掉一个不执行的Handler
	void InvokeWithEmitter(DELEGATE_TYPE_VALUE
#if DELEGATE_PARAM_COUNT > 0
		,
#endif
		const Handler& emitter) const
	{
		for (unsigned i = 0; i < m_handlers.GetNum(); ++i)
		{
			if (m_handlers[i] != emitter)
			{
				m_handlers[i](DELEGATE_VALUE);
			}
		}
	}

private:
	typedef MWDArray<Handler> DelegateList;
	DelegateList m_handlers;
};
#undef DELEGATE_SEPARATOR
#undef DELEGATE_DO_JOIN
#undef DELEGATE_CLASS_NAME
#undef EVENT_CLASS_NAME