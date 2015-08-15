/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableComponent.cpp
**
** Author:
** - Jordan Ellis - J.Ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "RenderableComponent.h"
#include "RenderSystem.h"
#include "Entity.h"

namespace Ursine
{
    namespace ECS
    {
        Renderable::Renderable(void)
            : BaseComponent()
            , _visible( true )
            , effect( FX_DEFAULT )
            , _uv_start { 0.0f, 0.0f }
            , _uv_end { 1.0f, 1.0f }
            , _uv_scale { 1.0f, 1.0f }
        {
            
        }

        Renderable::~Renderable(void)
        {
            
        }

        void Renderable::OnInitialize(void)
        {
            UAssert(effect >= FX_DEFAULT && effect < FX_COUNT, 
                "Unknown render effect type (%i).", 
                effect);
        }

        bool Renderable::GetVisible(void)
        {
            return _visible;
        }

        void Renderable::SetVisible(bool visible)
        {
            _visible = visible;
        }

        void Renderable::SetLayer(RenderLayer layer)
        {
            // hasn't changed
            if (layer == this->layer)
                return;

            RenderLayer old = this->layer;

            this->layer = layer;

            GetOwner()
                ->GetWorld()
                ->GetEntitySystem(RenderSystem)
                ->OnLayerChanged(this->GetOwner(), old, layer);
        }

        void Renderable::SetTexture(Texture2D *texture)
        {
            // hasn't changed
            if (this->texture && texture->GetID() == this->texture->GetID())
                return;

            this->texture = texture;

            GetOwner()
                ->GetWorld()
                ->GetEntitySystem(RenderSystem)
                ->OnTextureChanged(this->GetOwner(), this);
        }

        RenderEffectType Renderable::GetEffect(void) const
        {
            return effect;
        }

        const Color &Renderable::GetTint(void) const
        {
            return tint;
        }

        void Renderable::SetTint(const Color &tint)
        {
            this->tint = tint;
        }

        const Vector2 &Renderable::GetUVOffset(void) const
        {
            return _uv_start;
        }

        const Vector2 &Renderable::GetUVScale(void) const
        {
            return _uv_scale;
        }

        void Renderable::SetUVOffset(const Vector2 &offset)
        {
            _uv_start = offset;
            _uv_end = offset + _uv_scale;
        }

        void Renderable::SetUVScale(const Vector2 &scale)
        {
            _uv_scale = scale;
            _uv_end = _uv_start + scale;
        }

        const Vector2 &Renderable::GetUVEnd(void)
        {
            return _uv_end;
        }
    }
}
