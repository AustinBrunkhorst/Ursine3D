#pragma once

#define UI_EVENT(name, definition)                 \
    struct name {                                  \
        static constexpr auto DefaultName = #name; \
        ##definition##                             \
    } Meta(Enable)                                 \

namespace ui_event
{
    UI_EVENT( ToggleHUD,
        bool toggled;
    );
}