import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.0
import QtQuick.Particles 2.0
import Qt.Singleton 0.0
Window {
    id:_root
    width: 1200
    height: 700
    visible: true
    title: qsTr("Hello World")

    Rectangle{
        anchors.fill: parent
        Keys.onEscapePressed: _root.close()
        focus:true
        RowLayout{
            anchors.fill: parent
            ScrollView{
                id:flickable
                Layout.fillHeight: true
                Layout.preferredWidth: 600
                Layout.margins: 15
                TextArea{
                    id:textEdit
                    selectByMouse:true
                    wrapMode: TextEdit.WordWrap
                }

                ScrollBar.vertical: ScrollBar {
                                id: scroll1
                                policy: size<1.0? ScrollBar.AlwaysOn : ScrollBar.AlwaysOff
                                parent: flickable
                                anchors.top: flickable.top
                                anchors.right: flickable.right
                                anchors.bottom: flickable.bottom
                                contentItem: Item {
                                     anchors.horizontalCenter: scroll1.horizontalCenter
                                     implicitHeight: 100
                                     Rectangle {
                                          x: scroll1.width/2-width/2
                                          width: scroll1.hovered || scroll1.pressed ? 8: 3
                                          height: parent.height
                                          radius: width / 2
                                          Behavior on width {
                                              NumberAnimation { duration: 150; easing.type: Easing.InOutQuad}
                                          }
                                          color: scroll1.pressed  ? "#ff1054"  : "#ff1012"
                                     }
                                }
                                width: 8
                                background: Item {

                                }
                           }
            }


            ColumnLayout
            {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Button{
                    text:Server.start ? "close" : "start"
                    onClicked: Server.start ? Server.close() : Server.init()
                }

                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button{
                        text:"print"
                        onClicked: Server.print();
                    }

                }

                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Button{
                        text:"BitRow"
                        onClicked: Server.printBitBoardRow();
                    }

                    Button{
                        text:"BitCol"
                        onClicked: Server.printBitBoardCol();
                    }

                    Button{
                        text:"BitPiece"
                        onClicked: Server.printBitPiece();
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Button{
                        text: "material";
                        onClicked: Server.printMaterial();
                    }

                    Button{
                        text: "AP";
                        onClicked: Server.printAp();
                    }

                    Button{
                        text: "Sh";
                        onClicked: Server.printSh();
                    }

                    Button{
                        text: "KT";
                        onClicked: Server.printKT();
                    }

                    Button{
                        text: "Rm";
                        onClicked: Server.printRm();
                    }
                }


                Item{
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }

            Connections{
                target: Server
                function onMessageReceived(rec){
                    textEdit.append(rec);
                }
            }


        }

    }



}
