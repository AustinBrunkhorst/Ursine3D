#pragma once

/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RevivePlayerComponent.h
**
** Author:
** - Chad George - chad.george@digipen.edu
**
** -------------------------------------------------------------------------*/


#include <Component.h>
#include "BaseInteraction.h"
#include "TextureData.h"

namespace ursine{
    namespace ecs{
        class EntityHandle;
    }
}

class RevivePlayer 
    : public ursine::ecs::Component
    , public Interaction
{
    NATIVE_COMPONENT
public:

    RevivePlayer(void);
    ~RevivePlayer(void);

    EditorField(
        float ReviveTime,
        GetReviveTime,
        SetReviveTime
        );
    float GetReviveTime(void) const;
    void SetReviveTime(float time);

    EditorResourceField(
        ursine::resources::TextureData,
        Texture,
        GetTexture,
        SetTexture
        );
    const ursine::resources::ResourceReference& GetTexture(void) const;
    void SetTexture(const ursine::resources::ResourceReference& texture);


protected:
    void OnInitialize(void) override;

private:
    // interaction logic
    void StartInteraction(const ursine::ecs::EntityHandle& entity) override;
    void Interact(const ursine::ecs::EntityHandle& entity) override;
    void StopInteraction(const ursine::ecs::EntityHandle& entity) override;
    void InteractionComplete(void);

    void OnReviveCheat(EVENT_HANDLER(ursine::ecs::Entity));

    void messageUIToggle(const ursine::ecs::EntityHandle &reviver, bool toggle);
    void messageUIProgress(const ursine::ecs::EntityHandle &reviver, float time);
    void messageUISuccess(const ursine::ecs::EntityHandle &reviver);

    void revivingSfx(const ursine::ecs::EntityHandle &reviver);
    void revivedSfx(void);

    // command queues
    std::unordered_map<ursine::ecs::EntityHandle, CommandQueue*> m_queues;

    // how much time has button been held
    std::unordered_map<ursine::ecs::EntityHandle, float> m_times;

    // time it takes to revive player
    float m_reviveTime;

    // texture to display
    ursine::resources::ResourceReference m_texture;

} Meta(Enable,
    DisplayName( "RevivePlayer" )
) EditorMeta(
    RequiresComponents( typeof( Interactable ) )
);

