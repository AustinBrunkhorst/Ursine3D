/* ----------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** IKLeafComponent.cpp
**
** Author:
** - Jordan Ellis - j.ellis@digipen.edu
**
** Contributors:
** - <list in same format as author if applicable>
** --------------------------------------------------------------------------*/

#include "Precompiled.h"

#include "IKLeafComponent.h"
#include "IKRootComponent.h"

using namespace ursine;
using namespace ecs;

NATIVE_COMPONENT_DEFINITION( IKLeaf );

IKLeaf::IKLeaf(void)
    : BaseComponent( )
    , m_errorMargin( 0.5f ) { }

const std::string &IKLeaf::GetGoalEntity(void) const
{
    return m_goalEntityName;
}

void IKLeaf::SetGoalEntity(const std::string &entityName)
{
    m_goalEntityName = entityName;

    NOTIFY_COMPONENT_CHANGED( "goalEntity", m_goalEntityName );
}

float IKLeaf::GetGoalErrorMargin(void) const
{
    return m_errorMargin;
}

void IKLeaf::SetGoalErrorMargin(float margin)
{
    m_errorMargin = margin;

    NOTIFY_COMPONENT_CHANGED( "goalErrorMargin", m_errorMargin );
}

void IKLeaf::Initialize(IKRoot *root)
{
    // Create joints from the root to this leaf
    auto trans = GetOwner( )->GetTransform( );
    m_joints.clear( );

    int chainLength = 1;

    while (trans != root->GetOwner( )->GetTransform( ))
    {
        ++chainLength;
        trans = trans->GetParent( ).Get( );
    }

    m_joints.resize( chainLength );

    trans = GetOwner( )->GetTransform( );

    for (int i = chainLength - 1; i > 0; --i)
    {
        m_joints[ i ] = IKJoint( trans );
        trans = trans->GetParent( ).Get( );
    }

    // Add the root
    m_joints[ 0 ] = IKJoint( trans );

    m_distanceFromRoot.resize( m_joints.size( ) );

    m_distanceFromRoot[ 0 ] = 0.0f;

    // Store the total length of chain
    for (size_t i = 1, n = m_joints.size( ); i < n; ++i)
    {
        auto distance = SVec3::Distance(
            m_joints[i - 1].GetWorldPosition( ),
            m_joints[i].GetWorldPosition( )
        );

        m_distanceFromRoot[i] = m_distanceFromRoot[i - 1] + distance;
    }

    trans = GetOwner( )->GetTransform( );

    m_startPosition = trans->GetRoot( )->GetWorldToLocalMatrix( ).TransformPoint( 
        trans->GetWorldPosition( )
    );

    m_goalEntity = GetOwner( )->GetWorld( )->GetEntityFromName( m_goalEntityName );
}

void IKLeaf::IKUpdate(float t)
{
    t = math::Clamp( t, 0.0f, 1.0f );

    if (m_goalEntity == nullptr || m_joints.size( ) <= 1 || t == 0.0f)
            return;

    auto goalPos = m_goalEntity->GetTransform( )->GetWorldPosition( );

    // assign the joint's orientation before solving
    for (auto &joint : m_joints)
    {
        joint.UpdateOrientation( goalPos );
    }

    // Get the distance from the leaf joint to the goal
    auto trans = GetOwner( )->GetTransform( );
    auto startPos = trans->GetRoot( )->GetLocalToWorldMatrix( ).TransformPoint( m_startPosition );
    auto leafToGoal = SVec3::Distance( startPos, goalPos );
    auto goalVec = SVec3::Normalize( goalPos - startPos );
    auto solveForPos = startPos + goalVec * leafToGoal * t;
    auto leafToSolve = SVec3::Distance( trans->GetWorldPosition( ), solveForPos );

    // If we're in range, early out
    if (leafToSolve <= m_errorMargin)
        return;

    // Get the distance from the root to the goal
    auto rootToGoal = SVec3::Distance(
        m_joints[ 0 ].GetWorldPosition( ), solveForPos
    );

    // If it is greater than the total distance, fully extend the chain
    if (rootToGoal >= getChainLength( ))
        fullyExtendChain( solveForPos );
    // Else, perform FABRIK
    else
        FABRIK( solveForPos );

    // Correct the joint's orientations
    for (auto joint : m_joints)
    {
        joint.CorrectOrientation( goalPos );
    }
}

float IKLeaf::getChainLength(void) const
{
    return m_distanceFromRoot[ m_distanceFromRoot.size( ) - 1 ];
}

void IKLeaf::fullyExtendChain(const SVec3 &solveForPosition)
{
    auto rootPos = m_joints[ 0 ].GetWorldPosition( );
    auto direction = solveForPosition - rootPos;
    direction.Normalize( );

    for (size_t i = 1, n = m_joints.size( ); i < n; ++i)
    {
        m_joints[ i ].SetWorldPosition(
            rootPos + direction * m_distanceFromRoot[ i ]
        );
    }
}

void IKLeaf::FABRIK(const SVec3 &solveForPosition)
{
    int i = 0;
    while (i++ < 10)
    {
        // Save the root position
        auto rootPosition = m_joints[ 0 ].GetWorldPosition( );

        // backward step
        backwardStep( solveForPosition );

        // forward step
        forwardStep( rootPosition );

        // check for completion
        auto thisPosition = m_joints.back( ).GetWorldPosition( );
        auto distance = SVec3::Distance( thisPosition, solveForPosition );

        if (distance <= m_errorMargin)
        {
            break;
        }
    }
}

void IKLeaf::backwardStep(const SVec3 &solveForPosition)
{
    // place leaf at goal
    m_joints.back( ).SetWorldPosition( solveForPosition );

    // calculate out parent positions
    for (int i = (int)m_joints.size( ) - 2; i >= 0; --i)
    {
        auto &childPos = m_joints[ i + 1 ].GetWorldPosition();
        auto direction = m_joints[ i ].GetWorldPosition() - childPos;
        auto distance = m_distanceFromRoot[ i + 1 ] - m_distanceFromRoot[ i ];

        direction.Normalize( );

        m_joints[ i ].SetWorldPosition( childPos + direction * distance );
    }
}

void IKLeaf::forwardStep(const SVec3 &rootPosition)
{
    // Place root at root's position
    m_joints[ 0 ].SetWorldPosition( rootPosition );

    // Calculate out children's position
    for (size_t i = 1, n = m_joints.size( ); i < n; ++i)
    {
        auto parentPos = m_joints[ i - 1 ].GetWorldPosition( );
        auto direction = m_joints[ i ].GetWorldPosition( ) - parentPos;
        auto distance = m_distanceFromRoot[ i ] - m_distanceFromRoot[ i - 1 ];

        direction.Normalize( );

        m_joints[ i ].SetWorldPosition( parentPos + direction * distance );
    }
}
