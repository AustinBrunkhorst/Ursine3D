#pragma once

#include <Window.h>
#include <UIView.h>

class EditorWindow
{
public:
    EditorWindow(void);

    ursine::Window::Handle GetWindow(void) const;
    ursine::UIView::Handle GetUI(void) const;

private:
    friend class Editor;

    ursine::Window::Handle m_window;
    ursine::UIView::Handle m_ui;
};