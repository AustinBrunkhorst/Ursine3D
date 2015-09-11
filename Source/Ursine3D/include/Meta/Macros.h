#pragma once

#ifdef __REFLECTION_PARSER__

#define Meta(...) __attribute__((annotate(#__VA_ARGS__)))

#define META_OBJECT

#else

#define Meta(...)

#define META_OBJECT                                                                                    \
    public:                                                                                            \
    ursine::meta::Type GetType(void) const override                                                    \
    {                                                                                                  \
        return typeof( decltype( *this ) );                                                            \
    }                                                                                                  \
    ursine::meta::Object *Clone(void) const override                                                   \
    {                                                                                                  \
        typedef std::remove_const< std::remove_reference< decltype( *this ) >::type >::type ClassType; \
        return new ClassType( *this );                                                                 \
    }                                                                                                  \
    private:                                                                                           \

#endif