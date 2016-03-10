#include "Precompiled.h"

#include "AgileAIStateMachine.h"

namespace ursine
{
    namespace sm
    {
        AgileAIStateMachine::AgileAIStateMachine()
        {
            // we gon use this to stop permaslam
            AddFloat("SlamCooldown", 0.0f);
            // (welcome to the jam)

            AddBool("HitPlayer", false);
            AddFloat("PauseTimer", 0.0f);
        }
    }
}