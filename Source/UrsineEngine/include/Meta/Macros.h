#pragma once

#ifdef __REFLECTION_PARSER__

#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))

#define META_OBJECT

#else

#define Meta(...)

#define META_OBJECT                                        \
    public:                                                \
    inline ursine::meta::Type GetType(void) const override \
    {                                                      \
        return typeof( decltype( *this ) );                \
    }                                                      \
    private:                                               \

#endif