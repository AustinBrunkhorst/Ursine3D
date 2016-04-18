/* ----------------------------------------------------------------------------
** Team Bear King
** © 2016 DigiPen Institute of Technology, All Rights Reserved.
**
** EditorWindow.h
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

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