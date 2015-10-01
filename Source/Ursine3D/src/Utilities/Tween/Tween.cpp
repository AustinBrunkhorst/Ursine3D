/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** Action.cpp
**
** Author:
** - Austin Brunkhorst - A.Brunkhorst@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "Tween.h"
#include "TweenManager.h"

namespace ursine
{
    Tween::Tween(TweenGroupID group)
        : m_group( group )
        , m_deleting( false )
        , m_paused( false )
        , m_removed( nullptr ) { }

    Tween::~Tween(void)
    {
        for (auto *item : m_items)
            delete item;
    }

    void Tween::Update(DeltaTime dt)
    {
        if (m_items.empty( ))
            return;

        auto current = m_items.front( );

        if (current->Update( dt ))
        {
            delete current;

            m_items.pop_front( );
        }
    }

    TweenID Tween::Create(TweenGroupID group)
    {
        static auto *manager = GetCoreSystem( TweenManager );

        return manager->create( group );
    }

    void Tween::add(TweenItem *item)
    {
        m_items.emplace_back( item );
    }

    void Tween::stop(void)
    {
        for (auto *item : m_items)
            delete item;

        m_items.clear( );
    }
}
