#include "UrsinePrecompiled.h"

#include "RigidbodyComponent.h"
#include "EntityEvent.h"
#include "PhysicsSystem.h"

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
            m_rigidbody.SetID( GetOwner( )->GetUniqueID( ) );

			GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Rigidbody::onTransformChange );
		}

        BodyType Rigidbody::GetBodyType(void) const
        {
            return static_cast<BodyType>( m_rigidbody.GetBodyType( ) );
        }

        void Rigidbody::SetBodyType(BodyType bodyType)
        {
            if (bodyType == BodyType::Static || bodyType == BodyType::Dynamic)
                GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem )->ClearContacts( this );

            m_rigidbody.SetBodyType(
                static_cast<physics::BodyType>( bodyType )
            );

            if (bodyType == BodyType::Dynamic)
            {
                m_rigidbody.SetGravity( 
                    GetOwner( )->GetWorld( )->GetEntitySystem( PhysicsSystem )->GetGravity( )
                );
            }
        }

        void Rigidbody::SetAwake(void)
        {
            m_rigidbody.SetAwake( );
        }

        void Rigidbody::SetOffset(const SVec3 &offset)
        {
            m_rigidbody.SetOffset( offset );
            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }

        SVec3 Rigidbody::GetOffset(void) const
        {
            return m_rigidbody.GetOffset( );
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

        void Rigidbody::UpdateInertiaTensor(void)
        {
            m_rigidbody.UpdateInertiaTensor( );
        }

        void Rigidbody::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
