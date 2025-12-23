#include "CRenderState.h"

CRenderState::~CRenderState()
{
	SAFE_RELEASE(State);
	SAFE_RELEASE(PrevState);
}
