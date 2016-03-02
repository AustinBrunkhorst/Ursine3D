/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
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
#include "SVec3.h"
#include "Vec2.h"
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

            void SetTargetDirection(const SVec3 &dir);
                                   
            void SetTargetDirection(const Vec2 &dir);

            EditorField(
                float Speed,
                GetSpeed,
                SetSpeed
                );

            EditorField(
                bool enabled,
                GetEnable,
                SetEnable
            );

            void OnInitialize(void) override;

            void Update(void);

            bool GetEnable(void) const;
            void SetEnable(bool enable);

            float GetSpeed(void) const;
            void SetSpeed(float newSpeed);

        private:
            SVec3 m_targetDir;

            Handle<Rigidbody> m_rigid;


            float m_speed;

            bool m_enable;
        } Meta(Enable, DisplayName("AIMovementController"));
    }
}
