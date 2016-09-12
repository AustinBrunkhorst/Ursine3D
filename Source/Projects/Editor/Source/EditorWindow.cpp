/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorWindow.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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