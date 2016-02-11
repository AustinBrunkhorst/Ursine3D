/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Body.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Body.h"
#include "TransformComponent.h"

namespace ursine
{
    namespace physics
    {
	    Body::Body(void)
            : BodyBase( )
			, m_ghost( false )
        {
            
        }

		void Body::SetUserID(int id)
        {
        #ifdef BULLET_PHYSICS

            setUserIndex( id );

        #endif
        }

	    int Body::GetUserID(void)
        {
        #ifdef BULLET_PHYSICS

            return getUserIndex( );

        #endif
        }

		void Body::SetUserPointer(void* ptr)
		{
		#ifdef BULLET_PHYSICS

			setUserPointer( ptr );

		#endif
		}

		void *Body::GetUserPointer(void)
		{
		#ifdef BULLET_PHYSICS

			return getUserPointer( );

		#endif
		}

		Body* Body::DownCast(BodyBase* body)
		{
		#ifdef BULLET_PHYSICS

			if (!body || body->getInternalType( ) != BT_BODY)
				return nullptr;

			return reinterpret_cast<Body*>( body );

		#endif
		}

		const Body* Body::DownCast(const BodyBase* body)
		{
		#ifdef BULLET_PHYSICS

			if (!body || body->getInternalType() != BT_BODY)
				return nullptr;

			return reinterpret_cast<const Body*>( body );

		#endif
		}

        void Body::SetTransform(ecs::Transform* transform)
        {
        #ifdef BULLET_PHYSICS

            auto rot = transform->GetWorldRotation( );
            auto pos = transform->GetWorldPosition( ) + rot * m_offset;
            
            setWorldTransform(btTransform(
                btQuaternion( rot.X( ), rot.Y( ), rot.Z( ), rot.W( ) ),
                btVector3( pos.X( ), pos.Y( ), pos.Z( ) )
            ));

        #endif
        }

        void Body::GetTransform(ecs::Transform* transform)
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

        void Body::SetCollider(ColliderBase* collider)
        {
		#ifdef BULLET_PHYSICS

            setCollisionShape( collider );

		#endif
        }

	    ColliderBase *Body::GetCollider(void)
	    {
		#ifdef BULLET_PHYSICS

			return getCollisionShape( );

		#endif
	    }

        void Body::SetOffset(const SVec3 &offset)
        {
            m_offset = offset;
        }

	    void Body::SetGhost(bool enable)
	    {
			m_ghost = enable;

			if (m_ghost)
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

	    bool Body::GetGhost(void) const
	    {
			return m_ghost;
	    }

	    void Body::SetAwake(void)
	    {
		#ifdef BULLET_PHYSICS

			activate( );

		#endif
	    }

	    SVec3 Body::GetOffset(void) const
        {
            return m_offset;
        }
    }
}