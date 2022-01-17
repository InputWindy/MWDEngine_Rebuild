#pragma once
//==========================================================================
// ��������ʵ��ԭ��
//		�����⺯����ͳһд��һ�ָ�ʽ�ĺ�������һ����������ά������ͳһ��ʽ
//		
//		ʹ�ú�������Ŀ���ǰ�ͬһ�����͵ĺ�����ͳһ�ú���������ִ�У�
//		�����������ʵ�֡�ע�ắ�������ȰѺ���ע�������
//		֮�����ֶ������Ƿ�ִ��
//==========================================================================
#include "../MWDSystem/MWDSystem.h"
#include "MWDContainer.h"
#include "MWDArray.h"
#include <stdio.h>
namespace MWDEngine
{
//#define DELEGATE_PREFERRED_SYNTAX
#ifdef DELEGATE_PREFERRED_SYNTAX
	template <typename TSignature> class MWDDelegate;
	template <typename TSignature> class MWDDelegateEvent;
#endif

#define DELEGATE_PARAM_COUNT	0
#define DELEGATE_TEMPLATE_TYPE
#define DELEGATE_TYPE
#define DELEGATE_TYPE_VALUE		
#define DELEGATE_VALUE
#include "MWDDelegateTemplate.h"
#undef DELEGATE_PARAM_COUNT
#undef DELEGATE_TEMPLATE_TYPE
#undef DELEGATE_TYPE
#undef DELEGATE_TYPE_VALUE		
#undef DELEGATE_VALUE

#define DELEGATE_PARAM_COUNT	1	
#define DELEGATE_TEMPLATE_TYPE	typename A1
#define DELEGATE_TYPE			A1
#define DELEGATE_TYPE_VALUE		A1 a1
#define DELEGATE_VALUE			a1
#include "MWDDelegateTemplate.h"
#undef DELEGATE_PARAM_COUNT
#undef DELEGATE_TEMPLATE_TYPE
#undef DELEGATE_TYPE
#undef DELEGATE_TYPE_VALUE		
#undef DELEGATE_VALUE

#define DELEGATE_PARAM_COUNT	2	
#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2
#define DELEGATE_TYPE			A1,A2
#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2
#define DELEGATE_VALUE			a1,a2
#include "MWDDelegateTemplate.h"
#undef DELEGATE_PARAM_COUNT
#undef DELEGATE_TEMPLATE_TYPE
#undef DELEGATE_TYPE
#undef DELEGATE_TYPE_VALUE		
#undef DELEGATE_VALUE


#define DELEGATE_PARAM_COUNT	3	
#define DELEGATE_TEMPLATE_TYPE	typename A1,typename A2,typename A3
#define DELEGATE_TYPE			A1,A2,A3
#define DELEGATE_TYPE_VALUE		A1 a1,A2 a2,A3 a3
#define DELEGATE_VALUE			a1,a2,a3
#include "MWDDelegateTemplate.h"
#undef DELEGATE_PARAM_COUNT
#undef DELEGATE_TEMPLATE_TYPE
#undef DELEGATE_TYPE
#undef DELEGATE_TYPE_VALUE		
#undef DELEGATE_VALUE
#ifdef DELEGATE_PREFERRED_SYNTAX
	typedef MWDDelegateEvent<int(int, int)> MouseMoveEvent;
#else
	//���е�MoveEvent����Ĳ�������һģһ���ģ����Կ�����invokeͳһִ��
	typedef MWDDelegateEvent2<int, int, int> MouseMoveEvent;
#endif
}
