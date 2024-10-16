import QtQuick 2.15

Rectangle{
    property alias text :edit.text

    color:"#fe25ffc0"
    implicitWidth: edit.contentWidth + 10
    implicitHeight: edit.contentHeight + 10

    TextEdit{
        id:edit
        text:parent.text
        selectByMouse: true
        verticalAlignment: TextEdit.AlignVCenter
    }



}
