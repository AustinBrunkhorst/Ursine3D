/*---------------------------------------------------------------------------
** Team Bear King
** 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ShieldFXComponent.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
**-------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "TextureAnimatorComponent.h"
#include <Model3DComponent.h>

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION(TextureAnimator);


TextureAnimator::TextureAnimator(void)
    : BaseComponent( )
    , m_isDiffusePlaying( false )
    , m_isEmissivePlaying( false )
{
}

TextureAnimator::~TextureAnimator(void)
{
    auto *model = GetOwner( )->GetComponent<Model3D>( );

    model->SetTextureUVScalar( Vec2(1.0f, 1.0f) );
    model->SetTextureUVOffset( Vec2(0, 0) );

    model->SetEmissiveTextureUVScalar( Vec2(1.0f, 1.0f) );
    model->SetEmissiveTextureUVOffset( Vec2(0, 0) );
}

void TextureAnimator::OnInitialize(void)
{
}

bool TextureAnimator::GetIsDiffusePlaying(void) const
{
    return m_isDiffusePlaying;
}

void TextureAnimator::SetIsDiffusePlaying(bool isPlaying)
{
    m_isDiffusePlaying = isPlaying;
}

bool TextureAnimator::GetIsEmissivePlaying(void) const
{
    return m_isEmissivePlaying;
}

void TextureAnimator::SetIsEmissivePlaying(bool isPlaying)
{
    m_isEmissivePlaying = isPlaying;
}

Vec2 TextureAnimator::GetDiffuseTextureVelocity(void) const
{
    return m_textureVelocity;
}

void TextureAnimator::SetDiffuseTextureVelocity(const ursine::Vec2& textureVel)
{
    m_textureVelocity = textureVel;

    NOTIFY_COMPONENT_CHANGED( "diffuseTextureVelocity", textureVel );
}

Vec2 TextureAnimator::GetDiffuseTextureScalar(void) const
{
    return GetOwner( )->GetComponent<Model3D>( )->GetTextureUVScalar( );
}

void TextureAnimator::SetDiffuseTextureScalar(const ursine::Vec2& textureScalar)
{
    GetOwner( )->GetComponent<Model3D>( )->SetTextureUVScalar( textureScalar );
    NOTIFY_COMPONENT_CHANGED( "diffuseTextureScalar", textureScalar );
}

Vec2 TextureAnimator::GetEmissiveTextureVelocity(void) const
{
    return m_emissiveTextureVelocity;
}

void TextureAnimator::SetEmissiveTextureVelocity(const ursine::Vec2& textureVel)
{
    m_emissiveTextureVelocity = textureVel;

    NOTIFY_COMPONENT_CHANGED( "emissiveTextureVelocity", textureVel );
}

Vec2 TextureAnimator::GetEmissiveTextureScalar(void) const
{
    return GetOwner( )->GetComponent<Model3D>( )->GetEmissiveTextureUVScalar( );
}

void TextureAnimator::SetEmissiveTextureScalar(const ursine::Vec2& textureScalar)
{
    GetOwner( )->GetComponent<Model3D>( )->SetTextureUVScalar( textureScalar );
    NOTIFY_COMPONENT_CHANGED( "emissiveTextureScalar", textureScalar );
}