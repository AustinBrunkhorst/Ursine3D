#include "Precompiled.h"

#include "DuplicateTool.h"
#include "SelectedComponent.h"

#include <EditorConfig.h>
#include <SystemManager.h>
#include <CameraComponent.h>
#include <Model3DComponent.h>

#include <math.h>

using namespace ursine;
using namespace ecs;

DuplicateTool::DuplicateTool(Editor *editor)
	: EditorTool( editor )
	, m_gizmo( nullptr )
	, m_selected( -1 )
	, m_snapping( false )
	, m_deleteGizmo( false )
	, m_planeType( PLANE_XZ )
	, m_plane( SVec3::Zero( ), SVec3::UnitX( ), SVec3::UnitZ( ) )
	, m_altDown( false )
	, m_origin( false )
{
	m_graphics = GetCoreSystem( graphics::GfxAPI );
	m_world = m_editor->GetProject( )->GetScene( )->GetWorld( );
	m_drawer = m_world->GetEntitySystem( DebugSystem );
	m_editorCameraSystem = m_world->GetEntitySystem( EditorCameraSystem );
}

void DuplicateTool::OnEnable(EntityUniqueID selected)
{
	m_selected = selected;

	if (m_selected != -1)
		enableGizmo( );
}

void DuplicateTool::OnDisable(void)
{
	m_selected = -1;
	m_snapping = false;

	disableGizmo( );
}

void DuplicateTool::OnSelect(Entity* entity)
{
	auto prevSelected = m_gizmo != nullptr;

	m_selected = entity->GetUniqueID( );

	if (!prevSelected)
		enableGizmo( );

	m_deleteGizmo = false;
}

void DuplicateTool::OnDeselect(Entity* entity)
{
	m_selected = -1;

	m_deleteGizmo = true;
}

void DuplicateTool::OnMouseDown(const MouseButtonArgs& args)
{
	// place the object and set it to be the next selected (+ remove selected from previous
	if (m_selected == -1 || args.button != MBTN_LEFT || m_altDown)
		return;

	auto selected = m_world->GetEntityUnique( m_selected );

	auto clone = selected->Clone( );

	selected->RemoveComponent<Selected>( );

	// Add the selected component
	clone->AddComponent<Selected>( );
	m_selected = clone->GetUniqueID( );

	// Set the position
	auto gizTrans = m_gizmo->GetTransform( );
	auto cloneTrans = clone->GetTransform( );
	
	cloneTrans->SetWorldPosition( gizTrans->GetWorldPosition( ) );

	// Set the name
	clone->SetName( selected->GetName( ) );
}

void DuplicateTool::OnKeyDown(const KeyboardKeyArgs& args)
{
	if (args.key == KEY_SPACE)
	{
		m_planeType = static_cast<PlaneType>( m_planeType + 1 );

		if (m_planeType == PLANE_NUM)
			m_planeType = PLANE_XZ;
	}

	if (args.key == KEY_LCONTROL)
		m_snapping = true;

	if (args.key == KEY_TILDE)
		m_origin = !m_origin;
}

void DuplicateTool::OnKeyUp(const KeyboardKeyArgs& args)
{
	if (args.key == KEY_LCONTROL)
		m_snapping = false;
}

void DuplicateTool::OnUpdate(KeyboardManager* kManager, MouseManager* mManager)
{
	if (m_deleteGizmo)
	{
		disableGizmo( );
		m_deleteGizmo = false;
	}

	m_altDown = kManager->IsDown( KEY_LMENU );

	// Place the gizmo where the mouse is
	updateGizmo( mManager );

	debugDraw( );
}

void DuplicateTool::enableGizmo(void)
{
	m_gizmo = m_world->CreateEntityFromArchetype(
		EDITOR_ARCHETYPE_PATH "EditorTools/DuplicateGizmo.uatype",
		"DuplicateGizmo"		
	);

	setEntitySerializationToggle( false, m_gizmo );

	// Make it invisible in the hiearchy
	m_gizmo->SetVisibleInEditor( false );

	// Make all models draw over everything
	for (auto &model : m_gizmo->GetComponentsInChildren<ecs::Model3D>( ))
	{
		//model->SetOverdraw( true );
		model->SetMaterialData( 4, 0, 0 );
	}
}

void DuplicateTool::disableGizmo(void)
{
	if (m_gizmo)
	{
		m_gizmo->Delete( );
		
		// Clear the deletion queue if the scene is paused
		EditorClearDeletionQueue( );
	}

	m_gizmo = nullptr;
}

void DuplicateTool::updateGizmo(MouseManager *mManager)
{
	if (m_selected == -1 || m_gizmo == nullptr)
		return;

	auto selected = m_world->GetEntityUnique( m_selected );
	auto gizTrans = m_gizmo->GetTransform( );
	auto selTrans = selected->GetTransform( );

	auto p = m_origin ? SVec3::Zero( ) : selTrans->GetWorldPosition( );

	// Update the plane
	switch (m_planeType)
	{
	case PLANE_XZ:
		m_plane = Plane( p, p + SVec3::UnitX( ), p + SVec3::UnitZ( ) );
		break;
	case PLANE_ZY:
		m_plane = Plane( p, p + SVec3::UnitZ( ), p + SVec3::UnitY( ) );
		break;
	case PLANE_XY:
		m_plane = Plane( p, p + SVec3::UnitX( ), p + SVec3::UnitY( ) );
		break;
	}

	// update the size of the gizmo (wrap the object with the gizmo)
	auto scale = selTrans->GetWorldScale( );
	auto rotation = selTrans->GetWorldRotation( );
	
	gizTrans->SetWorldScale( scale );
	gizTrans->SetWorldRotation( rotation );

	if (m_altDown)
	{
		gizTrans->SetWorldPosition( selTrans->GetWorldPosition( ) );
		return;
	}

	// update the position of the gizmo (position of the mouse pointer)
	auto screenP = mManager->GetPosition( );
	auto editorCam = m_editorCameraSystem->GetEditorCamera( );
	auto origin = editorCam->GetOwner( )->GetTransform( )->GetWorldPosition( );
	auto end = editorCam->ScreenToWorld( screenP, m_editorCameraSystem->GetCamZoom( ) );
	Ray ray( origin, -(end - origin) );

	auto position = m_plane.ClosestPoint( ray );

	if (m_snapping)
	{
		for (int i = 0; i < 3; ++i)
		{
			if (position[ i ] > 0.0f)
				position[ i ] = float(int(position[ i ] * 2.0f + 0.5f)) / 2.0f;
			else
				position[ i ] = float(int(position[ i ] * 2.0f - 0.5f)) / 2.0f;
		}
	}

	gizTrans->SetWorldPosition( position );
}

void DuplicateTool::debugDraw(void)
{
	if (m_selected == -1)
		return;

	SVec3 v[ 2 ];

	switch (m_planeType)
	{
	case PLANE_XZ:
		v[ 0 ] = SVec3::UnitX( ); v[ 1 ] = SVec3::UnitZ( );
		break;
	case PLANE_ZY:
		v[ 0 ] = SVec3::UnitZ( ); v[ 1 ] = SVec3::UnitY( );
		break;
	case PLANE_XY:
		v[ 0 ] = SVec3::UnitX( ); v[ 1 ] = SVec3::UnitY( );
		break;
	}

	auto gizTrans = m_gizmo->GetTransform( );
	auto scale = gizTrans->GetWorldScale( );
	auto maxScale = math::Max( fabsf(scale.X( )), fabsf(scale.Y( )), fabsf(scale.Z( )) );
	auto pos = gizTrans->GetWorldPosition( );
	int num = static_cast<int>( maxScale );
	
	for (int i = 0; i < 2; ++i)
	{
		auto v1 = i == 0 ? v[ 0 ] : v[ 1 ];
		auto v2 = i == 0 ? v[ 1 ] : v[ 0 ];
		SVec3 axis = pos - maxScale * 0.5f * v1;
		float mod = maxScale / num;
		SVec3 start = maxScale * 0.5f * v2;
		SVec3 end = -start;

		for (int j = 0; j <= num; ++j)
		{
			m_drawer->DrawLine( axis + start, axis + end, Color::Green, Color::Green, 0.0f);
			
			axis += mod * v1;
		}
	}
}

void DuplicateTool::setEntitySerializationToggle(bool toggle, Entity* entity)
{
	for (auto child : entity->GetTransform( )->GetChildren( ))
	{
		setEntitySerializationToggle( toggle, child->GetOwner( ) );
	}

	entity->EnableSerialization( toggle );
}