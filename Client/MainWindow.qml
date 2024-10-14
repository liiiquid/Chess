import QtQuick 2.15

import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.0

Rectangle {
    id: _mainWindow
    color:_root.skin.mainColor

    RowLayout{
        anchors.fill: parent
        anchors.margins: 10
        spacing:15
        Navigator{
            id:_navigator
            Layout.fillHeight: true
            width:_root.navigatorWidth
            itemArray: _pageWindow._navigatorMenu
        }

        PageWindow{
            id:_pageWindow
            Layout.fillWidth: true
            Layout.fillHeight: true
            color: _root.skin.contentColor
        }
    }

    Connections{
        target: _navigator
        function onNavigateItemClicked(name)
        {
            _pageWindow.switchPage(name)
        }
    }
}
