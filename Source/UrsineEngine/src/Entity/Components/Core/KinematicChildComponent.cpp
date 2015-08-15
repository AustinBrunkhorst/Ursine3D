#include "UrsinePrecompiled.h"

#include "KinematicChildComponent.h"

namespace Ursine
{
    namespace ECS
    {
        KinematicChild::KinematicChild(void)
            : BaseComponent()
        {

        }

        KinematicChild::~KinematicChild(void)
        {

        }

        void KinematicChild::OnInitialize(void)
        {
            // initialize the last position to the player's position
            _last_position = GetOwner()->GetTransform()->PositionWC();
        }
    }
}