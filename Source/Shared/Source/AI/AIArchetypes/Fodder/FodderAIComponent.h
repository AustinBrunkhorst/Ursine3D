/* ---------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** FodderAIComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "FodderAIStateMachine.h"
#include "AIMovementControllerComponent.h"
#include <limits>
#include <Game Engine/Scene/Component/Native Components/Physics/CapsuleColliderComponent.h>

class FodderAI : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorMeta(InputRange(0.0f, std::numeric_limits<float>::infinity()))
    EditorField(
        float pauseTimeAfterAttack,
        GetPauseTime,
        SetPauseTime
        );

    EditorField(
        float damageOnAttack,
        GetDamage,
        SetDamage
        );

    EditorField(
        float Cohesion,
        GetCohesionScale,
        SetCohesionScale
        );

    EditorField(
        float Separation,
        GetSeparationScale,
        SetSeparationScale
        );

    EditorField(
        float TotalBoidBehaviorScale,
        GetBoidScale,
        SetBoidScale
        );

  FodderAI(void);
  ~FodderAI(void);

  float GetPauseTime(void) const;
  void SetPauseTime(float newTime);

  float GetDamage(void) const;
  void SetDamage(float dmg);

  float GetCohesionScale(void) const;
  void SetCohesionScale(float newScale);

  float GetSeparationScale(void) const;
  void SetSeparationScale(float newScale);

  float GetBoidScale(void) const;
   void SetBoidScale(float newScale);

private:

    void OnInitialize(void) override;

    void onUpdate(EVENT_HANDLER(World));

    Meta(Disable)
    ursine::sm::FodderAIStateMachine m_stateMachine;

    float m_pauseTime;

    float m_damage;

    float m_cohesionScale;

    float m_separationScale;

    float m_boidBehaviorScale;

} Meta(Enable, DisplayName("FodderAI"), RequiresComponents(typeof(ursine::ecs::AIMovementController)));
