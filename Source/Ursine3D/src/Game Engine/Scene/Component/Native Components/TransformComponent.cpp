/* ---------------------------------------------------------------------------
** Team Bear King
** © 2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#include "UrsinePrecompiled.h"

#include "TransformComponent.h"
#include "Entity.h"
#include "EntityEvent.h"

namespace ursine
{
    namespace ecs
    {
        NATIVE_COMPONENT_DEFINITION( Transform );

        Transform::Transform(void)
            : BaseComponent( )
            , m_dirty( true )
            , m_root( this )
            , m_parent( nullptr )
            , m_localScale( 1.0f ) { }

        Transform::Transform(const Transform &transform)
            : BaseComponent( )
        {
            copy( transform );
        }

        Transform::~Transform(void)
        {
            if (m_parent)
                m_parent->RemoveChild( this );

            UAssert( m_children.size( ) == 0, "This should never happen, entities should be deleted from the bottom up." );
        }

        Transform &Transform::operator=(const Transform &transform)
        {
            copy( transform );

            return *this;
        }

        void Transform::OnInitialize(void)
        {
            Component::OnInitialize( );

            dispatchAndSetDirty( true, true, true );
        }

        void Transform::SetLocalPosition(const SVec3& position)
        {
            auto oldPosition = m_localPosition;

            m_localPosition = position;

            dispatchAndSetDirty( true, false, false );

            notifyPositionChanged( &oldPosition );
        }

        const SVec3 &Transform::GetLocalPosition(void) const
        {
            return m_localPosition;
        }

        void Transform::SetWorldPosition(const SVec3 &position)
        {
            auto oldPosition = m_localPosition;

            if (m_parent)
                m_localPosition = m_parent->ToLocal(position);
            else
                m_localPosition = position;

            dispatchAndSetDirty( true, false, false );

            notifyPositionChanged( &oldPosition );
        }

        SVec3 Transform::GetWorldPosition(void)
        {
            if (m_parent)
                return m_parent->ToWorld( m_localPosition );
            else
                return m_localPosition;
        }

        void Transform::SetLocalRotation(const SQuat& rotation)
        {
            auto oldRotation = m_localRotation;

            m_localRotation = rotation;

            dispatchAndSetDirty( false, false, true );

            notifyRotationChanged( &oldRotation );
        }

        void Transform::SetLocalEuler(const SVec3& euler)
        {
            auto oldRotation = m_localRotation;

            m_localRotation.SetEulerAngles( euler );

            dispatchAndSetDirty( false, false, true );

            notifyRotationChanged( &oldRotation );
        }

        const SQuat& Transform::GetLocalRotation(void) const
        {
            return m_localRotation;
        }

        SVec3 Transform::GetLocalEuler(void) const
        {
            return m_localRotation.GetEulerAngles( );
        }

        void Transform::SetLocalEulerEditor(const SQuat &euler)
        {
            SetLocalEuler( SVec3( euler.X( ), euler.Y( ), euler.Z( ) ) );
        }

        SQuat Transform::GetLocalEulerEditor(void) const
        {
            auto euler = GetLocalEuler( );

            return SQuat( euler.X( ), euler.Y( ), euler.Z( ), 0.0f );
        }

        void Transform::SetWorldRotation(const SQuat &rotation)
        {
            auto oldRotation = m_localRotation;

            if (m_parent)
                m_localRotation = m_parent->ToLocal( rotation );
            else
                m_localRotation = rotation;

            dispatchAndSetDirty( false, false, true );

            notifyRotationChanged( &oldRotation );
        }

        void Transform::SetWorldEuler(const SVec3 &euler)
        {
            auto oldRotation = m_localRotation;

            if (m_parent)
                m_localRotation = m_parent->ToLocal( SQuat( euler.X( ), euler.Y( ), euler.Z( ) ) );
            else
                m_localRotation.SetEulerAngles( euler );

            dispatchAndSetDirty( false, false, true );

            notifyRotationChanged( &oldRotation );
        }

        SQuat Transform::GetWorldRotation(void)
        {
            if (m_parent)
                return m_parent->ToWorld( m_localRotation );
            else
                return m_localRotation;
        }

        SVec3 Transform::GetWorldEuler(void)
        {
            if (m_parent)
                return m_parent->ToWorld( m_localRotation ).GetEulerAngles( );
            else
                return m_localRotation.GetEulerAngles( );
        }

        void Transform::LookAt(const SVec3& worldPosition)
        {
            SVec3 dir = worldPosition - GetWorldPosition( );

            SetWorldRotation( SQuat::LookAt( dir ) );
        }

        void Transform::SetLocalScale(const SVec3& scale)
        {
            auto oldScale = m_localScale;

            m_localScale = scale;

            dispatchAndSetDirty( false, true, false );

            notifyScaleChanged( &oldScale );
        }

        const SVec3 &Transform::GetLocalScale(void) const
        {
            return m_localScale;
        }

        SVec3 Transform::GetWorldScale(void)
        {
            if (m_parent)
                return m_localScale * m_parent->GetWorldScale();
            else
                return m_localScale;
        }

        void Transform::SetWorldScale(const SVec3 &scale)
        {
            auto oldScale = m_localScale;

            if (m_parent)
                m_localScale = m_parent->GetWorldScale( ) * scale;
            else
                m_localScale = scale;

            dispatchAndSetDirty( false, true, false );

            notifyScaleChanged( &oldScale );
        }

        SVec3 Transform::GetForward(void)
        {
            return GetWorldRotation( ) * SVec3::UnitZ( );
        }

        SVec3 Transform::GetRight(void)
        {
            return GetWorldRotation( ) * SVec3::UnitX( );
        }

        SVec3 Transform::GetUp(void)
        {
            return GetWorldRotation( ) * SVec3::UnitY( );
        }

        bool Transform::GetDirty(void) const
        {
            return m_dirty;
        }

        const SMat4 &Transform::GetLocalToWorldMatrix(void)
        {
            if (m_dirty)
                recalculateMatrices( );

            return m_localToWorld;
        }

        const SMat4 &Transform::GetWorldToLocalMatrix(void)
        {
            if (m_dirty)
                recalculateMatrices( );

            return m_worldToLocal;
        }

        SVec3 Transform::ToLocal(const SVec3& point)
        {
            return GetWorldToLocalMatrix( ).TransformPoint( point );
        }

        SQuat Transform::ToLocal(const SQuat& quat)
        {
            if (m_parent)
                return m_localRotation.GetInverse( ) * m_parent->ToLocal( quat );
            else
                return m_localRotation.GetInverse( ) * quat;
        }

        SVec3 Transform::ToWorld(const SVec3& point)
        {
            return GetLocalToWorldMatrix( ).TransformPoint( point );
        }

        SQuat Transform::ToWorld(const SQuat& quat)
        {
            if (m_parent)
                return m_parent->GetWorldRotation( ) * m_localRotation * quat;
            else
                return m_localRotation * quat;
        }

		Component::Handle<Transform> Transform::GetRoot(void)
        {
            return m_root;
        }

        Component::Handle<Transform> Transform::GetParent(void)
        {
            return m_parent;
        }

        bool Transform::IsChildOf(Handle<Transform> parent)
        {
            if (parent == m_parent)
                return true;

            if (m_parent)
                return m_parent->IsChildOf(parent);
            
            return false;
        }

        void Transform::AddChild(Transform* child)
        {
            // Store old values we need to retain for later
            auto oldWorldPos = child->GetWorldPosition( );
            auto oldWorldScale = child->GetWorldScale( );
            auto oldWorldRot = child->GetWorldRotation( );

            if (!genericAddChild( child ))
                return;

            // Set the new local values
            child->SetLocalPosition( ToLocal( oldWorldPos ) );
            child->SetLocalRotation( ToLocal( oldWorldRot ) );
            child->SetLocalScale( oldWorldScale / GetWorldScale( ) );
        }

        void Transform::AddChildAlreadyInLocal(Transform* child)
        {
            // Store old values we need to retain for later
            auto oldWorldPos = child->GetWorldPosition( );
            auto oldWorldScale = child->GetWorldScale( );
            auto oldWorldRot = child->GetWorldRotation( );

            if (!genericAddChild( child ))
                return;

            // Set the new local values
            child->SetLocalPosition( oldWorldPos );
            child->SetLocalRotation( oldWorldRot );
            child->SetLocalScale( oldWorldScale );
        }

        void Transform::RemoveChild(Transform* child)
        {
            // find the child in our local array of children
            auto itr = std::find( m_children.begin( ), m_children.end( ), child );

            if ( itr == m_children.end( ) )
                return;

            m_children.erase( itr );

            child->setParent( this, nullptr );
        }

        void Transform::RemoveChildren(void)
        {
            for (auto &child : m_children)
                child->setParent( this, nullptr );

            m_children.clear( );
        }

        Component::Handle<Transform> Transform::GetChild(uint index)
        {
            UAssert(index < m_children.size( ), "The index must be less than the child count");

            return m_children[ index ];
        }

        const Component::Handle<Transform> Transform::GetChild(uint index) const
        {
            UAssert(index < m_children.size( ), "The index must be less than the child count");

            return m_children[index];
        }

        const std::vector< Component::Handle<Transform> > &Transform::GetChildren(void) const
        {
            return m_children;
        }

        uint Transform::GetSiblingIndex(void) const
        {
            return GetOwner( )->GetSiblingIndex( );
        }

        void Transform::SetAsFirstSibling(void)
        {
            SetSiblingIndex( 0 );
        }

        void Transform::SetSiblingIndex(uint index)
        {
            GetOwner( )->SetSiblingIndex( index );

            if (m_parent == nullptr)
                return;

            auto &childArray = m_parent->m_children;

            int i = 0;
            for (auto child : childArray)
            {
                if (child == this)
                    break;
                else
                    ++i;
            }

            UAssert(i != childArray.size( ), "This shouldn't happen. Something is wrong with the parent's children array.");

            // walk from the old place to the new place, making sure all things are moved
            int dir = static_cast<int>( index ) > i ? 1 : -1;
            for (int j = i; j != index; j += dir)
            {
                childArray[ j ] = childArray[ j + dir ];
                childArray[ j + dir ] = this;
            }
        }

		Component *Transform::GetComponentInChildren(ComponentTypeID id) const
		{
			return GetOwner( )->GetComponentInChildren( id );
		}

	    Component *Transform::GetComponentInParent(ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentInParent( id );
	    }

	    ComponentVector Transform::GetComponentsInChildren(ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentsInChildren( id );
	    }

	    ComponentVector Transform::GetComponentsInParents(ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentsInParents( id );
	    }

	    void Transform::copy(const Transform &transform)
        {
            m_dirty = transform.m_dirty;

            m_localPosition = transform.m_localPosition;
            m_localRotation = transform.m_localRotation;
            m_localScale    = transform.m_localScale;

            m_localToWorld = transform.m_localToWorld;
            m_worldToLocal = transform.m_worldToLocal;

            if (transform.m_parent)
            {
				auto *p = const_cast<Transform*>( transform.m_parent.operator->( ) );
                p->AddChild( this );
            }
            else
            {
                m_root = this;
                m_parent = nullptr;
            }

            URSINE_TODO( "Test this and make sure it is called AFTER"
                         " the 'Create' function has been called in EntityManager.cpp" );

            notifyPositionChanged( nullptr );
            notifyRotationChanged( nullptr );
            notifyScaleChanged( nullptr );

            // We don't copy over children
        }

        void Transform::dispatchAndSetDirty(bool transChanged, bool scaleChanged, bool rotChanged)
        {
            TransformChangedArgs args( transChanged, scaleChanged, rotChanged );

            dispatchAndSetDirty( &args );
        }

        void Transform::dispatchAndSetDirty(const TransformChangedArgs *args)
        {
            m_dirty = true;

            GetOwner( )->Dispatch( ENTITY_TRANSFORM_DIRTY, args );
        }

        void Transform::dispatchParentChange(Handle<Transform> oldParent, Handle<Transform> newParent) const
        {
            ParentChangedArgs args( 
                newParent ? newParent->GetOwner( ) : nullptr,
                oldParent ? oldParent->GetOwner( ) : nullptr
            );

            auto *owner = GetOwner( );

            owner->Dispatch( ENTITY_PARENT_CHANGED, &args );

        #if defined(URSINE_WITH_EDITOR)

            owner->GetWorld( )->Dispatch( WORLD_EDITOR_ENTITY_PARENT_CHANGED, owner, &args );

        #endif
        }

        void Transform::onParentDirty(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, TransformChangedArgs);
            
            // dispatch to notify my children and anyone else who cares
            dispatchAndSetDirty( args );
        }

        void Transform::recalculateMatrices(void)
        {
            if (m_dirty)
            {
                if (m_parent)
                {
                    m_localToWorld = m_parent->GetLocalToWorldMatrix( ) *
                                     SMat4( m_localPosition, m_localRotation, m_localScale );
                }
                else
                {
                    m_localToWorld.TRS( m_localPosition, m_localRotation, m_localScale );
                }

                m_worldToLocal = m_localToWorld;
                m_worldToLocal.Inverse( );

                m_dirty = false;
            }
        }

        void Transform::notifyPositionChanged(const SVec3 *oldPosition)
        {
            if (!oldPosition || m_localPosition != *oldPosition)
            {
                NOTIFY_COMPONENT_CHANGED( "translation", m_localPosition );
            }
        }

        void Transform::notifyRotationChanged(const SQuat *oldRotation)
        {
            if (!oldRotation || m_localRotation != *oldRotation)
            {
                NOTIFY_COMPONENT_CHANGED("rotation", m_localRotation.GetEulerAngles());
            }
        }

        void Transform::notifyScaleChanged(const SVec3 *oldScale)
        {
            if (!oldScale || m_localScale != *oldScale)
            {
                NOTIFY_COMPONENT_CHANGED("scale", m_localScale);
            }
        }

        bool Transform::genericAddChild(Handle<Transform> child)
        {
            if (child->m_parent == this)
                return false;

            // edge case for if the child is my parent
            if (IsChildOf( child ))
                return false;
            
            // Add the child to this transform
            child->setParent( child->m_parent, this );
            m_children.push_back( child );

            return true;
        }

        void Transform::setParent(Handle<Transform> oldParent, Handle<Transform> newParent)
        {
            if (oldParent == newParent)
                return;

            m_parent = newParent;

			if (newParent)
				setRoot( newParent->m_root );
			else
				setRoot( this );

            // unsubscribe this entity from the old parent's events
            if (oldParent)
            {
                oldParent->GetOwner( )->Listener( this )
                    .Off( ENTITY_TRANSFORM_DIRTY, &Transform::onParentDirty );

                // remove this transform from the old parent
                oldParent->RemoveChild( this );
            }

            // subscribe this entity to my events
            if (newParent)
                newParent->GetOwner( )->Listener( this )
                    .On( ENTITY_TRANSFORM_DIRTY, &Transform::onParentDirty );

            // dispatch messages for hierarchical change
            dispatchParentChange( oldParent, newParent );
        }

		void Transform::setRoot(Handle<Transform> root)
        {
			m_root = root;

			for (auto &c : m_children)
			{
				c->setRoot( root );
			}
        }
    }
}
