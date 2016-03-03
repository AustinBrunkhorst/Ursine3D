/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GhostComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GhostComponent.h"
#include "EntityEvent.h"

#include "CollisionEventArgs.h"
#include "SweptControllerComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Ghost );

        Ghost::Ghost(void)
            : BaseComponent( )
            , m_enableContactCallback( false )
        {
            
        }

        Ghost::~Ghost(void)
        {
            GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &Ghost::onTransformChange );
        }

        void Ghost::OnInitialize(void)
        {
            GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Ghost::onTransformChange );

            m_ghost.SetUserID( GetOwner( )->GetUniqueID( ) );
            m_ghost.SetUserPointer( this );

			GetOwner( )->Listener( this )
				.On( ENTITY_COLLISION_PERSISTED, &Ghost::onCollision );
        }

        void Ghost::SetOffset(const SVec3 &offset)
        {
            m_ghost.SetOffset( offset );
            m_ghost.SetTransform( GetOwner( )->GetTransform( ) );
        }

        SVec3 Ghost::GetOffset(void) const
        {
            return m_ghost.GetOffset( );
        }

        void Ghost::SetDisableContactResponse(bool disable)
        {
            m_ghost.SetDisableContactResponse( disable );
        }

        bool Ghost::GetDisableContactResponse(void) const
        {
            return m_ghost.GetDisableContactResponse( );
        }

        void Ghost::SetEnableContactCallback(bool enable)
        {
            m_enableContactCallback = enable;
        }

        bool Ghost::GetEnableContactCallback(void) const
        {
            return m_enableContactCallback;
        }

        void Ghost::SetAwake(void)
        {
            m_ghost.SetAwake( );
        }

        void Ghost::GetOverlappingPairs(std::vector<physics::GhostOverlappingItem> &pairs)
        {
            m_ghost.GetOverlappingPairs( pairs );
        }

        void Ghost::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_ghost.SetTransform( GetOwner( )->GetTransform( ) );
        }

		void Ghost::onCollision(EVENT_HANDLER(Entity))
        {
			EVENT_ATTRS(Entity, physics::CollisionEventArgs);

			if (args->otherEntity->HasComponent<SweptController>( ))
			{
				GetOffset( );
			}
        }
    }
}
