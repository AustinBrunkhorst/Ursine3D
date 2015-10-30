#include "Precompiled.h"

#include "CharacterControllerComponent.h"

NATIVE_COMPONENT_DEFINITION( CharacterController );

CharacterController::CharacterController(void)
    : BaseComponent()
    , speed( 1.0f )
    , id( 0 ) { }
