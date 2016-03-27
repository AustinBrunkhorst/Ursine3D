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

#include "FontData.h"

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
            EditorResourceField(
                ursine::resources::FontData,
                font,
                GetFont,
                SetFont
            );

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

            EditorMeta(BitMaskEditor)
            EditorField(
                ursine::ecs::RenderMask renderMask,
                GetRenderMask,
                SetRenderMask
            );

            EditorField(
                bool overdraw,
                GetOverdraw,
                SetOverdraw
            );

            Meta(Enable)
            SpriteText(void);
            ~SpriteText(void);

            void OnInitialize(void) override;
            void OnSceneReady(Scene *scene) override;

            const ursine::resources::ResourceReference &GetFont(void) const;
            void SetFont(const ursine::resources::ResourceReference &font);

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

            ursine::ecs::RenderMask GetRenderMask(void) const;
            void SetRenderMask(ursine::ecs::RenderMask mask);

            void SetOverdraw(bool flag);
            bool GetOverdraw(void) const;

        private:
            resources::ResourceReference m_font;

            graphics::SpriteText *m_spriteText;

            RenderableComponentBase::Handle m_base;

            void updateRenderer(void);
            void invalidateFont(void);

        } Meta(Enable, WhiteListMethods, DisplayName( "SpriteText" ));
    }
}
