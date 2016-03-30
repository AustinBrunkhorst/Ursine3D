/* ----------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** NativeEditorTool.cpp
**
** Author:
** - Austin Brunkhorst - a.brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "NativeEditorTool.h"

using namespace native_tool;

NativeEditorTool::NativeEditorTool(void)
    : EventDispatcher( this )
    , m_isFocused( false )
    , m_hasCursorFocus( false )
{
    Listener( this )
        .On( event::Focus, &NativeEditorTool::onFocusChange )
        .On( event::Blur, &NativeEditorTool::onFocusChange )
        .On( event::MouseOver, &NativeEditorTool::onMouseEntry )
        .On( event::MouseOut, &NativeEditorTool::onMouseEntry );
}

NativeEditorTool::~NativeEditorTool(void)
{
    Listener( this )
        .Off( event::Focus, &NativeEditorTool::onFocusChange )
        .Off( event::Blur, &NativeEditorTool::onFocusChange )
        .Off( event::MouseOver, &NativeEditorTool::onMouseEntry )
        .Off( event::MouseOut, &NativeEditorTool::onMouseEntry );
}

bool NativeEditorTool::IsFocused(void) const
{
    return m_isFocused;
}

bool NativeEditorTool::HasCursorFocus(void) const
{
    return m_hasCursorFocus;
}

void NativeEditorTool::onFocusChange(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    m_isFocused = (args->name == event::Focus);
}

void NativeEditorTool::onMouseEntry(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    m_hasCursorFocus = (args->name == event::MouseOver);
}