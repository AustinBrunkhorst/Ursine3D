/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** AIMovementControllerComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/
#pragma once

#include "Component.h"
#include "Vec3.h"
#include "RigidbodyComponent.h"


namespace ursine
{
    namespace ecs
    {
        class AIMovementController : public Component
        {
            NATIVE_COMPONENT;

        public:
            AIMovementController(void);

            void SetTargetDirection(const Vec3 &dir);
                                   
            void SettargetDirection(const Vec2 &dir);

            EditorField(
                float Speed,
                GetSpeed,
                SetSpeed
                );

            void OnInitialize(void) override;

            void Update(void);

            float GetSpeed(void);
            void SetSpeed(float newSpeed);

        private:
            Vec3 m_targetDir;

            Rigidbody *m_rigid;

            float m_speed;
        } Meta(Enable, DisplayName("AIMovementController"));


    }
}
