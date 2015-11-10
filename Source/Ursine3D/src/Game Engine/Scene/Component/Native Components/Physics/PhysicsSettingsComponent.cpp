#include "UrsinePrecompiled.h"

#include "PhysicsSettingsComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( PhysicsSettings );

        PhysicsSettings::PhysicsSettings(void)
            : BaseComponent( )
        {
            
        }

        PhysicsSettings::~PhysicsSettings(void)
        {
            
        }

        SVec3 PhysicsSettings::GetGravity(void) const
        {
            return m_physicsSystem->GetGravity( );
        }

        void PhysicsSettings::SetGravity(const SVec3& gravity)
        {
            m_physicsSystem->SetGravity( gravity );
        }

        void PhysicsSettings::OnInitialize(void)
        {
            m_physicsSystem = GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem );
        }
    }
}