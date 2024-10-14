import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.15

import Qt.Window 0.0
import Qt.Singleton 0.0

import "controls"

Frameless {
    id: _root;
    visible: true
    minimumWidth: 1600
    minimumHeight: 900
    color:"transparent"
    property var skin: Skin.skins[0]

    property var navigatorWidth : 230

    property var netState:"unconnect"

    disableArea: []

    ColumnLayout{
        anchors.fill: parent
        spacing:0
        Title{
            Layout.fillWidth:true
            height:30
        }

        MainWindow{
            id: _mainWindow
            Layout.fillWidth: true
            Layout.fillHeight: true

        }
        StateWindow{
            id: _stateWindow
            Layout.fillWidth: true
            Layout.preferredHeight: 30
        }
    }

    function addDisableArea(ctl){
        var point = ctl.mapToGlobal(ctl.x, ctl.y)
        var rect = Qt.rect(point.x, point.y, ctl.width, ctl.height)
        _root.appendArea( rect )
    }

    function notify(text)
    {
        _notify.notify(text)
    }

    Notification{
        id:_notify
        visible:false
        anchors.horizontalCenter: parent.horizontalCenter

    }


}
