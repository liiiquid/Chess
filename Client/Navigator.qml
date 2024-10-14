import QtQuick 2.15
import "./controls/tree/"
import QtGraphicalEffects 1.15

Rectangle{
    property alias itemArray : _tree.itemArray
    signal navigateItemClicked(var name)
    radius:10
    TreeEntry{
        anchors.fill: parent
        id: _tree
        radius: parent.radius
        backColor: skin.contentColor
        selColor: skin.selColor
        containColor: skin.containColor
        accentIndicate: skin.accentIndicate
        accentHint: skin.accentHint
        accentFont: skin.accentFont
        itemHeight: 60
        layerEnable: skin.shadowSupport

    }

    Connections{
        target: _tree
        function onItemClicked(name)
        {
            navigateItemClicked(name)
        }
    }

}


