/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** RenderableComponent.h
** - Renderable component
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

////////////////////////////////////////////////////////////////////////////////
#ifndef RENDERABLE_COMPONENT_H
#define RENDERABLE_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#include "Component.h"

#include "Renderlayer.h"

#include "Texture2D.h"

#include "Color.h"

namespace ursine
{
    namespace ecs
    {
        enum RenderEffectType;

        class Renderable : public Component
        {
            bool _visible;

            SerializerAllow(Renderable, layer);
            RenderLayer layer;

            SerializerAllow(Renderable, texture);
            Texture2D *texture;

            SerializerAllow(Renderable, tint);
            Color tint;

            SerializerAllow(Renderable, effect);
            RenderEffectType effect;

            // top left
            Vec2 _uv_start;

            // bottom right
            Vec2 _uv_end;

            Vec2 _uv_scale;
        public:
            Renderable(void);

            ~Renderable(void);

            void OnInitialize(void) override;

            bool GetVisible(void);
            void SetVisible(bool visible = true);

            inline RenderLayer GetLayer(void);

            // Sets a new layer for this entity. Requires resorting, try to avoid.
            void SetLayer(RenderLayer layer);

            inline Texture2D *GetTexture(void);

            // Sets a new texture for this entity
            void SetTexture(Texture2D *texture);

            RenderEffectType GetEffect(void) const;

            const Color &GetTint(void) const;
            void SetTint(const Color &tint);

            const Vec2 &GetUVOffset(void) const;
            const Vec2 &GetUVScale(void) const;

            void SetUVOffset(const Vec2 &offset);
            void SetUVScale(const Vec2 &scale);

            URSINE_TODO("this will be removed after actual graphics pipeline is finalized");
            const Vec2 &GetUVEnd(void);
        };
    }
}

#include "RenderableComponent.hpp"

////////////////////////////////////////////////////////////////////////////////
#endif // !RENDERABLE_COMPONENT_H
////////////////////////////////////////////////////////////////////////////////

#ifdef REGISTER_COMPONENTS
#ifndef RENDERABLE_REGISTERED
#define RENDERABLE_REGISTERED
namespace ursine
{
    namespace ecs 
    {
        RegisterComponent(Renderable);
        RegisterComponentData(Renderable, texture);
        RegisterComponentData(Renderable, effect);
        RegisterComponentData(Renderable, layer);
        RegisterComponentData(Renderable, tint);
    }
}
#endif
#endif
