/* ----------------------------------------------------------------------------
** Team Bear King
** 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** ParticleColorAnimatorComponent.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "Renderable.h"
#include "RenderMask.h"
#include "RenderableComponentBase.h"

namespace ursine
{
    namespace ecs
    {
        enum class TextAlignment
        {
            Left = graphics::SpriteText::ALIGN_LEFT,
            Center = graphics::SpriteText::ALIGN_CENTER,
            Right = graphics::SpriteText::ALIGN_RIGHT,
        } Meta(Enable);


        class SpriteText : public Component
        {
            NATIVE_COMPONENT;

            friend class RenderSystem;

        public:
            EditorField(
                float size,
                GetSize,
                SetSize
            );

            EditorField(
                std::string text,
                GetText,
                SetText
            );

            EditorField(
                float pixelsPerUnit,
                GetPPU,
                SetPPU
            );

            EditorField(
                TextAlignment alignment,
                GetAlignment,
                SetAlignment
            );

            SpriteText(void);
            ~SpriteText(void);

            Meta(Disable)
            void OnInitialize(void) override;

            float GetSize(void) const;
            void SetSize(float size);

            Meta(Disable)
            float GetWidth(void) const;
            Meta(Disable)
            void SetWidth(float size);

            Meta(Disable)
            float GetHeight(void) const;
            Meta(Disable)
            void SetHeight(float size);

            Meta(Disable)
            const SVec3 &GetPosition(void) const;
            Meta(Disable)
            void SetPosition(const SVec3 &position);

            const std::string &GetText(void) const;
            void SetText(const std::string &text);

            float GetPPU(void) const;
            void SetPPU(float ppu);

            TextAlignment GetAlignment(void) const;
            void SetAlignment(TextAlignment alignment);

        private:
            graphics::SpriteText *m_spriteText;

            RenderableComponentBase *m_base;

            void updateRenderer(void);

        } Meta(Enable, DisplayName("SpriteText"));
    }
}
