/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** DirectionalLightProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/
#include "UrsinePrecompiled.h"

#include "DirectionalLightProcessor.h"
#include "Renderable.h"
#include "GfxManager.h"

namespace ursine
{
    namespace graphics
    {
        DirectionalLightProcessor::DirectionalLightProcessor(void)
        {
            m_renderableType = RENDERABLE_LIGHT;
        }

        bool DirectionalLightProcessor::cullOperation(_DRAWHND handle, Camera &currentCamera)
        {
            UAssert(handle.Type_ == m_renderableType, "GfxEntityProcessor attempted to proces invalid type!");

            Light light = m_manager->renderableManager->GetRenderableByID<Light>(handle.Index_);

            // if inactive
            if ( !light.GetActive() )
                return true;

            // if culed by camera mask
            if ( currentCamera.CheckMask(light.GetRenderMask()) )
                return true;

            // return false as in DO NOT CULL ME
            return false;
        }

        void DirectionalLightProcessor::prepOperation(_DRAWHND handle, SMat4 &view, SMat4 &proj, Camera &currentCamera)
        {
            Light light = m_manager->renderableManager->GetRenderableByID<Light>(handle.Index_);
        }

        void DirectionalLightProcessor::renderOperation(_DRAWHND handle, Camera &currentCamera)
        {
            Light light = m_manager->renderableManager->GetRenderableByID<Light>(handle.Index_);
            m_manager->shaderManager->Render(
                m_manager->modelManager->GetModelVertcountByID(
                    m_manager->modelManager->GetModelIDByName("internalQuad")
                )
            );
        }
    }
}