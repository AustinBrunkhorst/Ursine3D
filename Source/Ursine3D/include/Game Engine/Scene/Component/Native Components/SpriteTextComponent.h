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

            // not ready for the world yet
            //EditorField(
            //    float pixelsPerUnit,
            //    GetPPU,
            //    SetPPU
            //);

            EditorField(
                TextAlignment alignment,
                GetAlignment,
                SetAlignment
            );

            EditorField(
                bool useFiltering,
                GetFilter,
                SetFilter
            );

            EditorField(
                Color textColor,
                GetColor,
                SetColor
            );

            Meta(Enable)
            SpriteText(void);
            ~SpriteText(void);

            void OnInitialize(void) override;

            float GetSize(void) const;
            void SetSize(float size);

            float GetWidth(void) const;
            void SetWidth(float size);

            float GetHeight(void) const;
            void SetHeight(float size);

            const SVec3 &GetPosition(void) const;
            void SetPosition(const SVec3 &position);

            const std::string &GetText(void) const;
            void SetText(const std::string &text);

            float GetPPU(void) const;
            void SetPPU(float ppu);

            TextAlignment GetAlignment(void) const;
            void SetAlignment(TextAlignment alignment);

            bool GetFilter(void) const;
            void SetFilter(bool useFilter);

            const Color &GetColor(void) const;
            void SetColor(const Color &color);

        private:
            graphics::SpriteText *m_spriteText;

            RenderableComponentBase *m_base;

            void updateRenderer(void);

        } Meta(Enable, WhiteListMethods, DisplayName( "SpriteText" ));
    }
}
