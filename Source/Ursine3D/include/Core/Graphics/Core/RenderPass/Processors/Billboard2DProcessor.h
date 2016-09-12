/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Billboard2DProcessor.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GraphicsEntityProcessor.h"

namespace ursine
{
    class SMat4;

    namespace graphics
    {
        class Billboard2DProcessor : public GraphicsEntityProcessor
        {
        public:
            Billboard2DProcessor(void);
        private:
             virtual bool cullOperation(_DRAWHND, Camera &currentCamera);
             virtual void prepOperation(_DRAWHND, SMat4 &view, SMat4 &proj, Camera &currentCamera);
             virtual void renderOperation(_DRAWHND handle, Camera &currentCamera);
        };
    }
}
