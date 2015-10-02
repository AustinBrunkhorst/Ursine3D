#include "Precompiled.h"

#include "GridRenderer.h"

#include <SystemManager.h>
#include <GfxAPI.h>

using namespace ursine;

ENTITY_SYSTEM_DEFINITION( GridRenderer ) ;

GridRenderer::GridRenderer(ecs::World *world)
    : EntitySystem( world )
      , m_graphics( nullptr )
      , m_renderSystem( nullptr )
{
}

void GridRenderer::OnInitialize(void)
{
    m_graphics = GetCoreSystem( GfxAPI );

    m_renderSystem = m_world->GetEntitySystem( ecs::RenderSystem );

    m_renderSystem->Listener( this )
        .On( ecs::RENDER_HOOK, &GridRenderer::onRenderHook );
}

void GridRenderer::OnRemove(void)
{
    m_renderSystem->Listener( this )
        .Off( ecs::RENDER_HOOK, &GridRenderer::onRenderHook );
}

void GridRenderer::onRenderHook(EVENT_HANDLER(ecs::RenderSystem))
{
    EVENT_ATTRS(ecs::RenderSystem, ecs::RenderHookArgs);

    auto &camera = m_graphics->CameraMgr.GetCamera( args->camera );

    auto position = camera.GetPosition( );

    ///////////////////////////////////////////////////////////////////
    // VARIABLES
    //rendering a grid
    float gridColor[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float xColor[4] = { 0.58f, 0.07f, 0.07f, 1.0f };
    float zColor[4] = { 0.0f, 0.12f, 0.75f, 1.0f };
    float subColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

    //size of a cell
    float cellSize = 1;

    // # of cells
    int widthCount = 200;
    int heightCount = 200;

    //number of cells between dividers
    static int subSector = 10;

    //current center position of the grid
    float centerX = position.X( );
    float centerZ = position.Z( );

    ///////////////////////////////////////////////////////////////////
    // IMPLEMENTATION
    //make sure it is off
    if (widthCount % 2 == 0)
        widthCount++;
    if (heightCount % 2 == 0)
        heightCount++;

    float scalar = (1.f / cellSize);
    if (scalar < 1) scalar = 1;

    //round to the right size
    float posX = round( centerX * (1.f / cellSize) ) / (1.f / cellSize);
    float posZ = round( centerZ * (1.f / cellSize) ) / (1.f / cellSize);

    //to get to the side, get the middle
    posX -= ( int )(widthCount / 2) * cellSize;

    //render w/ X changing
    for (int x = 0; x < widthCount; ++x)
    {
        if (round( posX * scalar ) / scalar == 0)
            m_graphics->DrawingMgr.SetColor( zColor[ 0 ], zColor[ 1 ], zColor[ 2 ], zColor[ 3 ] );
        else if (( int )(posX * ( int )scalar) % subSector == 0)
            m_graphics->DrawingMgr.SetColor( subColor[ 0 ], subColor[ 1 ], subColor[ 2 ], subColor[ 3 ] );
        else
            m_graphics->DrawingMgr.SetColor( gridColor[ 0 ], gridColor[ 1 ], gridColor[ 2 ], gridColor[ 3 ] );

        m_graphics->DrawingMgr.DrawLine( posX, 0, posZ + heightCount / 2 * cellSize, posX, 0, posZ - heightCount / 2 * cellSize );
        posX += cellSize;
    }

    posX = round( centerX * (1.f / cellSize) ) / (1.f / cellSize);
    posZ -= ( int )(heightCount / 2) * cellSize;

    //render w/ z changing
    for (int x = 0; x < heightCount; ++x)
    {
        if (round( posZ * scalar ) / scalar == 0)
            m_graphics->DrawingMgr.SetColor( xColor[ 0 ], xColor[ 1 ], xColor[ 2 ], xColor[ 3 ] );
        else if (( int )(posZ * ( int )scalar) % subSector == 0)
            m_graphics->DrawingMgr.SetColor( subColor[ 0 ], subColor[ 1 ], subColor[ 2 ], subColor[ 3 ] );
        else
            m_graphics->DrawingMgr.SetColor( gridColor[ 0 ], gridColor[ 1 ], gridColor[ 2 ], gridColor[ 3 ] );

        m_graphics->DrawingMgr.DrawLine( posX + (widthCount / 2) * cellSize, 0, posZ, posX - (widthCount / 2) * cellSize, 0, posZ );
        posZ += cellSize;
    }
}
