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
{
}

void TextureAnimator::OnInitialize(void)
{
}

Vec2 TextureAnimator::GetTextureVelocity(void) const
{
    return m_textureVelocity;
}

void TextureAnimator::SetTextureVelocity(const ursine::Vec2& textureVel)
{
    m_textureVelocity = textureVel;

    NOTIFY_COMPONENT_CHANGED( "textureVelocity", textureVel );
}

Vec2 TextureAnimator::GetTextureScalar(void) const
{
    return m_textureVelocity;
}

void TextureAnimator::SetTextureScalar(const ursine::Vec2& textureScalar)
{
    GetOwner( )->GetComponent<Model3D>( )->SetTextureUVScalar( textureScalar );
    NOTIFY_COMPONENT_CHANGED( "textureScalar", textureScalar );
}
