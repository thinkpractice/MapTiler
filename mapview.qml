import QtQuick 2.0
import QtLocation 5.9
import QtPositioning 5.8

Rectangle {
    id: rectangle
    width: 400
    height: 400

    Plugin {
        id: mapPlugin
        name: "osm" // "mapboxgl", "esri", ...
        // specify plugin parameters if necessary
        // PluginParameter {
        //     name:
        //     value:
        // }
    }

    Map {
        anchors.fill: parent
        plugin: mapPlugin
        center: mainWindow.mapCenter // Oslo
        zoomLevel: 14
    }
}
