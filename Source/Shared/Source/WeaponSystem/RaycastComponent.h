/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RaycastComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include "EntityHandle.h"
#include "CharacterControllerComponent.h"

class RaycastComponent : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    RaycastComponent(void);
    ~RaycastComponent(void);

    EditorField(
        float StickyAimRange,
        GetRange,
        SetRange
    );

    float GetRange(void) const;
    void SetRange(float range);

    EditorField(
        float StickyAimModifier,
        GetRotationModifier,
        SetRotationModifier
    );

    float GetRotationModifier(void) const;
    void SetRotationModifier(float range);

    const ursine::ecs::EntityHandle& GetEntityHit(void) const;
    void SetEntityHit(ursine::ecs::EntityHandle& entity);

    ursine::ecs::Transform *GetCameraTransform(void) const;

    EditorField(
        bool Debug,
        GetDebug,
        SetDebug
    );

    bool GetDebug(void) const;
    void SetDebug(bool debug);

protected:
    void OnInitialize(void) override;

private:
    float m_range;

    float m_rotationModifier;

    // first object hit by raycast
    ursine::ecs::EntityHandle m_entityHit;

    ursine::ecs::Transform* m_camTransform;

    bool m_debug;

    void OnHeirachyFinished(EVENT_HANDLER(ursine::ecs::Entity));
} Meta(
    Enable, 
    DisplayName( "RaycastComponent" )
) EditorMeta(
    RequiresComponents( typeof( CharacterController ) )
);
