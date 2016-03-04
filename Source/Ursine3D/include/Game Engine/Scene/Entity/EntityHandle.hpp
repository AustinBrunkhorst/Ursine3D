#include <boost/functional/hash.hpp>

namespace std
{
    template<>
    struct hash<ursine::ecs::EntityHandle>
    {
        size_t operator()(const ursine::ecs::EntityHandle &handle) const
        {
            size_t seed = 0;

            boost::hash_combine( seed, handle.m_id );
            boost::hash_combine( seed, handle.m_version );

            return seed;
        }
    };
}