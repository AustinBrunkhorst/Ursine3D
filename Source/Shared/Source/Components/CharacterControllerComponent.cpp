#include "Precompiled.h"

#include "CharacterControllerComponent.h"

NATIVE_COMPONENT_DEFINITION( CharacterController );

CharacterController::CharacterController(void)
    : BaseComponent()
	, CanJump(true)
	, CanStep(true)
	, inAir(false)
    , moveSpeed( 50.0f )
    , jumpSpeed( 1000.0f )
	, rotateSpeed( 300.0f )
{
    lookDir = ursine::Vec2(0, 0);
    moveDir = ursine::Vec2(0, 0);
}


