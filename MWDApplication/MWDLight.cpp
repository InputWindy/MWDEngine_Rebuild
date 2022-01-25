#include "pch.h"
#include "MWDLight.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDDirectionalLightComponent)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDDirectionalLightComponent)

	BEGIN_ADD_PROPERTY(MWDDirectionalLightComponent)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDPointLightComponent)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDPointLightComponent)

	BEGIN_ADD_PROPERTY(MWDPointLightComponent)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDAreaLightComponent)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDAreaLightComponent)

	BEGIN_ADD_PROPERTY(MWDAreaLightComponent)
	END_ADD_PROPERTY



	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDSpotLightComponent)
	ADD_INITIAL_FUNCTION(InitialClassFactory)
	INITIAL_WITH_INIT_TERMINAL_END(MWDSpotLightComponent)

	BEGIN_ADD_PROPERTY(MWDSpotLightComponent)
	END_ADD_PROPERTY

}