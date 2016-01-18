#pragma once

#include <string>
#include <functional>

namespace notification
{
    struct Button
    {
        // text to be displayed in the button
        std::string text;

        // callback when the button is clicked
        std::function<void(void)> onClick;
    };
}