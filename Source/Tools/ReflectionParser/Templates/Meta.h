#pragma once

#ifdef REFLECTION_PARSER
#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))
#else
#define Meta(...)
#endif