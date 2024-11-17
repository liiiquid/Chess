import QtQuick 2.15
import QtQuick.Layouts 1.15
import Qt.Window 0.0
import QtQuick.Controls 2.0
import Qt.Auxiliary 0.0
import "../controls"
Item {

    property int verticalRulerWidth:24
    property int horizonRulerHeight:verticalRulerWidth

    RowLayout{
        anchors.fill: parent
        spacing:0
        Rectangle{

            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout{
                anchors.margins: 15
                anchors.fill: parent
                spacing:0

                HorizonRuler{
                    Layout.leftMargin: verticalRulerWidth
                    Layout.preferredWidth: _board.width
                    Layout.preferredHeight:horizonRulerHeight
                }

                RowLayout{
                    Layout.fillWidth : true
                    Layout.preferredHeight: _board.height
                    spacing:0
                    VerticalRuler{
                        Layout.preferredWidth:verticalRulerWidth
                        Layout.preferredHeight :_board.height
                    }

                    Board {
                        id: _board

                        Layout.preferredWidth: 520
                        Layout.preferredHeight: 576

                        Connections{
                            target: _board
                            function onNetStateChanged(v)
                            {
                                _root.netState = v;
                            }

                            function onInvalidMove()
                            {
                                _root.notify("invalid move")
                            }

                            function onSelectPieceError()
                            {
                                _root.notify("selected Piece side != current turn side");
                            }

                            function onUndomakeError()
                            {
                                _root.notify("undomake error");
                            }

                            function onGamewin(winner)
                            {
                                _root.notify(winner)
                            }

                            function onGoRecovered()
                            {
                                _goBtn.enabled = true;
                            }
                        }
                    }

                    VerticalRuler{
                        Layout.preferredWidth:verticalRulerWidth
                        Layout.preferredHeight :_board.height
                    }
                }
                HorizonRuler{
                    Layout.leftMargin: verticalRulerWidth
                    Layout.preferredWidth: _board.width
                    Layout.preferredHeight:horizonRulerHeight
                }

                Item{
                    Layout.fillHeight:true
                    Layout.fillWidth: true
                }

            }


        }



        Rectangle{
            Layout.fillWidth: true
            Layout.fillHeight: true
            color:"transparent"
            ColumnLayout{
                Layout.fillWidth: true
                Layout.fillHeight: true
                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "undo"
                        onClicked: _board.undoMakeMove()
                    }
                    Button{
                        text: "mirror_k=1"
                        onClicked: _board.mirror_k1()
                    }
                    Button{
                        text: "print"
                        onClicked: _board.print()
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "ucci"
                        onClicked: function va(){
                            _board.ucci()
                            _goBtn.enabled = true;
                        }
                    }
                    Button{
                        text: "isready"
                        onClicked: _board.isready()
                    }
                    Button{
                        text:"startpos"
                        onClicked: _board.startpos()
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "position"
                        onClicked: _board.position(posEdit.text)
                    }

                    TextControl{
                        text: "position startpos r"
                        id: posEdit
                    }

                    // Rectangle{
                    //     color:"#fe25ffc0"
                    //     implicitWidth: posEdit.contentWidth + 10
                    //     implicitHeight: 50
                    //     TextEdit{
                    //         id:posEdit
                    //         text:"position startpos r"
                    //         selectByMouse: true
                    //         cursorDelegate: Qt.PointingHandCursor
                    //         verticalAlignment: Text.AlignVCenter
                    //     }
                    // }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "getpos"
                        onClicked: _board.getpos()
                    }

                    Button{
                        text: "setpos"
                        onClicked: _board.setpos()
                    }


                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "go"
                        id: _goBtn;
                        onClicked: {
                            _board.go(gomodeedit.text);
                            _goBtn.enabled = false;
                        }
                    }

                    TextControl{
                        id: gomodeedit
                        text: "go depth 3"
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "protect"
                        onClicked: {
                            _board.protect(protectedit.text)
                        }
                    }

                    TextControl{
                        id: protectedit
                        text: "1 b9"
                    }
                }

                RowLayout{
                    Layout.fillWidth: true
                    Button{
                        text: "recover"
                        onClicked: {
                            _goBtn.enabled = true;
                        }
                    }
                }

            }
        }

    }


}
