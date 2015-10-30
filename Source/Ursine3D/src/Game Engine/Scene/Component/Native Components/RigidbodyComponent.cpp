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

        void Rigidbody::SetAwake(void)
        {
            m_rigidbody.SetAwake( );
        }

        void Rigidbody::SetOffset(const SVec3 &offset)
        {
            m_rigidbody.SetOffset( offset );
        }

        void Rigidbody::LockXRotation(bool flag)
        {
            m_rigidbody.LockXRotation( flag );
        }

        void Rigidbody::LockYRotation(bool flag)
        {
            m_rigidbody.LockYRotation( flag );
        }

        void Rigidbody::LockZRotation(bool flag)
        {
            m_rigidbody.LockZRotation( flag );
        }

        SVec3 Rigidbody::GetOffset(void) const
        {
            return m_rigidbody.GetOffset( );
        }

        void Rigidbody::onTransformChange(EVENT_HANDLER(Entity))
        {
            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
