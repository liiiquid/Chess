import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtGraphicalEffects 1.15

import "./pages"

Rectangle{
    id: _container
    radius: 10
    layer.enabled: skin.shadowSupport
    layer.effect: DropShadow{
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#60000000"
    }

    StackLayout {
        id: _stack
        anchors.fill: parent
        anchors.margins: 10
        currentIndex: 0

        BoardPage{
            id: _board
            property var name: "board"

        }

        Rectangle {
            property var name: "plum"
            color:"plum"
        }
    }

    property var _navigatorMenu :[
            {
                name:"root",
                expand:true,
                hint:"",
                icon:"qrc:/img/board1/rk.ico",
                child:[
                    {
                        name:_board.name,
                        icon:"qrc:/img/board1/ra.ico",
                        expand:true,
                        hint:"",
                    },
                    {
                        name:"plum",
                        icon:"qrc:/img/board1/ra.ico",
                        expand:true,
                        hint:"",
                    }
                ]

            }
    ]

    function switchPage(name)
    {
        for (var i = 0; i < _stack.data.length; i++) {
            if (_stack.data[i].name === name) {
                _stack.currentIndex = i;
                break;
            }
        }
    }

}


