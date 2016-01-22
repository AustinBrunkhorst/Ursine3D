/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** GridRendererSettingsComponent.h
**
** Author:
** - Joshua Shlemmer - joshua.shlemmer@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#pragma once

#include <Component.h>

#include <Array.h>

class GridRendererSettings : public ursine::ecs::Component
{
    NATIVE_COMPONENT;

public:
    EditorField(
        int CellSize,
        GetCellSize,
        SetCellSize
    );

    EditorField(
        int Width,
        GetWidth,
        SetWidth
    );

    EditorField(
        int Height,
        GetHeight,
        SetHeight
    );

    EditorField(
        int SubDivisions,
        GetSubDivisions,
        SetSubDivisions
    );

    ursine::Array<double> testing;

    GridRendererSettings(void);
    ~GridRendererSettings(void);

    Meta(Disable)
    void OnInitialize(void) override;

    int GetCellSize(void) const;
    void SetCellSize(int size);

    int GetWidth(void) const;
    void SetWidth(int width);

    int GetHeight(void) const;
    void SetHeight(int height);

    int GetSubDivisions(void) const;
    void SetSubDivisions(int subDivisions);

private:
    int m_cellSize;
    int m_width;
    int m_height;
    int m_subDivisions;
} Meta(Enable, DisableSerialization);