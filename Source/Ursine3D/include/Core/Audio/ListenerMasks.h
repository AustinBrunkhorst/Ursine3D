#pragma once

enum ListenerIndex
{
	LISTENER_NONE,
	LISTENER_ONE = 1 << 0u,
	LISTENER_TWO = 1 << 1u,
	LISTENER_THREE = 1 << 2u,
	LISTENER_FOUR = 1 << 3u,
	LISTENER_FIVE = 1 << 4u,
	LISTENER_SIX = 1 << 5u,
	LISTENER_SEVEN = 1 << 6u,
	LISTENER_EIGHT = 1 << 7u
} Meta(Enable);