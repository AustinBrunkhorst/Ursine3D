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
        enum DEPTH_STATES : unsigned;

        class Model3DProcessor : public GraphicsEntityProcessor
        {
        public:
            Model3DProcessor(bool shadowPass = false, bool notVisiblePass = false);

        private:
             bool cullOperation(_DRAWHND, Camera &currentCamera) override;
             void prepOperation(_DRAWHND, SMat4 &view, SMat4 &proj, Camera &currentCamera) override;
             void renderOperation(_DRAWHND handle, Camera &currentCamera) override;

             void renderFullModel(_DRAWHND handle, bool renderDebug = false);
             void renderSection(_DRAWHND handle, bool renderDebug = false);

             bool m_shadowPass;
             bool m_notVisiblePass;

             DEPTH_STATES m_defaultState;
        };
    }
}