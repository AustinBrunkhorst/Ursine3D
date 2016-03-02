/* ---------------------------------------------------------------------------
** Team Bear King
** ?2015 DigiPen Institute of Technology, All Rights Reserved.
**
** TransformComponent.h
**
** Author:
** - Jordan Ellis - contact@jordanellis.me
**
** Contributors:
** - <list in same format as author if applicable>
** -------------------------------------------------------------------------*/

#pragma once

#include "Component.h"
#include "SVec3.h"
#include "SQuat.h"
#include "SMat4.h"

namespace ursine
{
    namespace ecs
    {
        struct TransformChangedArgs : public EventArgs
        {
            bool transChanged, scaleChanged, rotChanged;

            TransformChangedArgs(bool transChanged, bool scaleChanged, bool rotChanged)
                : transChanged( transChanged )
                , scaleChanged( scaleChanged )
                , rotChanged( rotChanged ) { }
        };

        class Transform : public Component
        {
            NATIVE_COMPONENT;

        public:
            EditorField( 
                SVec3 translation,
                GetLocalPosition,
                SetLocalPosition
            );

            EditorField( 
                SVec3 scale,
                GetLocalScale,
                SetLocalScale
            );

            EditorMeta(ForceEditorType( typeof( ursine::SVec3 ) ))
            EditorMeta(EditorGetter( "GetLocalEulerEditor" ))
            EditorMeta(EditorSetter( "SetLocalEulerEditor" ))
            EditorField( 
                SQuat rotation,
                GetLocalRotation,
                SetLocalRotation
            );

            ALLOW_ALIGNED_ALLOC(16);

            Meta(Enable)
            Transform(void);

            Transform(const Transform &transform);

            ~Transform(void);

            Transform &operator=(const Transform &transform);

            void OnInitialize(void) override;

            ////////////////////////////////////////////////////////////////////
            // Position
            ////////////////////////////////////////////////////////////////////

            void SetLocalPosition(const SVec3 &position);
            const SVec3 &GetLocalPosition(void) const;

            void SetWorldPosition(const SVec3 &position);

            SVec3 GetWorldPosition(void);

            ////////////////////////////////////////////////////////////////////
            // Rotation
            ////////////////////////////////////////////////////////////////////

            void SetLocalRotation(const SQuat &rotation);

            void SetLocalEuler(const SVec3 &euler);
            const SQuat &GetLocalRotation(void) const;
            SVec3 GetLocalEuler(void) const;

            Meta(Enable)
            void SetLocalEulerEditor(const SQuat &euler);

            Meta(Enable)
            SQuat GetLocalEulerEditor(void) const;

            void SetWorldRotation(const SQuat &rotation);
            void SetWorldEuler(const SVec3 &euler);

            SQuat GetWorldRotation(void) const;
            SVec3 GetWorldEuler(void) const;

            void LookAt(const SVec3 &worldPosition);

            ////////////////////////////////////////////////////////////////////
            // Scale
            ////////////////////////////////////////////////////////////////////

            void SetLocalScale(const SVec3 &scale);
            const SVec3 &GetLocalScale(void) const;

            void SetWorldScale(const SVec3 &scale);

            SVec3 GetWorldScale(void) const;

            ////////////////////////////////////////////////////////////////////
            // Axis
            ////////////////////////////////////////////////////////////////////

            // Do this in the quaternion class
            SVec3 GetForward(void) const;
            SVec3 GetRight(void) const;
            SVec3 GetUp(void) const;

            ////////////////////////////////////////////////////////////////////
            // Matrix
            ////////////////////////////////////////////////////////////////////

            bool GetDirty(void) const;

            const SMat4 &GetLocalToWorldMatrix(void);

            const SMat4 &GetWorldToLocalMatrix(void);

            ////////////////////////////////////////////////////////////////////
            // Transformations
            ////////////////////////////////////////////////////////////////////

            SVec3 ToLocal(const SVec3 &point);
            SQuat ToLocal(const SQuat &quat) const;

            SVec3 ToWorld(const SVec3 &point);
            SQuat ToWorld(const SQuat &quat) const;

            ////////////////////////////////////////////////////////////////////
            // Hierarchy
            ////////////////////////////////////////////////////////////////////

            Handle<Transform> &GetRoot(void);
            const Handle<Transform> &GetRoot(void) const;

            Handle<Transform> &GetParent(void);
            const Handle<Transform> &GetParent(void) const;

            // Check to see if this transform is a 
            // child (anywhere in the hierarchy) of the given parent
            bool IsChildOf(const Handle<Transform> parent) const;

            // Add a child to the hierarchy, assuming its coordinates are in world space
            void AddChild(Transform *child);

            // Add a child to the hierarchy, assuming its coordinates are in local space
            void AddChildAlreadyInLocal(Transform *child);

            // Remove a child from the hierarchy (adding to root node)
            void RemoveChild(Transform *child);

            // Remove all children from the hiearchy (adding to root node)
            void RemoveChildren(void);

            // Detach from current parent
            void DetachFromParent(void);

            // Find child by their index in the list
            // If the index is too large, return nullptr
            Handle<Transform> &GetChild(uint index);
            const Handle<Transform> &GetChild(uint index) const;

            const std::vector<Handle<Transform>> &GetChildren(void) const;

            // Find this transform's index in relation to the other children
            uint GetSiblingIndex(void) const;

            // Move this transform to the start of the parent's children lists
            void SetAsFirstSibling(void);

            // Sets this transform's index in the parent's children list
            void SetSiblingIndex(uint index);

            // Gets a component of the specified type in this entity's children (type safe) (depth first)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline ComponentType *GetComponentInChildren(void) const;

            // Gets a component of the specified type id in this entity's children (depth first)
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponentInChildren(ComponentTypeID id) const;

            // Gets a component of the specified type in this entity's parent (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline ComponentType *GetComponentInParent(void) const;

            // Gets a component of the specified type id in this entity's parent
            // nullptr if it doesn't exist. Use the type safe version when possible
            Component *GetComponentInParent(ComponentTypeID id) const;

            // Gets the components of the specified type in this entity's children (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline std::vector<ComponentType*> GetComponentsInChildren(void) const;

            // Gets the components of the specified type id in this entity's children
            // nullptr if it doesn't exist. Use the type safe version when possible
            ComponentVector GetComponentsInChildren(ComponentTypeID id) const;

            // Gets the components of the specified type in this entity's parents (type safe)
            // nullptr if it doesn't exist
            template<class ComponentType>
            inline std::vector<ComponentType*> GetComponentsInParents(void) const;

            // Gets the components of the specified type id in this entity's parents
            // nullptr if it doesn't exist. Use the type safe version when possible
            ComponentVector GetComponentsInParents(ComponentTypeID id) const;

        protected:

            // Flag telling us if this object's data has been changed this frame
            bool m_dirty;

            // The top most transform in the hierarchy
            // If this is the top most, root == this
            Handle<Transform> m_root;

            // The parent of this transform.
            // If there is no parent, parent == nullptr
            Handle<Transform> m_parent;

            // Child pointers.
            std::vector< Handle<Transform> > m_children;

            // Local coordinates (coordinates in relation to the parent)
            SVec3 m_localPosition,
                  m_localScale;

            SQuat m_localRotation;

            // Transformations
            SMat4 m_localToWorld;
            SMat4 m_worldToLocal;

        private:
            void copy(const Transform &transform);

            void dispatchAndSetDirty(bool transChanged, bool scaleChanged, bool rotChanged);
            void dispatchAndSetDirty(const TransformChangedArgs *args);
            void dispatchParentChange(const Handle<Transform> &oldParent, const Handle<Transform> &newParent) const;

            void onParentDirty(EVENT_HANDLER(Entity));

            // Recalculate the matrices
            void recalculateMatrices(void);

            // notify the editor our values have changed
            void notifyPositionChanged(const SVec3 *oldPosition);
            void notifyRotationChanged(const SQuat *oldRotation);
            void notifyScaleChanged(const SVec3 *oldScale);

            // Generically add a child to our hierarch, without 
            // handling value changes in scale, position, or rotation
            bool genericAddChild(Handle<Transform> child);

            void setParent(Handle<Transform> oldParent, Handle<Transform> newParent, bool removing = false);
            
            void setRoot(Handle<Transform> root);

        } Meta(
            Enable,
            WhiteListMethods,
            DisableComponentRemoval, 
            DisplayName( "Transform" )
        );
    }
}

#include "TransformComponent.hpp"
