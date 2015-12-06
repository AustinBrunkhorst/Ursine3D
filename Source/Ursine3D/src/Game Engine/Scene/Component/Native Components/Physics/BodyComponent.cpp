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
        {
            
        }

		Body::~Body(void)
		{
			GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &Body::onTransformChange );
		}

		void Body::OnInitialize(void)
		{
			GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Body::onTransformChange );

            m_body.setUserIndex( GetOwner( )->GetUniqueID( ) );
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

		void Body::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_body.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
