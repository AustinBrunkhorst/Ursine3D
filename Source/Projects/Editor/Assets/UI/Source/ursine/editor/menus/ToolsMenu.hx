package ursine.editor.menus;
import ursine.native.Extern;

@menuIndex( 7 )
class ToolsMenu extends MenuItemHandler {
    @mainMenuItem( "Tools/Waypoint Connector/Unidirectional Connections" )
    static function uniConnector() {
        Extern.WaypointUnidirectionalConnector();
    }

    @mainMenuItem( "Tools/Waypoint Connector/Bidirectional Connections" )
    static function biConnector() {
        Extern.WaypointBidirectionalConnector();
    }

    @mainMenuItem( "Tools/Waypoint Connector/Debug Lines/Enable" )
    static function enableLines() {
        Extern.WaypointEnableDrawLines();
    }

    @mainMenuItem( "Tools/Waypoint Connector/Debug Lines/Disable" )
    static function disableLines() {
        Extern.WaypointDisableDrawLines();
    }
}
