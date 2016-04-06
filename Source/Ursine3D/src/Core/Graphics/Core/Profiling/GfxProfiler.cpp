/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GfxProfiler.cpp
**
** Author:
** - Matt Yan - m.yan@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"
#include "GfxProfiler.h"
#include <d3d11.h>
#include "GfxDefines.h"

#define PROFILE(target) #target

namespace ursine
{
    namespace graphics
    {
        static int start = 0, end = 0;
        void GfxProfiler::Initialize(ID3D11Device *device, ID3D11DeviceContext *devCon, bool run)
        {
            m_run = run;
            if (!run)
                return;

            m_device = device;
            m_devCon = devCon;

            //init variables
            m_frame = 0;
            m_lastFrame = -1;
            m_lastBaseLine = 0;

            //create disjointed queries
            HRESULT hr;
            D3D11_QUERY_DESC queryDesc;
            queryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
            queryDesc.MiscFlags = 0;

            hr = m_device->CreateQuery(&queryDesc, &m_disjointedQueries[ 0 ]);
            UAssert(hr == S_OK, "Failed to create query device!");
            hr = m_device->CreateQuery(&queryDesc, &m_disjointedQueries[ 1 ]);
            UAssert(hr == S_OK, "Failed to create query device!");

            m_targetNames.resize(MAX_PROFILER_TARGETS);
            m_profiled.resize(MAX_PROFILER_TARGETS);
            m_timeStamps.resize(MAX_PROFILER_TARGETS);

            //create queries
            for (int x = 0; x < MAX_PROFILER_TARGETS; ++x)
            {
                HRESULT hr;
                D3D11_QUERY_DESC queryDesc;
                queryDesc.Query = D3D11_QUERY_TIMESTAMP;
                queryDesc.MiscFlags = 0;

                hr = m_device->CreateQuery(&queryDesc, &m_queryList[ x ][ 0 ]);
                UAssert(hr == S_OK, "Failed to create query device!");
                hr = m_device->CreateQuery(&queryDesc, &m_queryList[ x ][ 1 ]);
                UAssert(hr == S_OK, "Failed to create query device!");

                m_timeStamps[ x ] = -1;
            }
        }

        void GfxProfiler::Uninitialize()
        {
            if (!m_run)
                return;

            //release disjoint
            RELEASE_RESOURCE(m_disjointedQueries[ 0 ]);
            RELEASE_RESOURCE(m_disjointedQueries[ 1 ]);

            //release others
            for (auto x : m_queryList)
            {
                RELEASE_RESOURCE(x[ 0 ]);
                RELEASE_RESOURCE(x[ 1 ]);
            }

            m_device = nullptr;
            m_devCon = nullptr;
        }

        //start frame
        void GfxProfiler::BeginFrame()
        {
            if (!m_run)
                return;

            for ( auto &x : m_profiled )
                x = false;


            start++;
            m_devCon->Begin(m_disjointedQueries[ m_frame ]);

            m_numberOfTargets = 0;
            m_maxNameSize = 0;
            Stamp("Frame Begin");
        }

        //end frame
        void GfxProfiler::EndFrame()
        {
            if (!m_run)
                return;

            //stamp last


            Stamp("FRC");
            end++;
            m_devCon->End(m_disjointedQueries[ m_frame ]);

            //alternate frames
            if ( m_frame == 0 )
                m_frame = 1;
            else
                m_frame = 0;
        }

        //call "Time" on each segment
        void GfxProfiler::Stamp(std::string name)
        {
            if (!m_run)
                return;

            if(!(m_numberOfTargets + 1 <= MAX_PROFILER_TARGETS))
            {
                std::cout << start << ", " << end << std::endl;
                UWarning("Out of gpu profiler targets!");
                return;
            }

            m_targetNames[ m_numberOfTargets ] = name;
            m_profiled[ m_numberOfTargets ] = true;

            if ( name.length() > m_maxNameSize )
                m_maxNameSize = static_cast<int>(name.length());

            m_devCon->End(m_queryList[ m_numberOfTargets++ ][ m_frame ]);
        }

        void GfxProfiler::WaitForCalls(bool output)
        {
            if (!m_run)
                return;

            //make sure we have a previous frame to compare to
            if (m_lastFrame < 0)
            {
                m_lastFrame = 0;
                return;
            }

            //wait for data
            while (m_devCon->GetData(m_disjointedQueries[ m_lastFrame ], nullptr, 0, 0) == S_FALSE) { }

            //grab data
            D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointStamp;
            m_devCon->GetData(m_disjointedQueries[ m_lastFrame ], &disjointStamp, sizeof(disjointStamp), 0);

            //if data was disjointed during frame, skip
            if (disjointStamp.Disjoint)
                return;

            //grab the baseline for the whole frame
            UINT64 baseline;
            m_devCon->GetData(m_queryList[ 0 ][ m_lastFrame ], &baseline, sizeof(UINT64), 0);
            m_timeStamps[ 0 ] = 0;

            double totalTime = 0;

            //for each query, get the time w/ respect to the baseline
            for (int x = 1; x < m_numberOfTargets; ++x)
            {
                UINT64 current;

                //get the time
                m_devCon->GetData(m_queryList[ x ][ m_lastFrame ], &current, sizeof(UINT64), 0);

                //save data
                m_timeStamps[ x ] = (double)(current - baseline) / (double)disjointStamp.Frequency;

                //update
                baseline = current;

                totalTime += m_timeStamps[ x ];
            }

            // time to output data
            if ( output )
            {
                //get the total time of the frame, as well as the time that wasn't profiled
                printf("%f, %f\n", totalTime, totalTime);

                // calculate the values for each time
                for ( int x = 1; x < m_numberOfTargets - 1; ++x )
                {
                    // print name, time, and percent of time
                    if( m_timeStamps[ x ] >= 0.00001)
                        printf("%-*s, %10.5f, %2.0f%% of frame\n", static_cast<int>(m_maxNameSize + 1), m_targetNames[x].c_str(), m_timeStamps[x], (m_timeStamps[x] / totalTime) * 100.0f);
                }

                printf("TOTAL FRAME TIME: %f\n", totalTime);
            }

            //alternate the last frame
            if (m_lastFrame == 0)
                m_lastFrame = 1;
            else
                m_lastFrame = 0;

            m_lastBaseLine = baseline;

            for ( auto &x : m_profiled )
                x = false;
        }
    }
}