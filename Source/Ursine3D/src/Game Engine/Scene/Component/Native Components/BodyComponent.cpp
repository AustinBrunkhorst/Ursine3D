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
		}

        void Body::SetOffset(const SVec3 &offset)
        {
            m_body.SetOffset( offset );
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
