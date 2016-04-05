package ursine.editor.menus;

import ursine.native.Extern;

import ursine.editor.scene.entity.Entity;

@menuIndex( 3 )
class ProjectMenu extends MenuItemHandler {
    private static var m_buildSettingsEntity : Entity = null;

    @mainMenuItem( "Project/Edit Build Settings", false, false, 'settings' )
    static function doEditBuildSettings() {
        var settings = getBuildSettingsEntity( );

        settings.select( );
    }

    @mainMenuItem( "Project/Build", true, false, 'export' )
    static function doBuild() {
        Extern.ProjectBuildStart( );
    }

    private static function getBuildSettingsEntity() : Entity {
        if (m_buildSettingsEntity == null || !m_buildSettingsEntity.isValid( )) {
            m_buildSettingsEntity = Entity.create( );

            m_buildSettingsEntity.setVisibleInEditor( false );
            m_buildSettingsEntity.enableSerialization( false );

            m_buildSettingsEntity.addComponent( 'ProjectBuildSettings' );
        }

        return m_buildSettingsEntity;
    }
}
