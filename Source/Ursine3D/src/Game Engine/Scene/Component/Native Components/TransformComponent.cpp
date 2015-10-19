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
            , m_worldScale( 1.0f )
            , m_localScale( 1.0f )
        {
            dispatchDirty( );
        }

        Transform::Transform(const Transform &transform)
            : BaseComponent( )
        {
            copy( transform );
        }

        Transform::~Transform( )
        {
            if (m_parent)
                m_parent->RemoveChild( this );
        }

        Transform &Transform::operator=(const Transform &transform)
        {
            copy( transform );

            return *this;
        }

        void Transform::OnInitialize(void)
        {
            dispatchDirty( );
        }

        const SVec3 &Transform::GetWorldPosition(void) const
        {
            return m_worldPosition;
        }

        void Transform::SetWorldPosition(const SVec3 &position)
        {
            m_worldPosition = position;

            dispatchDirty( );

            NOTIFY_COMPONENT_CHANGED( "position", m_worldPosition );
        }

        const SQuat &Transform::GetWorldRotation(void) const
        {
            return m_worldRotation;
        }

        void Transform::SetWorldRotation(const SQuat &rotation)
        {
            m_worldRotation = rotation;

            dispatchDirty( );

            NOTIFY_COMPONENT_CHANGED( "rotation", m_worldRotation.GetEulerAngles( ) );
        }

        SVec3 Transform::GetWorldEuler(void) const
        {
            return m_worldRotation.GetEulerAngles( );
        }

        void Transform::SetWorldEuler(const SVec3 &euler)
        {
            m_worldRotation.SetEulerAngles( euler );

            dispatchDirty( );

            NOTIFY_COMPONENT_CHANGED( "rotation", euler );
        }

        void Transform::editorSetRotation(const SVec3 &euler)
        {
            m_worldRotation.SetEulerAngles( euler );

            dispatchDirty( );

            NOTIFY_COMPONENT_CHANGED( "rotation", euler );
        }

        const SVec3 &Transform::GetWorldScale(void) const
        {
            return m_worldScale;
        }

        SVec3 Transform::GetForward(void) const
        {
            return m_worldRotation * SVec3::UnitZ( );
        }

        SVec3 Transform::GetRight(void) const
        {
            return m_worldRotation * SVec3::UnitX( );
        }

        SVec3 Transform::GetUp(void) const
        {
            return m_worldRotation * SVec3::UnitZ( );
        }

        bool Transform::GetDirty(void) const
        {
            return m_dirty;
        }

        const SMat4& Transform::GetLocalToWorldMatrix(void)
        {
            if (m_dirty)
                recalculateLocalToWorldMatrix( );

            return m_localToWorld;
        }

        const SMat4& Transform::GetWorldToLocalMatrix(void)
        {
            if (m_dirty)
                recalculateWorldToLocalMatrix( );

            return m_worldToLocal;
        }

        SVec3 Transform::ToLocal(const SVec3& point)
        {
            return GetWorldToLocalMatrix( ).TransformPoint( point );
        }

        SQuat Transform::ToLocal(const SQuat& quat)
        {
            return m_worldRotation.GetInverse( ) * quat;
        }

        SVec3 Transform::ToWorld(const SVec3& point)
        {
            return GetLocalToWorldMatrix( ).TransformPoint( point );
        }

        SQuat Transform::ToWorld(const SQuat& quat)
        {
            return m_worldRotation * quat;
        }

        Transform* Transform::GetRoot(void)
        {
            return m_root;
        }

        Transform* Transform::GetParent(void)
        {
            return m_parent;
        }

        bool Transform::IsChildOf(Transform* parent)
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

            genericAddChild( child );

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

            genericAddChild( child );

            // Set the new local values
            child->SetLocalPosition( oldWorldPos );
            child->SetLocalRotation( oldWorldRot );
            child->SetLocalScale( oldWorldScale );
        }

        void Transform::RemoveChild(Transform* child)
        {
            // find the child in our local array of children
            auto itr = std::find( m_children.begin( ), m_children.end( ), child );

            UAssert( itr != m_children.end( ), 
                     "You're trying to remove a child "
                     "that isn't a child of this transform" );

            m_children.erase( itr );
            child->setParent( this, nullptr );
        }

        void Transform::RemoveChildren(void)
        {
            for (auto &child : m_children)
                child->setParent( this, nullptr );

            m_children.clear( );
        }

        Transform* Transform::GetChild(uint index)
        {
            UAssert(m_children.size() < index, "The index must be less than the child count");

            return m_children[ index ];
        }

        const Transform* Transform::GetChild(uint index) const
        {
            UAssert(m_children.size() < index, "The index must be less than the child count");

            return m_children[index];
        }

        uint Transform::GetSiblingIndex(void) const
        {
            return GetOwner( )->GetSiblingIndex( );
        }

        void Transform::SetAsFirstSibling(void)
        {
            GetOwner( )->SetAsFirstSibling( );
        }

        void Transform::SetSiblingIndex(uint index) const
        {
            GetOwner( )->SetSiblingIndex( index );
        }

        void Transform::SetWorldScale(const SVec3 &scale)
        {
            m_worldScale = scale;

            dispatchDirty( );

            NOTIFY_COMPONENT_CHANGED( "scale", scale );
        }

		Component* Transform::GetComponentInChildren(const Entity* entity, ComponentTypeID id) const
		{
			return GetOwner( )->GetComponentInChildren( entity, id );
		}

	    Component* Transform::GetComponentInParent(const Entity* entity, ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentInParent( entity, id );
	    }

	    ComponentVector Transform::GetComponentsInChildren(const Entity* entity, ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentsInChildren( entity, id );
	    }

	    ComponentVector Transform::GetComponentsInParents(const Entity* entity, ComponentTypeID id) const
	    {
			return GetOwner( )->GetComponentsInParents( entity, id );
	    }

	    void Transform::copy(const Transform &transform)
        {
            m_dirty = transform.m_dirty;

            m_worldPosition = transform.m_worldPosition;
            m_worldRotation = transform.m_worldRotation;
            m_worldScale    = transform.m_worldScale;

            m_localPosition = transform.m_localPosition;
            m_localRotation = transform.m_localRotation;
            m_localScale    = transform.m_localScale;

            m_localToWorld = transform.m_localToWorld;
            m_worldToLocal = transform.m_worldToLocal;

            if (transform.m_parent)
            {
                transform.m_parent->AddChild( this );
            }
            else
            {
                m_root = this;
                m_parent = nullptr;
            }

            URSINE_TODO( "Test this and make sure it is called AFTER"
                         " the 'Create' function has been called in EntityManager.cpp" );

            // We don't copy over children
        }

        void Transform::dispatchDirty(void) const
        {
            GetOwner( )->Dispatch( ENTITY_TRANSFORM_DIRTY, EventArgs::Empty );
        }

        void Transform::dispatchParentChange(Transform *oldParent, Transform *newParent) const
        {
            ParentChangedArgs args( oldParent->GetOwner( ), newParent->GetOwner( ) );

            GetOwner( )->Dispatch( ENTITY_PARENT_CHANGED, &args );
        }

        void Transform::onParentDirty(EVENT_HANDLER(Entity))
        {
            EVENT_ATTRS(Entity, EventArgs);

            // set myself to dirty
            m_dirty = true;

            // dispatch to notify my children and anyone else who cares
            dispatchDirty( );
        }

        void Transform::genericAddChild(Transform *child)
        {
            // Add the child to this transform
            m_children.push_back( child );
            child->setParent( child->m_parent, this );
        }

        void Transform::setParent(Transform *oldParent, Transform *newParent)
        {
            m_parent = newParent;
            m_root = newParent->m_root;
            m_dirty = true;

            // unsubscribe this entity from the old parent's events
            if (oldParent)
                oldParent->GetOwner( )->Listener( this )
                    .Off( ENTITY_TRANSFORM_DIRTY, &Transform::onParentDirty );

            // subscribe this entity to my events
            if (newParent)
                newParent->GetOwner( )->Listener( this )
                    .On( ENTITY_TRANSFORM_DIRTY, &Transform::onParentDirty );

            // dispatch messages for dirty and hierarchical change
            dispatchParentChange( oldParent, newParent );
            dispatchDirty( );
        }
    }
}
