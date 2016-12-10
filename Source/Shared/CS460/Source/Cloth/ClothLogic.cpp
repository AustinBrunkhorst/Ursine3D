/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** ClothLogic.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "ClothLogic.h"
#include "ClothNode.h"

using namespace ursine;

#define AT(x, y) (y * m_width + x)

ClothLogic::ClothLogic(const Vec2 & dimensions, float segmentLength)
    : m_width( 0 )
    , m_height( 0 )
    , m_constraintLength( 0.0f )
    , m_stiffness( 200.0f )
    , m_damping( 0.2f )
    , m_nodeMass( 0.2f )
    , m_nodes( nullptr ) { }

void ClothLogic::Update(float dt)
{
    static const uint numIterations = 4;
    static const float rkAvg = 1.0f / 6.0f;
    static const float damping = 0.9999f;
    static const SVec3 gravity = SVec3::UnitY( ) * -9.8f;

    auto numNodes = m_width * m_height;

    // acceleration and velocity approximations
    m_accelerations.resize( numIterations * numNodes );
    m_velocities.resize( (numIterations + 1) * numNodes );
    m_positions.resize( numNodes );

    // store initial positions and velocities for approximation 0
    for (uint i = 0; i < numNodes; ++i)
    {
        auto *node = m_nodes + i;

        m_velocities[ i ] = node->velocity;
        m_positions[ i ] = node->position;
    }

    float dts[ numIterations ] = { dt, dt / 2.0f, dt / 2.0f, dt };

    for (uint i = 0; i < numIterations; ++i)
    {
        // apply all spring forces
        for (uint x = 0; x < m_width; ++x)
        {
            for (uint y = 0; y < m_height; ++y)
            {
                auto curr = GetNode( x, y );
                auto left = GetNode( x - 1, y );
                auto up = GetNode( x, y - 1 );

                if (left)
                    springUpdate( left, curr );

                if (up)
                    springUpdate( up, curr );
            }
        }

        for (uint x = 0; x < m_width; ++x)
        {
            for (uint y = 0; y < m_height; ++y)
            {
                if (x == 0 && y == 0)
                    continue;
                if (x + 1 == m_width && y == 0)
                    continue;

                auto curr = GetNode( x, y );

                // current acceleration approximation + gravity
                auto accelIndex = i * numNodes + AT(x, y);
                auto velIndex = (i + 1) * numNodes + AT(x, y);
                auto prevVelIndex = accelIndex;

                m_accelerations[ accelIndex ] = curr->force / m_nodeMass + gravity;
                m_velocities[ velIndex ] = curr->velocity + m_accelerations[ accelIndex ] * dts[ i ];

                curr->position = m_positions[ AT(x, y) ] + m_velocities[ prevVelIndex ] * dts[ i ];

                curr->force = SVec3::Zero( );
            }
        }
    }

    // compute weighted average of all approximations
    for (uint x = 0; x < m_width; ++x)
    {
        for (uint y = 0; y < m_height; ++y)
        {
            if (x == 0 && y == 0)
                continue;
            if (x + 1 == m_width && y == 0)
                continue;

            auto curr = GetNode( x, y );

            // v = v0 + 1/6 * (a0 + a3 + 2 * (a1 + a2)) * dt
            auto a0 = AT(x, y),
                 a1 = numNodes + AT(x, y),
                 a2 = 2 * numNodes + AT(x, y),
                 a3 = 3 * numNodes + AT(x, y);

            curr->velocity += rkAvg * (
                m_accelerations[ a0 ] + m_accelerations[ a3 ] + 2.0f * (
                    m_accelerations[ a1 ] + m_accelerations[ a2 ]
                )
            ) * dt;

            // x = x0 + 1 / 6 * damping * (v1 + v4 + 2 * (v2 + v3)) * dt
            auto v1 = a1,
                 v2 = a2,
                 v3 = a3,
                 v4 = 4 * numNodes + AT(x, y);

            curr->position = m_positions[ AT(x, y) ] + rkAvg * damping * ( 
                m_velocities[ v1 ] + m_velocities[ v4 ] + 2.0f * ( 
                    m_velocities[ v2 ] + m_velocities[ v3 ] 
                ) 
            ) * dt;
        }
    }
}

void ClothLogic::UpdateDimensions(const Vec2 &dimensions, float segmentLength)
{
    UAssert(!math::IsZero( segmentLength ), "Cloth segment length can never be zero");

    // calculate out new values
    m_width  = static_cast<uint>( ceilf( dimensions.X( ) ) );
    m_height = static_cast<uint>( ceilf( dimensions.Y( ) ) );

    m_constraintLength = segmentLength;

    delete[] m_nodes;
    m_nodes = m_width == 0 && m_height == 0 ? nullptr : new ClothNode[m_width * m_height];

    for (uint x = 0; x < m_width; ++x)
    {
        for (uint y = 0; y < m_height; ++y)
        {
            initializeNode(x, y);
        }
    }
}

uint ClothLogic::GetWidth(void) const
{
    return m_width;
}

uint ClothLogic::GetHeight(void) const
{
    return m_height;
}

ClothNode* ClothLogic::GetNode(uint x, uint y) const
{
    if (x >= m_width || y >= m_height)
        return nullptr;

    return m_nodes + AT(x, y);
}

void ClothLogic::initializeNode(uint x, uint y) const
{
    auto &node = m_nodes[ AT(x, y) ];

    node.position = SVec3( x * m_constraintLength, y * -m_constraintLength, 0.0f );
}

void ClothLogic::springUpdate(ClothNode *a, ClothNode *b)
{
    SVec3 relVel = a->velocity - b->velocity;
    SVec3 relPos = a->position - b->position;
    float len = relPos.Length( );

    relPos.Normalize( );

    float mag = (len - m_constraintLength) * m_stiffness + relVel.Dot( relPos ) * m_damping;

    relPos *= mag;

    a->force -= relPos;
    b->force += relPos;
}
