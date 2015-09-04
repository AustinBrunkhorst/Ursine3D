#pragma once

#ifdef __REFLECTION_PARSER__
#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define Meta(...)
#endif