import QtQuick 2.15
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.5
Rectangle {
    color:skin.stateColor

    RowLayout{
        anchors.fill: parent
        anchors.leftMargin: 30
        Label{
            text:"network:" + _root.netState
            Layout.preferredHeight: 30
            Layout.preferredWidth:100
        }
    }

}
