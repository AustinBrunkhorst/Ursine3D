/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GridRendererSettingsComponent.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "GridRendererSettingsComponent.h"

NATIVE_COMPONENT_DEFINITION( GridRendererSettings );

GridRendererSettings::GridRendererSettings(void)
    : BaseComponent( ) 
    , m_cellSize( 1 )
    , m_width( 200 )
    , m_height( 200 )
    , m_subDivisions( 10 )
{ 

}

GridRendererSettings::~GridRendererSettings(void) { }

void GridRendererSettings::OnInitialize(void)
{
}

int GridRendererSettings::GetCellSize(void) const 
{
    return m_cellSize;
}

void GridRendererSettings::SetCellSize(int cellSize) 
{
    m_cellSize = std::max( 0, cellSize );

    NOTIFY_COMPONENT_CHANGED( "CellSize", m_cellSize );
}

int GridRendererSettings::GetWidth(void) const 
{
    return m_width;
}

void GridRendererSettings::SetWidth(int width) 
{
    m_width = std::max( 0, width );

    NOTIFY_COMPONENT_CHANGED( "Width", m_width );
}

int GridRendererSettings::GetHeight(void) const 
{
    return m_height;
}

void GridRendererSettings::SetHeight(int height) 
{
    m_height = std::max( 0, height );

    NOTIFY_COMPONENT_CHANGED( "Height", m_height );
}

int GridRendererSettings::GetSubDivisions(void) const 
{
    return m_subDivisions;
}

void GridRendererSettings::SetSubDivisions(int subDivisions) 
{
    m_subDivisions = std::max( 0, subDivisions );

    NOTIFY_COMPONENT_CHANGED( "SubDivisions", m_subDivisions );
}