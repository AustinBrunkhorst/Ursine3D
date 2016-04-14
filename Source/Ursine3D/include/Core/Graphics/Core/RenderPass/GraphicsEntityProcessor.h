/* ----------------------------------------------------------------------------
** Team Bear King
** ?2016 DigiPen Institute of Technology, All Rights Reserved.
**
** GraphicsEntityProcessor.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "GfxDefines.h"

namespace ursine
{
    namespace graphics
    {
        class GfxManager;
        class Camera;

        class GraphicsEntityProcessor
        {
        public:
            // process entities from here to there
            void Process(const std::vector<_DRAWHND> &drawcallList, Camera &currentCamera, unsigned start, unsigned end);
            virtual ~GraphicsEntityProcessor(void);

            static void SetGfxMgr(GfxManager *mgr);

        protected:
            static GfxManager *m_manager;

            virtual bool cullOperation(_DRAWHND, Camera &currentCamera) = 0;
            virtual void prepOperation(_DRAWHND, SMat4 &view, SMat4 &proj, Camera &currentCamera) = 0;
            virtual void renderOperation(_DRAWHND handle, Camera &currentCamera) = 0;

            // what does this processor process?
            unsigned m_renderableType;
        };
    }
}