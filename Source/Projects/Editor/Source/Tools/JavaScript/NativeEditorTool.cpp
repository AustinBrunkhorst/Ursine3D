#include "Precompiled.h"

#include "NativeEditorTool.h"

NativeEditorTool::NativeEditorTool(void)
    : m_isFocused( false )
{
    Listener( this )
        .On( "focus", &NativeEditorTool::onFocusChange )
        .On( "blur", &NativeEditorTool::onFocusChange );
}

NativeEditorTool::~NativeEditorTool(void)
{
    Listener( this )
        .Off( "focus", &NativeEditorTool::onFocusChange )
        .Off( "blur", &NativeEditorTool::onFocusChange );
}

bool NativeEditorTool::IsFocused(void) const
{
    return m_isFocused;
}

void NativeEditorTool::onFocusChange(EVENT_HANDLER(NativeEditorTool))
{
    EVENT_ATTRS(NativeEditorTool, NativeToolEvent);

    m_isFocused = (args->name == "focus");
}