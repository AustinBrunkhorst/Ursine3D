#include "Precompiled.h"

#include "NativeEditorTool.h"

using namespace native_tool;

NativeEditorTool::NativeEditorTool(void)
    : m_isFocused( false )
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