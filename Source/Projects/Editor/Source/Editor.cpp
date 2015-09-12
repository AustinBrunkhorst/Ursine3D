#include "Precompiled.h"

#include "Editor.h"

#include <Application.h>
#include <WindowManager.h>

CORE_SYSTEM_DEFINITION( Editor );

Editor::Editor(void)
    : m_mainWindow( nullptr )
{

}

Editor::~Editor(void)
{

}

void Editor::OnInitialize(void)
{
    auto *windowManager = 
        ursine::Application::Instance->GetCoreSystem<ursine::WindowManager>( );

    m_mainWindow = windowManager->AddWindow(
        "Ursine3D Editor", 
        { 0, 0 }, 
        { 1280, 720 }, 
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    m_mainWindow->SetLocationCentered( );
    m_mainWindow->Show( true );
}

void Editor::initializeTools(void)
{
    // @@@ TODO:
}