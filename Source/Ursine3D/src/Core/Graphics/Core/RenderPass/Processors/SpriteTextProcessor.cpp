/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** SpriteTextProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "SpriteTextProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        SpriteTextProcessor::SpriteTextProcessor(void)
        {
            m_renderableType = RENDERABLE_SPRITE_TEXT;
        }

        bool SpriteTextProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            SpriteText spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );

            // if inactive
            if (!spriteText.GetActive())
                return true;

            // if culed by camera mask
            if (currentCamera.CheckMask(spriteText.GetRenderMask( )))
                return true;

            if ( spriteText.GetText().length() <= 0 )
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void SpriteTextProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            SpriteText spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );
        }

        void SpriteTextProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            SpriteText spriteText = m_manager->renderableManager->GetRenderableByID<SpriteText>( handle.Index_ );
            m_manager->shaderManager->Render( 6 * static_cast<unsigned>( spriteText.GetText( ).length( ) ) );
        }
    }
}