#pragma once

namespace ursine
{
    enum class ListenerIndex
    {
        Meta(Disable)
        None,
        One = 1 << 0u,
        Two = 1 << 1u,
        Three = 1 << 2u,
        Four = 1 << 3u,
        Five = 1 << 4u,
        Six = 1 << 5u,
        Seven = 1 << 6u,
        Eight = 1 << 7u
    } Meta(Enable);
}
