#pragma once

#include <NativeJSClass.h>

#include "NativeToolEvent.h"

class NativeEditorTool : public ursine::EventDispatcher<std::string>
{
public:
    NativeEditorTool(void);
    virtual ~NativeEditorTool(void);

    bool IsFocused(void) const;

protected:
    // determines if this tool currently has focused in the editor
    bool m_isFocused;

private:

    void onFocusChange(EVENT_HANDLER(NativeEditorTool));
} Meta(Register);