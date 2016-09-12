/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GraphicsEntityProcessor.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "GraphicsEntityProcessor.h"

#include "Camera.h"

namespace ursine
{
    namespace graphics
    {
        GfxManager *GraphicsEntityProcessor::m_manager = nullptr;

        void GraphicsEntityProcessor::Process(
            const std::vector<_DRAWHND>& drawcallList, 
            Camera &currentCamera, 
            unsigned start, 
            unsigned end
        )
        {
            SMat4 projection = currentCamera.GetProjMatrix( );
            SMat4 view = currentCamera.GetViewMatrix( );

            for(unsigned index = start; index < end; ++index)
            {
                auto &handle = drawcallList[index];

                // if it needs to be culled, cull it
                if(cullOperation( handle, currentCamera ))
                    continue;

                // prep for a render
                prepOperation( handle, view, projection, currentCamera );

                // render object
                renderOperation( handle, currentCamera );
            }
        }

        GraphicsEntityProcessor::~GraphicsEntityProcessor(void)
        {
        }

        void GraphicsEntityProcessor::SetGfxMgr(GfxManager *mgr)
        {
            m_manager = mgr;
        }
    }
}