import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.0
import QtQml.Models 2.15
 import QtGraphicalEffects 1.15
Rectangle{
    id: _treeRoot

    signal itemClicked(var name)

    property var itemArray
    property int currentIndex: -1
    property int totalCnt:0
    property var backColor:"#fff"
    property var selColor: "#ccc"
    property var containColor: "#bbb"
    property var backcolor:"#fff"
    property var accentIndicate: "#FF5E54"
    property var accentHint: "#EF3473"
    property var accentFont: "#000"
    property var layerEnable: true

    property int itemHeight      : 60
    color: backColor
    radius:10

    layer.enabled: layerEnable
    layer.effect: DropShadow{
        anchors.fill: _treeRoot
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#60000000"
        source: skin.shadowSupport ? _treeRoot : null
    }

    ListModel{
        id: _model
    }

    ColumnLayout{

        anchors.fill: parent
        ListView
        {
            Layout.margins: 10
            id: _view

            model:_model
            delegate:TreeDelegate{
                _itemHeight: itemHeight
                _selColor:selColor
                _containColor:containColor
                _backcolor:backColor
                _accentIndicate:accentIndicate
                _accentHint:accentHint
                _accentFont:accentFont
            }

            Layout.fillWidth: true
            Layout.fillHeight: true

            clip:true

            displaced: Transition {
                NumberAnimation{
                    properties:"x,y"
                    duration: 90
                }
            }

            ScrollBar.vertical: ScrollBar{

            }
        }
    }

    Component.onCompleted: {
        initModel(itemArray);
    }

    function printa(model)
    {
        for(var i = 0; i < model.count; i++)
        {
            if(model.get(i).child !== null && model.get(i).child !== undefined)
            {
                console.log("childs:");
                printa(model.get(i).child)
            }
        }
    }

    function initModel(data)
    {
        appendChild(_model, data, 0)
        //printa(_model)

    }

    function appendChild(parent, item, tier)
    {
        if(parent === undefined || parent === null)return
        if(item === undefined || item === null) return

        for(var i = 0; i < item.length; i++)
        {
            let childLen = -1

            if(item[i].child !== undefined && item[i].child !== null)
            {
                childLen = item[i].child.length
            }

            let model = createModel(item[i], childLen > 0 ? false : true, tier )
            parent.append(model)

            if(childLen > 0)
            {
                appendChild( parent.get(i).child, item[i].child, tier + 1)
            }
        }

    }

    function createModel(item, isleaf, tier)
    {
        if(item === undefined || item === null) return
        let toIndex= totalCnt
        //findItemById(_model.count)

        totalCnt += 1

        return {
            name:item.name,
            icon:item.icon,
            hint:item.hint === undefined ? "" : item.hint,
            toIndex:toIndex,
            tier: tier,
            expand:item.expand,
            child:[],
            isLeaf:isleaf,
        }
    }



}

