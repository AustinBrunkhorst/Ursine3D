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
            auto owner = GetOwner( );

            m_rigidbody.SetSimulation( 
                &owner->GetWorld( )->GetEntitySystem( PhysicsSystem )->m_simulation 
            );

            m_rigidbody.SetID( owner->GetUniqueID( ) );

			GetOwner( )->Listener( this )
                .On( ENTITY_TRANSFORM_DIRTY, &Rigidbody::onTransformChange );
		}

        BodyType Rigidbody::GetBodyType(void) const
        {
            return static_cast<BodyType>( m_rigidbody.GetBodyType( ) );
        }

        float Rigidbody::GetMass(void) const
        {
            return m_rigidbody.GetMass( );
        }

        void Rigidbody::SetMass(float mass)
        {
            m_rigidbody.SetMass( mass );
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
            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }

        SVec3 Rigidbody::GetOffset(void) const
        {
            return m_rigidbody.GetOffset( );
        }

        void Rigidbody::SetRotationFreezeX(bool flag)
        {
            m_rigidbody.SetRotationFreezeX( flag );
        }

        bool Rigidbody::GetRotationFreezeX(void) const
        {
            return m_rigidbody.GetRotationFreezeX( );
        }

        void Rigidbody::SetRotationFreezeY(bool flag)
        {
            m_rigidbody.SetRotationFreezeY( flag );
        }

        bool Rigidbody::GetRotationFreezeY(void) const
        {
            return m_rigidbody.GetRotationFreezeY( );
        }

        void Rigidbody::SetRotationFreezeZ(bool flag)
        {
            m_rigidbody.SetRotationFreezeZ( flag );
        }

        bool Rigidbody::GetRotationFreezeZ(void) const
        {
            return m_rigidbody.GetRotationFreezeZ( );
        }

        void Rigidbody::UpdateInertiaTensor(void)
        {
            m_rigidbody.UpdateInertiaTensor( );
        }

        void Rigidbody::SetVelocity(const SVec3 &velocity)
        {
            m_rigidbody.SetVelocity( velocity );
        }

        SVec3 Rigidbody::GetVelocity(void) const
        {
            return m_rigidbody.GetVelocity( );
        }

        void Rigidbody::SetAngularVelocity(const SVec3 &angularVelocity)
        {
            m_rigidbody.SetAngularVelocity( angularVelocity );
        }

        SVec3 Rigidbody::GetAngularVelocity(void) const
        {
            return m_rigidbody.GetAngularVelocity( );
        }

        void Rigidbody::onTransformChange(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);

            m_rigidbody.SetTransform( GetOwner( )->GetTransform( ) );
        }
    }
}
