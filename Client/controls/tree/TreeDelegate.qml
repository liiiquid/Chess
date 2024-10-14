import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

Rectangle {
    id: _treeDelegate
    width: ListView.view.width
    height:_columnLayout.height

    property var _selColor
    property var _containColor
    property var _backcolor
    property var _itemHeight

    property var _accentIndicate
    property var _accentHint
    property var _accentFont

    color:_backcolor


    ColumnLayout{
        id: _columnLayout
        width:_treeDelegate.width
        spacing: 0

        Rectangle{
            id: _item
            Layout.fillWidth: true
            Layout.preferredHeight: _itemHeight
            color: toIndex == currentIndex ? _selColor : _mouse.containsMouse ? _containColor : _backcolor
            Behavior on color {
                ColorAnimation { duration: 200 }
            }

            RowLayout{
                anchors.fill: parent
                spacing:5
                Image {
                    id: _icon
                    source: icon
                    Layout.preferredHeight: _itemHeight * 0.7
                    Layout.preferredWidth: Layout.preferredHeight
                }

                Text{
                    text: name
                    Layout.fillWidth: true
                    font.pointSize: 12
                    color:_accentFont
                    // horizontalAlignment: Text.AlignHCenter
                    verticalAlignment:Text.AlignVCenter
                }

                Rectangle{

                    property var maxWidth: _itemHeight * 0.6
                    property var minWidth: _itemHeight * 0.4
                    property var e:10
                    Layout.preferredHeight: minWidth
                    Layout.preferredWidth: _hint.contentWidth + e > maxWidth ? maxWidth : _hint.contentWidth + e < minWidth ? minWidth : _hint.contentWidth + e
                    Layout.rightMargin: 10
                    color:_accentHint
                    clip:true
                    visible:hint.length > 0 ? true : false

                    radius:Layout.preferredHeight / 2
                    Text{
                        anchors.fill: parent
                        id:_hint
                        text: hint
                        font.pointSize: 10

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment:Text.AlignVCenter
                    }
                }

                Image{
                    id: _indicator
                    source: "./img/navigation.png"
                    visible:!isLeaf
                    rotation: expand ? 0 : -90
                    Layout.preferredHeight: _itemHeight * 0.19
                    Layout.preferredWidth: Layout.preferredHeight
                    Layout.rightMargin: 15
                    Behavior on rotation{
                        NumberAnimation {
                            duration: 200
                        }
                    }

                    ColorOverlay{
                        anchors.fill: parent
                        color: _accentIndicate
                        source: _indicator
                    }

                }

            }



            MouseArea{
                id: _mouse
                hoverEnabled: true

                anchors.fill: parent
                onClicked:
                {
                    if( isLeaf )
                    {
                        itemClicked(name)
                    }else
                    {
                        expand = !expand
                    }

                    if(currentIndex != toIndex )
                    {
                        currentIndex = toIndex
                    }

                    //console.log("node clicked", name, icon, toIndex, hint, isLeaf, tier)

                }
            }
        }

        ListView{
            id: _child
            visible:true
            Layout.fillWidth: true
            Layout.preferredHeight: expand ? contentHeight : 0
            model: child
            delegate: _view.delegate

            clip: true

            Behavior on Layout.preferredHeight{
                NumberAnimation {
                    duration: 200
                }
            }
            displaced: Transition {
                NumberAnimation { properties: "x,y"; duration: 90 }
            }
        }
    }
}


