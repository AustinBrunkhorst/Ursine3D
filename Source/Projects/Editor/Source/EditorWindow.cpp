#include "Precompiled.h"

#include "EditorWindow.h"

EditorWindow::EditorWindow(void)
    : m_window( nullptr )
    , m_ui( nullptr ) { }

ursine::Window::Handle EditorWindow::GetWindow(void) const
{
    return m_window;
}

ursine::UIView::Handle EditorWindow::GetUI(void) const
{
    return m_ui;
}