/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Ghost.cpp
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Ghost.h"

namespace ursine
{
    namespace physics
    {
        Ghost::Ghost(void)
        {
            SetDisableContactResponse( true );
        }

        void Ghost::SetUserID(int id)
        {
        #ifdef BULLET_PHYSICS

            setUserIndex( id );

        #endif
        }

        int Ghost::GetUserID(void)
        {
        #ifdef BULLET_PHYSICS

            return getUserIndex( );

        #endif
        }

        void Ghost::SetUserPointer(void *ptr)
        {
        #ifdef BULLET_PHYSICS

            setUserPointer( ptr );

        #endif
        }

        void *Ghost::GetUserPointer(void)
        {
        #ifdef BULLET_PHYSICS

            return getUserPointer( );

        #endif
        }

        Ghost *Ghost::DownCast(BodyBase *body)
        {
        #ifdef BULLET_PHYSICS

            if (!body || body->getInternalType( ) != BT_GHOST)
                return nullptr;

            return reinterpret_cast<Ghost*>( body );

        #endif
        }

        const Ghost *Ghost::DownCast(const BodyBase *body)
        {
        #ifdef BULLET_PHYSICS

            if (!body || body->getInternalType( ) != BT_GHOST)
                return nullptr;

            return reinterpret_cast<const Ghost*>( body );

        #endif
        }

        void Ghost::SetTransform(ecs::Transform *transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = transform->GetWorldRotation( );
            auto pos = transform->GetWorldPosition( ) + rot * m_offset;
            
            setWorldTransform( btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            ) );

        #endif
        }

        void Ghost::GetTransform(ecs::Transform *transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = m_worldTransform.getRotation( );
            auto pos = m_worldTransform.getOrigin( );

            transform->SetWorldRotation( 
                SQuat( rot.getX( ), rot.getY( ), rot.getZ( ), rot.getW( ) )
            );

            transform->SetWorldPosition(
                SVec3( pos.getX( ), pos.getY( ), pos.getZ( ) ) - 
                transform->GetWorldRotation( ) * m_offset
            );

        #endif
        }

        void Ghost::SetCollider(ColliderBase *collider)
        {
        #ifdef BULLET_PHYSICS

            setCollisionShape( collider );

        #endif
        }

        ColliderBase *Ghost::GetCollider(void)
        {
        #ifdef BULLET_PHYSICS

            return getCollisionShape( );

        #endif
        }

        void Ghost::SetOffset(const SVec3 &offset)
        {
            m_offset = offset;
        }

        SVec3 Ghost::GetOffset(void) const
        {
            return m_offset;
        }

        void Ghost::SetDisableContactResponse(bool disable)
        {
            m_disableContactResponse = disable;

            if (m_disableContactResponse)
            {
            #ifdef BULLET_PHYSICS

                setCollisionFlags( getCollisionFlags( ) | CF_NO_CONTACT_RESPONSE );

            #endif
            }
            else
            {
            #ifdef BULLET_PHYSICS

                setCollisionFlags( getCollisionFlags( ) & ~CF_NO_CONTACT_RESPONSE );

            #endif
            }
        }

        bool Ghost::GetDisableContactResponse(void) const
        {
            return m_disableContactResponse;
        }

        void Ghost::SetAwake(void)
        {
        #ifdef BULLET_PHYSICS

            activate( );

        #endif
        }

        void Ghost::GetOverlappingPairs(std::vector<GhostOverlappingItem> &pairs)
        {
        #ifdef BULLET_PHYSICS

            auto &p = getOverlappingPairs( );

            for (int i = 0; i < p.size( ); ++i)
            {
                auto *obj = p[ i ];
                auto type = obj->getInternalType( );

                if (type & BT_RIGID_BODY)
                    pairs.emplace_back( BT_RIGID_BODY, obj->getUserPointer( ) );
                else if (type & BT_GHOST)
                    pairs.emplace_back( BT_GHOST, obj->getUserPointer( ) );
                else if (type & BT_SOFT_BODY)
                    pairs.emplace_back( BT_SOFT_BODY, obj->getUserPointer( ) );
                else
                    pairs.emplace_back( BT_BODY, obj->getUserPointer( ) );
            }

        #endif
        }
    }
}
