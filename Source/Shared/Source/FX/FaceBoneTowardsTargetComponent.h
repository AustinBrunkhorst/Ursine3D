/*---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** FaceBoneTowardsTargetComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <SVec3.h>

class FaceBoneTowardsTarget : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:

    EditorButton(
        visualizeFOV,
        "Visualize Field Of View"
    );

    EditorField(
        ursine::SVec3 localForwardVector,
        GetLocalForward,
        SetLocalForward
    );

    Meta(InputRange(0.0f, 90.0f, 0.1f, "{{value.toFixed( 2 )}} deg"))
    EditorField(
        float maxViewAngle,
        GetMaxViewAngle,
        SetMaxViewAngle
    );

    FaceBoneTowardsTarget(void);
    ~FaceBoneTowardsTarget(void);

    void OnSceneReady(ursine::Scene *scene) override;

    void SetTargetPosition(const ursine::SVec3 &worldPosition);

    const ursine::SVec3 &GetLocalForward(void) const;
    void SetLocalForward(const ursine::SVec3 &forward);

    float GetMaxViewAngle(void) const;
    void SetMaxViewAngle(float degrees);

private:

    bool m_subscribed;

    float m_maxViewAngle;

    ursine::SVec3 m_localForward;

    ursine::SVec3 m_targetPosition;

    void onUpdate(EVENT_HANDLER(ursine::ecs::Entity));

    void onAnimationManipulation(EVENT_HANDLER(ursine::ecs::Entity));

} Meta(Enable);
