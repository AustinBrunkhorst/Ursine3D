/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** PhysicsSettingsComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "PhysicsSettingsComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( PhysicsSettings );

        PhysicsSettings::PhysicsSettings(void)
            : BaseComponent( )
            , m_physicsSystem( nullptr )
            , m_gravity( 0.0f, -10.0f, 0.0f )
        {
            
        }

        PhysicsSettings::~PhysicsSettings(void)
        {
            
        }

        SVec3 PhysicsSettings::GetGravity(void) const
        {
            if (m_physicsSystem)
                return m_physicsSystem->GetGravity( );
            else
                return SVec3::Zero( );
        }

        void PhysicsSettings::SetGravity(const SVec3& gravity)
        {
            m_gravity = gravity;

            if (m_physicsSystem)
                m_physicsSystem->SetGravity( gravity );
        }

        bool PhysicsSettings::GetEnableDebugDraw(void) const
        {
            if (m_physicsSystem)
                return m_physicsSystem->GetEnableDebugDraw( );
            else
                return false;
        }

	    void PhysicsSettings::SetEnableDebugDraw(bool enable)
        {
            if (m_physicsSystem)
                m_physicsSystem->SetEnableDebugDraw( enable );
        }

		bool PhysicsSettings::GetPlaymodeDebugDraw(void) const
		{
			if (m_physicsSystem)
				return m_physicsSystem->GetPlaymodeDebugDraw( );
			else
				return false;
		}

		void PhysicsSettings::SetPlaymodeDebugDraw(bool enable)
		{
			if (m_physicsSystem)
				m_physicsSystem->SetPlaymodeDebugDraw( enable );
		}

        void PhysicsSettings::OnInitialize(void)
        {
            m_physicsSystem = GetOwner( )->GetWorld( )->GetEntitySystem<PhysicsSystem>( );

            m_physicsSystem->SetGravity( m_gravity );
        }
    }
}