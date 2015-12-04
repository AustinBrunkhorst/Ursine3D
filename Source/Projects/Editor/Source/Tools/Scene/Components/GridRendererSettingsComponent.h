#pragma once

#include <Component.h>

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