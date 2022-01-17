#include "pch.h"
#include "MWDBind.h"
//#include "MWDDefaultResource.h"
namespace MWDEngine {

	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDBind)
	ADD_PRIORITY(MWDObject)
	INITIAL_WITH_INIT_TERMINAL_END(MWDBind)

	BEGIN_ADD_PROPERTY(MWDBind)
	REGISTER_PROPERTY(m_bIsStatic, IsStatic, MWDProperty::F_SAVE_LOAD_CLONE);
	REGISTER_PROPERTY(m_uiSwapChainNum, SwapChainNum, MWDProperty::F_SAVE_LOAD_CLONE);
	REGISTER_PROPERTY(m_uiLockFlag, LockFlag, MWDProperty::F_SAVE_LOAD_CLONE);
	REGISTER_PROPERTY(m_uiMemType, MemType, MWDProperty::F_SAVE_LOAD_CLONE);
	END_ADD_PROPERTY

	
}