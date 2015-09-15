#pragma once

#include <d3d11.h>
#include <string>
#include <unordered_map>

#include "ProfileTargetList.h"
#include "GraphicsDefines.h"

namespace rey_oso
{
  class GfxProfiler
  {
  public:
    void Initialize( ID3D11Device *device, ID3D11DeviceContext *devCon, bool run = false );
    void Uninitialize( );

    //start frame
    void BeginFrame( );

    //end frame
    void EndFrame( );

    //call "Time" on each segment
    void Stamp( ProfilingTargets target );

    //wait for calls to complete before flipping swap chain
    void WaitForCalls( bool output = false );
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

    //disjointed queries
    ID3D11Query *m_disjointedQueries[ 2 ];

    //data queries
    ID3D11Query * m_queryList[PROFILE_COUNT][2];

    //the data
    float m_timeStamps[PROFILE_COUNT];
  };
}