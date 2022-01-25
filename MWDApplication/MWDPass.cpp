#include "MWDPass.h"
#include "MWDRenderer.h"
namespace MWDEngine {
	INITIAL_WITH_INIT_TERMINAL_BEGIN(MWDPass)
	ADD_PRIORITY(MWDObject)
    INITIAL_WITH_INIT_TERMINAL_END(MWDPass)

    BEGIN_ADD_PROPERTY(MWDPass)
    END_ADD_PROPERTY

    bool MWDPass::Draw(MWDRenderer* pRenderer) {
		if (!pRenderer->SetRenderCtx(this)) {
			return false;
		};
		return true;
    }
}