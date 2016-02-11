/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxProfiler.h
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include "D3D11Forward.h"
#include "ProfileTargetList.h"
#include "GfxDefines.h"

#define STAMP(profileName) gfxProfiler->Stamp(#profileName);

namespace ursine
{
    namespace graphics
    {
        class GfxProfiler
        {
        public:
            void Initialize(ID3D11Device *device, ID3D11DeviceContext *devCon, bool run = false);
            void Uninitialize();

            //start frame
            void BeginFrame();

            //end frame
            void EndFrame();

            //call "Time" on each segment
            void Stamp(std::string name);

            //wait for calls to complete before flipping swap chain
            void WaitForCalls(bool output = false);
        private:

            //device/device context
            ID3D11Device *m_device;
            ID3D11DeviceContext *m_devCon;

            //what queries are we using?
            int m_frame;
            //last frame we retrieved data from
            int m_lastFrame;

            //should we run?
            bool m_run;

            int m_numberOfTargets;

            int m_maxNameSize;

            UINT64 m_lastBaseLine;

            //disjointed queries
            ID3D11Query *m_disjointedQueries[ 2 ];

            //data queries
            ID3D11Query *m_queryList[ MAX_PROFILER_TARGETS ][ 2 ];

            std::vector<std::string> m_targetNames;

            std::vector<bool> m_profiled;

            //the data
            std::vector<double> m_timeStamps;
        };
    }
}
