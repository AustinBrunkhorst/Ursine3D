#include "UrsinePrecompiled.h"
#include "GfxProfiler.h"

namespace rey_oso
{
  void GfxProfiler::Initialize( ID3D11Device *device, ID3D11DeviceContext *devCon, bool run )
  {
    m_device = device;
    m_devCon = devCon;

    m_run = run;

    if (!m_run)
      return;

    //init variables
    m_frame = 0;
    m_lastFrame = -1;

    //create disjointed queries
    HRESULT hr;
    D3D11_QUERY_DESC queryDesc;
    queryDesc.Query = D3D11_QUERY_TIMESTAMP_DISJOINT;
    queryDesc.MiscFlags = 0;

    hr = m_device->CreateQuery( &queryDesc, &m_disjointedQueries[ 0 ] );
    UAssert( hr == S_OK, "Failed to create query device!" );
    hr = m_device->CreateQuery( &queryDesc, &m_disjointedQueries[ 1 ] );
    UAssert( hr == S_OK, "Failed to create query device!" );


    //create queries
    for (int x = 0; x < PROFILE_COUNT; ++x)
    {
      HRESULT hr;
      D3D11_QUERY_DESC queryDesc;
      queryDesc.Query = D3D11_QUERY_TIMESTAMP;
      queryDesc.MiscFlags = 0;

      hr = m_device->CreateQuery( &queryDesc, &m_queryList[ x ][ 0 ] );
      UAssert( hr == S_OK, "Failed to create query device!" );
      hr = m_device->CreateQuery( &queryDesc, &m_queryList[ x ][ 1 ] );
      UAssert( hr == S_OK, "Failed to create query device!" );
    }
  }
  void GfxProfiler::Uninitialize( )
  {
    if (!m_run)
      return;

    //release disjoint
    RELEASE_RESOURCE( m_disjointedQueries[ 0 ] );
    RELEASE_RESOURCE( m_disjointedQueries[ 1 ] );

    //release others
    for (auto x : m_queryList)
    {
      RELEASE_RESOURCE( x[ 0 ] );
      RELEASE_RESOURCE( x[ 1 ] );
    }
  }

  //start frame
  void GfxProfiler::BeginFrame( )
  {
    if (!m_run)
      return;

    m_devCon->Begin( m_disjointedQueries[ m_frame ] );
    Stamp( PROFILE_FRAME_BEGIN );
  }

  //end frame
  void GfxProfiler::EndFrame( )
  {
    if (!m_run)
      return;

    //stamp last
    //Stamp( PROFILE_FRAME_END );
    m_devCon->End( m_disjointedQueries[ m_frame ] );

    //alternate frames
    if (m_frame == 0)
      m_frame = 1;
    else
      m_frame = 0;
  }

  //call "Time" on each segment
  void GfxProfiler::Stamp( ProfilingTargets target)
  {
    if (!m_run)
      return;

    m_devCon->End( m_queryList[ target ][m_frame] );
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
    while (m_devCon->GetData( m_disjointedQueries[ m_lastFrame ], NULL, 0, 0 ) == S_FALSE)
    {

    }

    //grab data
    D3D11_QUERY_DATA_TIMESTAMP_DISJOINT disjointStamp;
    m_devCon->GetData( m_disjointedQueries[ m_lastFrame ], &disjointStamp, sizeof( disjointStamp ), 0 );

    //if data was disjointed during frame, skip
    if (disjointStamp.Disjoint)
      return;

    //grab the baseline for the whole frame
    UINT64 baseline;
    m_devCon->GetData( m_queryList[ PROFILE_FRAME_BEGIN ][ m_lastFrame ], &baseline, sizeof( UINT64 ), 0 );

    static float total = 0;
    static int frameCount = 1;

    float currentTotal = 0;

    //for each query, get the time w/ respect to the baseline
    for (int x = 1; x < PROFILE_COUNT; ++x)
    {
      UINT64 current;

      //get the time
      m_devCon->GetData( m_queryList[x][ m_lastFrame ], &current, sizeof( UINT64 ), 0 );

      //save data
      m_timeStamps[ x ] = (float)(current - baseline) / (float)disjointStamp.Frequency;

      //update
      baseline = current;

      total += m_timeStamps[ x ];
      currentTotal += m_timeStamps[ x ];

      if(output)
        printf( "PROFILE: %i, %5.3f\n", x, m_timeStamps[ x ] / 0.016f );
    }

    if (output)
      printf( "TOTAL: %5.3f, FPS: %5.3f\n\n", currentTotal, 1.f / (total / frameCount++) );

    //alternate the last frame
    if (m_lastFrame == 0)
      m_lastFrame = 1;
    else
      m_lastFrame = 0;
  }
}