#include "Precompiled.h"

#include "WaypointAgentComponent.h"
#include "AIMovementControllerComponent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION(AIMovementController);

        AIMovementController::AIMovementController(void)
            : BaseComponent( )
            , m_rigid()
            , m_speed(0.0f)
        {
        }

        void AIMovementController::SetTargetDirection(const Vec3& dir)
        {
            m_targetDir = Vec3 (dir.X(), 0, dir.Z() );
        }

        void AIMovementController::SettargetDirection(const Vec2& dir)
        {
            m_targetDir = Vec3( dir.X(), 0, dir.Y() );

            m_targetDir.Normalize();
        }

        void AIMovementController::OnInitialize(void)
        {
            Component::OnInitialize( );

            m_rigid = static_cast< Handle<Rigidbody> >(GetOwner()->GetComponent<Rigidbody>());
        }

        void AIMovementController::Update()
        {
            m_rigid = static_cast< Handle<Rigidbody> >(GetOwner()->GetComponent<Rigidbody>());


            auto transform = GetOwner()->GetTransform();

            auto gravity = Vec3(0, m_rigid->GetVelocity().Y(), 0);

            m_rigid->SetVelocity (gravity + m_targetDir * m_speed );

            auto lookangle = transform->GetForward().Dot(m_targetDir);

            auto oldV = m_rigid->GetAngularVelocity();

            m_rigid->SetAngularVelocity( Vec3(oldV.X(), lookangle, oldV.Z()) );
        }

        float AIMovementController::GetSpeed() const
        {
            return m_speed;
        }

        void AIMovementController::SetSpeed(float newSpeed)
        {
            m_speed = newSpeed;
        }
    }
}
