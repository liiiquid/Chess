pragma Singleton

import QtQuick 2.15

Item {
    property list<Item> skins:[

        Item{
            property var name:"colorful"

            property var titleColor: "red"
            property var mainColor:"pink"
            property var contentColor:"#fff"
            property var stateColor:"#DB5A6B"

            property var accentIndicate: "#EF3473"
            property var accentHint: "#FF5E54"
            property var accentFont: "#000"
            property var backColor: contentColor
            property var selColor: "#ccc"
            property var containColor: "#bbb"

            property var shadowSupport:true
            property var shadowColor: "#60000000"



        }

    ]

}
