package ursine.editor.menus;

import ursine.native.Extern;

import ursine.editor.scene.entity.Entity;

@menuIndex( 3 )
class ProjectMenu extends MenuItemHandler {
    private static var m_buildSettingsEntity : Entity = null;
    private static var m_installSettingsEntity : Entity = null;

    @mainMenuItem( "Project/Edit Build Settings", false, false, 'settings' )
    static function doEditBuildSettings() {
        var settings = getBuildSettingsEntity( );

        settings.select( );
    }

    @mainMenuItem( "Project/Build", false, false, 'export' )
    static function doBuild() {
        Extern.ProjectBuildStart( );
    }

    @mainMenuItem( "Project/Edit Install Settings", true, false, 'settings' )
    static function doEditInstallerSettings() {
        var settings = getInstallSettingsEntity( );

        settings.select( );
    }

    @mainMenuItem( "Project/Build Installer", false, false, 'installer' )
    static function doInstall() {
        Extern.ProjectInstallStart( );
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

    private static function getInstallSettingsEntity() : Entity {
        if (m_installSettingsEntity == null || !m_buildSettingsEntity.isValid( )) {
            m_installSettingsEntity = Entity.create( );

            m_installSettingsEntity.setVisibleInEditor( false );
            m_installSettingsEntity.enableSerialization( false );

            m_installSettingsEntity.addComponent( 'ProjectInstallerSettings' );
        }

        return m_installSettingsEntity;
    }
}
