#pragma once

#include <CoreSystem.h>

#include <Window.h>

#include "Tools/EditorTool.h"

class Editor : public ursine::core::CoreSystem
{
    CORE_SYSTEM
public:
    Editor(void);
    ~Editor(void);

    void OnInitialize(void) override;
    
private:
    ursine::Window *m_mainWindow;

    std::vector<EditorTool *> m_tools;

    void initializeTools(void);
} Meta(Enable);