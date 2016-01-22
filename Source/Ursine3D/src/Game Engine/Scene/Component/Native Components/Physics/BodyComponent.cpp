/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** BodyComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "BodyComponent.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Body );

        Body::Body(void)
            : BaseComponent( )
			, m_enableContactCallback( false )
        {
            
        }

		Body::~Body(void)
		{
			GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &Body::onTransformChange );
		}

		void Body::OnInitialize(void)
		{
            Component::OnInitialize( );

			GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Body::onTransformChange );

            m_body.SetUserID( GetOwner( )->GetUniqueID( ) );
			m_body.SetUserPointer( this );
		}

        void Body::SetOffset(const SVec3 &offset)
        {
            m_body.SetOffset( offset );
            m_body.SetTransform( GetOwner( )->GetTransform( ) );
        }

	    SVec3 Body::GetOffset(void) const
        {
            return m_body.GetOffset( );
        }

		void Body::SetGhost(bool enable)
		{
			m_body.SetGhost( enable );
		}

	    bool Body::GetGhost(void) const
		{
			return m_body.GetGhost( );
		}

		void Body::SetEnableContactCallback(bool enable)
		{
			m_enableContactCallback = enable;
		}

		bool Body::GetEnableContactCallback(void) const
		{
			return m_enableContactCallback;
		}

	    void Body::SetAwake(void)
	    {
			m_body.SetAwake( );
	    }

	    void Body::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_body.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
