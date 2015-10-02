#include "Precompiled.h"

#include "NativeEditorTool.h"

NativeEditorTool::NativeEditorTool(void)
    : m_isFocused( false )
    , m_hasCursorFocus( false )
{
    Listener( this )
        .On( "focus", &NativeEditorTool::onFocusChange )
        .On( "blur", &NativeEditorTool::onFocusChange )
        .On( "mouseover", &NativeEditorTool::onMouseEntry )
        .On( "mouseout", &NativeEditorTool::onMouseEntry );
}

NativeEditorTool::~NativeEditorTool(void)
{
    Listener( this )
        .Off( "focus", &NativeEditorTool::onFocusChange )
        .Off( "blur", &NativeEditorTool::onFocusChange )
        .Off( "mouseover", &NativeEditorTool::onMouseEntry )
        .Off( "mouseout", &NativeEditorTool::onMouseEntry );
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

    m_isFocused = (args->name == "focus");
}

void NativeEditorTool::onMouseEntry(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    m_hasCursorFocus = (args->name == "mouseover");
}