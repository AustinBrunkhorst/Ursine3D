/*******************************************
* File: Pool.h
* Author: Jordan Ellis
* Date: 11/05/2014
*
* Description:
*   Contiguous pool of objects, used for
* improved data locality. This in turn 
* boosts cache performance.
*
* Copyright: All content © 2013-2014 DigiPen
* (USA) Corporation, all rights reserved.
********************************************/

#pragma once

#include "Poolable.h"

#include <vector>
#include <stack>
#include <typeinfo>

namespace Ursine
{
    typedef unsigned Index;

    // Owner == class that is aggregating the object.
    // Object == class that is aggregated and will be pooled.
    template<class Owner, typename Object>
    class Pool
    {
    public:

        // One handle represents one index into the pool of objects.
        // Copying of handles results in a reference count increment.
        class Handle
        {
        public:

            Handle(const Handle &other);
            Handle(const Handle &&other);
            ~Handle(void);

            // Variadic constructor, allowing for
            // specialized construction of the pooled object.
            template<typename... Args>
            Handle(const Args&&...);
            
            Handle &operator=(const Handle &rhs);

            // Value access
            Object &operator*(void);
            const Object &operator*(void) const;

            // Method access
            Object *operator->(void);
            const Object *operator->(void) const;
            
            // Operators
            bool operator==(Handle &rhs) const { return _index == rhs._index; }
            bool operator!=(Handle &rhs) const { return _index != rhs._index; }

        private:

            // The index into the pool.
            Index _index;

            // Remove this handle's reference
            void removeHandle(void);
        };

        Pool(void);

    private:
        // The contiguous pool of objects.
        static std::vector<Object> &pool(void);

        // Stack of indices into the pool that aren't being used.
        static std::stack<Index, std::vector<Index>> &freeNodes(void);

        // Return a node from the pool for use.
        template<typename... Args>
        static Index create(Args&&...);
    };
}

#include "Pool.hpp"

