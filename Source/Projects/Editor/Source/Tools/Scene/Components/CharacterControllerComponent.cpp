#include "Precompiled.h"

#include "CharacterControllerComponent.h"

NATIVE_COMPONENT_DEFINITION( CharacterController );

CharacterController::CharacterController(void)
    : BaseComponent()
    , moveSpeed( 50.0f )
	, rotateSpeed( 300.0f )
    , id( 0 ) { }
