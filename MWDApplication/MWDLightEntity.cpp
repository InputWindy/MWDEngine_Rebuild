#include "pch.h"
#include "MWDLightEntity.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDLightEntity)
	ADD_PRIORITY(MWDEntity)
	INITIAL_WITH_INIT_TERMINAL_END(MWDLightEntity)

	BEGIN_ADD_PROPERTY(MWDLightEntity)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDDirectionalLightEntity)
		ADD_PRIORITY(MWDLightEntity)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDDirectionalLightEntity)

	BEGIN_ADD_PROPERTY(MWDDirectionalLightEntity)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDPointLightEntity)
		ADD_PRIORITY(MWDLightEntity)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDPointLightEntity)

	BEGIN_ADD_PROPERTY(MWDPointLightEntity)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDSpotLightEntity)
		ADD_PRIORITY(MWDLightEntity)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDSpotLightEntity)

	BEGIN_ADD_PROPERTY(MWDSpotLightEntity)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDAreaLightEntity)
		ADD_PRIORITY(MWDLightEntity)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDAreaLightEntity)

	BEGIN_ADD_PROPERTY(MWDAreaLightEntity)
	END_ADD_PROPERTY


}