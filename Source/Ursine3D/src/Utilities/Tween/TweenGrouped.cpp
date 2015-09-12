#include "UrsinePrecompiled.h"

#include "TweenGrouped.h"

#include "TweenDelay.h"
#include "TweenCall.h"

namespace ursine
{
    TweenGrouped::TweenGrouped(TweenManager *manager, uint32 owner)
        : m_owner( owner )
        , m_manager( manager ) { }

    TweenGrouped::~TweenGrouped(void)
    {
        for (auto *item : m_items)
            delete item;
    }

    bool TweenGrouped::Update(DeltaTime dt)
    {
        auto it = m_items.begin( );

        while (it != m_items.end( ))
        {
            auto item = *it;

            if (item->Update( dt ))
            {
                delete item;

                it = m_items.erase( it );
            }
            else
            {
                ++it;
            }
        }

        return m_items.empty( );
    }

    TweenGrouped &TweenGrouped::Delay(const TimeSpan &duration)
    {
        m_items.push_back( new TweenDelay( duration ) );

        return *this;
    }

    TweenGrouped &TweenGrouped::Call(TweenCallback callback)
    {
        m_items.emplace_back( new TweenCall( callback ) );

        return *this;
    }

    TweenID TweenGrouped::EndGroup(void) const
    {
        return TweenID( m_manager, m_owner );
    }

    void TweenGrouped::doProperty(const TimeSpan &duration,
        const TweenPercentageCallback &propertyFN)
    {
        m_items.emplace_back( new TweenProperty( duration, propertyFN ) );
    }
}
