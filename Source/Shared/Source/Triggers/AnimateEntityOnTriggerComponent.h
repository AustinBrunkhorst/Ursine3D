/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** AnimateEntityOnTriggerComponent.h
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <GhostComponent.h>

class AnimateEntityOnTrigger : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    AnimateEntityOnTrigger(void);
    ~AnimateEntityOnTrigger(void);

    EditorField(
        std::string entityName,
        GetEntityName,
        SetEntityName
    );

    EditorField(
        bool destroyOnPlay,
        GetDestroyOnPlay,
        SetDestroyOnPlay
    );

    const std::string &GetEntityName(void) const;
    void SetEntityName(const std::string &name);

    bool GetDestroyOnPlay(void) const;
    void SetDestroyOnPlay(bool flag);

    void OnSceneReady(ursine::Scene *scene) override;

private:
    std::string m_name;
    bool m_destroyOnPlay;

    ursine::ecs::EntityHandle m_entity;

    bool m_unsubscribed;

    void onUpdate(EVENT_HANDLER(ursine::ecs::World));

} Meta(
    Enable
) EditorMeta(
    RequiresComponents( typeof( ursine::ecs::Ghost ) )    
);
