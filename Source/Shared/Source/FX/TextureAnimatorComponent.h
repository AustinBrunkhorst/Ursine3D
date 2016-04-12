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
        bool playDiffuse,
        GetIsDiffusePlaying,
        SetIsDiffusePlaying
    );

    EditorField(
        ursine::Vec2 diffuseTextureVelocity,
        GetDiffuseTextureVelocity,
        SetDiffuseTextureVelocity
    );

    EditorField(
        ursine::Vec2 diffuseTextureScalar,
        GetDiffuseTextureScalar,
        SetDiffuseTextureScalar
    );

    EditorField(
        bool playEmissive,
        GetIsEmissivePlaying,
        SetIsEmissivePlaying
    );

    EditorField(
        ursine::Vec2 emissiveTextureVelocity,
        GetEmissiveTextureVelocity,
        SetEmissiveTextureVelocity
    );

    EditorField(
        ursine::Vec2 emissiveTextureScalar,
        GetEmissiveTextureScalar,
        SetEmissiveTextureScalar
    );


    Meta(Enable)
    TextureAnimator(void);

    ~TextureAnimator(void);

    Meta(Enable)
    void OnInitialize(void) override;

    bool GetIsDiffusePlaying(void) const;
    void SetIsDiffusePlaying(bool isPlaying);

    bool GetIsEmissivePlaying(void) const;
    void SetIsEmissivePlaying(bool isPlaying);

    ursine::Vec2 GetDiffuseTextureVelocity(void) const;
    void SetDiffuseTextureVelocity(const ursine::Vec2 &textureVel);

    ursine::Vec2 GetDiffuseTextureScalar(void) const;
    void SetDiffuseTextureScalar(const ursine::Vec2 &textureScalar);

    ursine::Vec2 GetEmissiveTextureVelocity(void) const;
    void SetEmissiveTextureVelocity(const ursine::Vec2 &textureVel);

    ursine::Vec2 GetEmissiveTextureScalar(void) const;
    void SetEmissiveTextureScalar(const ursine::Vec2 &textureScalar);

private:
    ursine::Vec2 m_textureVelocity;
    ursine::Vec2 m_emissiveTextureVelocity;

    bool m_isDiffusePlaying;
    bool m_isEmissivePlaying;
} 
Meta(Enable, WhiteListMethods, DisplayName("TextureAnimator"))
EditorMeta(
    RequiresComponents(typeof(ursine::ecs::Model3D))
);
