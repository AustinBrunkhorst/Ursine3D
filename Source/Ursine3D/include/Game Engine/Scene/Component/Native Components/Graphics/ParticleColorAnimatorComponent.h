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
#include "GfxAPI.h"
#include "RenderableComponentBase.h"
#include "Color.h"
#include "ParticleSystemComponent.h"

#define COLOR_FIELD(colorNum) EditorField(Color color##colorNum, GetColor##colorNum, SetColor##colorNum)

namespace ursine
{
    namespace ecs
    {
        class ParticleColorAnimator
            : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField(Color color0, GetColor0, SetColor0);
            EditorField(Color color1, GetColor1, SetColor1);
            EditorField(Color color2, GetColor2, SetColor2);
            EditorField(Color color3, GetColor3, SetColor3);
            EditorField(Color color4, GetColor4, SetColor4);

            ParticleColorAnimator(void);
            ~ParticleColorAnimator(void);

            Meta(Disable)
            void OnInitialize(void) override;

            const Color &GetColor0(void) const;
            void SetColor0(const Color &color);

            const Color &GetColor1(void) const;
            void SetColor1(const Color &color);

            const Color &GetColor2(void) const;
            void SetColor2(const Color &color);

            const Color &GetColor3(void) const;
            void SetColor3(const Color &color);

            const Color &GetColor4(void) const;
            void SetColor4(const Color &color);

        private:
            void onParticleUpdate(EVENT_HANDLER(Entity));

            Color interpolateColor(const Color &start, const Color &end, const float scalar);

            // parent component, this is kinda important
            ParticleSystem *m_particleComponent;

            Color m_color0;
            Color m_color1;
            Color m_color2;
            Color m_color3;
            Color m_color4;

        } Meta(
            Enable,
            DisplayName( "ParticleColorAnimator" )
        ) EditorMeta( 
            RequiresComponents( typeof( ursine::ecs::ParticleSystem ) )    
        );
    }
}