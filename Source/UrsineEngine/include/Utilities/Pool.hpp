/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Pool.hpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

namespace ursine
{
    template<class Owner, typename Object>
    Pool<Owner, Object>::Handle::Handle(const Handle &other)
        : _index(other._index)
    {
        // Increment the object's reference count.
        pool()[_index].AddRef();
    }
    
    template<class Owner, typename Object>
    Pool<Owner, Object>::Handle::Handle(const Handle &&other)
        : _index(std::move(other._index))
    {
        // Increment the object's reference count.
        pool()[_index].AddRef();
    }

    template<class Owner, typename Object>
    Pool<Owner, Object>::Handle::~Handle(void)
    {
        removeHandle();
    }

    template<class Owner, typename Object>
    template<typename... Args>
    Pool<Owner, Object>::Handle::Handle(const Args&&... args)
    {
        // Construct the object, and store it's index.
        _index = create(std::forward<Args>(args)...);
    }

    template<class Owner, typename Object>
    typename Pool<Owner, Object>::Handle
    &Pool<Owner, Object>::Handle::operator=(const Handle &rhs)
    {
        // If these handles differ
        if (_index != rhs._index)
        {
            // Remove self.
            removeHandle();

            // Copy other.
            _index = rhs._index;

            pool()[_index].AddRef();
        }

        return *this;
    }

    template<class Owner, typename Object>
    Object &Pool<Owner, Object>::Handle::operator*(void)
    {
        return pool()[_index];
    }

    template<class Owner, typename Object>
    const Object &Pool<Owner, Object>::Handle::operator*(void) const
    {
        return pool()[_index];
    }

    template<class Owner, typename Object>
    Object *Pool<Owner, Object>::Handle::operator->(void)
    {
        return &pool()[_index];
    }

    template<class Owner, typename Object>
    const Object *Pool<Owner, Object>::Handle::operator->(void) const
    {
        return &pool()[_index];
    }

    template<class Owner, typename Object>
    void Pool<Owner, Object>::Handle::removeHandle(void)
    {
        auto &node = pool()[_index];

        // Decrement the node's reference count.
        node.RemoveRef();

        // Release the node if no more references exist.
        if (node.RefCount() == 0)
            // Add the relased node to the free stack.
            freeNodes().push(_index);
    }

    template<class Owner, typename Object>
    Pool<Owner, Object>::Pool(void)
    {
        // Cannot construct of a pool of a non-poolable object.
        static_assert(std::is_base_of<Poolable, Object>::value,
            "Type is not derived from Poolable.");
    }

    template<class Owner, typename Object>
    std::vector<Object> &Pool<Owner, Object>::pool(void)
    {
        // This method is required for translation unit related issues.
        static std::vector<Object> pool;

        return pool;
    }

    template<class Owner, typename Object>
    std::stack<Index, std::vector<Index>> &Pool<Owner, Object>::freeNodes(void)
    {
        // This method is required for translation unit related issues.
        static std::stack<Index, std::vector<Index>> free_nodes;

        return free_nodes;
    }

    template <class Owner, typename Object>
    template <typename... Args>
    Index Pool<Owner, Object>::create(Args&&... args)
    {
        if (freeNodes().empty())
        {
            // Push a new item onto the vector.
            pool().emplace_back(Object(std::forward<Args>(args)...));

            // Return the index of the newly created object
            return pool().size() - 1;
        }

        // There are free nodes,
        // pop one off the stack.
        Index index = freeNodes().top();
        freeNodes().pop();

        // Reset that node's value to default.
        pool()[index].Reset(std::forward<Args>(args)...);

        return index;
    }
}