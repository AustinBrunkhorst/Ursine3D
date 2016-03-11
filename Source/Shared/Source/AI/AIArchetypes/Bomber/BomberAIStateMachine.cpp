#include "Precompiled.h"

#include "BomberAIStateMachine.h"

namespace ursine
{
    namespace sm
    {
        BomberAIStateMachine::BomberAIStateMachine()
        {
            // Condition/SM variables to go here
            AddBool("HitPlayer", false);

        }
    }
}