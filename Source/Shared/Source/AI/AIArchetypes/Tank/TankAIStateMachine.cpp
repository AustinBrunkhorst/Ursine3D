#include "Precompiled.h"

#include "TankAIStateMachine.h"

namespace ursine
{
    namespace sm
    {
        TankAIStateMachine::TankAIStateMachine()
        {
            // we gon use this to stop permaslam
            AddFloat("SlamCooldown", 0.0f);
            // (welcome to the jam)

            AddBool("HitPlayer", false);
            AddFloat("PauseTimer", 0.0f);
        }
    }
}