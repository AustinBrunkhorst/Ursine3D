#include "UrsinePrecompiled.h"

#include "RigidbodyComponent.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Rigidbody );

        Rigidbody::Rigidbody(void)
            : BaseComponent( )
            , m_rigidbody( 1.0f, nullptr ) { }

		Rigidbody::~Rigidbody(void)
		{
			GetOwner( )->Listener( this )
                .Off( ENTITY_TRANSFORM_DIRTY, &Rigidbody::onTransformChange );
		}

		void Rigidbody::OnInitialize(void)
		{
			GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Rigidbody::onTransformChange );
		}

        BodyType Rigidbody::GetBodyType(void) const
        {
            return static_cast<BodyType>( m_rigidbody.GetBodyType( ) );
        }

        void Rigidbody::SetBodyType(BodyType bodyType)
        {
            m_rigidbody.SetBodyType( 
                static_cast<physics::BodyType>( bodyType )
            );
        }

        void Rigidbody::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
