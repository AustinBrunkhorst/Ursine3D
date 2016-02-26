/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** Model3DProcessor.h
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
        class Model3DProcessor : public GraphicsEntityProcessor
        {
        public:
            Model3DProcessor(void);

        private:
             bool cullOperation(_DRAWHND, Camera &currentCamera) override;
             void prepOperation(_DRAWHND, SMat4 &view, SMat4 &proj, Camera &currentCamera) override;
             void renderOperation(_DRAWHND handle, Camera &currentCamera) override;

             void renderFullModel(_DRAWHND handle, bool renderDebug = false);
             void renderSection(_DRAWHND handle, bool renderDebug = false);
        };
    }
}