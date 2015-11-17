#pragma once

#include <NativeJSClass.h>

#include "NativeToolEvent.h"

class NativeEditorTool : public ursine::EventDispatcher<std::string>
{
public:
    NativeEditorTool(void);
    virtual ~NativeEditorTool(void);

    bool IsFocused(void) const;
    bool HasCursorFocus(void) const;

protected:
    // determines if this tool currently has focused in the editor
    bool m_isFocused;
    bool m_hasCursorFocus;

private:
    void onFocusChange(EVENT_HANDLER(NativeEditorTool));
    void onMouseEntry(EVENT_HANDLER(NativeEditorTool));
} Meta(Register);