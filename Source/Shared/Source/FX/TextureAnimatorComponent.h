/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShieldFXComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#pragma once

#include <Component.h>
#include <Renderable.h>

#include <Model3DComponent.h>

class TextureAnimator : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        bool isPlaying,
        GetIsPlaying,
        SetIsPlaying
    );

    EditorField(
        ursine::Vec2 textureVelocity,
        GetTextureVelocity,
        SetTextureVelocity
    );

    EditorField(
        ursine::Vec2 textureScalar,
        GetTextureScalar,
        SetTextureScalar
    );

    Meta(Enable)
    TextureAnimator(void);

    Meta(Enable)
    void OnInitialize(void) override;

    bool GetIsPlaying(void) const;
    void SetIsPlaying(bool isPlaying);

    ursine::Vec2 GetTextureVelocity(void) const;
    void SetTextureVelocity(const ursine::Vec2 &textureVel);

    ursine::Vec2 GetTextureScalar(void) const;
    void SetTextureScalar(const ursine::Vec2 &textureScalar);
private:
    ursine::Vec2 m_textureVelocity;

    bool m_isPlaying;
} 
Meta(Enable, WhiteListMethods, DisplayName("TextureAnimator"))
EditorMeta(
    RequiresComponents(typeof(ursine::ecs::Model3D))
);
