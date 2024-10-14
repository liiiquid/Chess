import QtQuick 2.15

Rectangle {
    implicitWidth: 500
    implicitHeight:50
    visible:false
    id:_notify

    color:"#FC7930"

    property var content:"content"

    Text{
        anchors.fill: parent
        anchors.margins: 10
        text: content
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment : Text.AlignVCenter
        wrapMode: Text.Wrap
    }

    function notify(text)
    {
        content =text
        animation.restart();
    }

    SequentialAnimation{
            id: animation
            running: false
            ParallelAnimation {
                ScriptAction {
                    script: _notify.visible = true
                }
                NumberAnimation  {
                  target: _notify
                  property: "y"
                  from: 0
                  to: 40
                  duration: 300
                  easing.type : Easing.InOutQuad
                }
                NumberAnimation  {
                  target: _notify
                  property: "opacity"
                  from: 0.3
                  to: 1.0
                  duration: 300
                  easing.type : Easing.InOutQuad
                }
            }


            PauseAnimation {
                duration:  2000
            }

            ParallelAnimation {
                NumberAnimation  {
                  target: _notify
                  property: "y"
                  from: 40
                  to: 0
                  duration: 300
                  easing.type : Easing.InOutQuad
                }
                NumberAnimation  {
                  target: _notify
                  property: "opacity"
                  from: 1.0
                  to: 0.3
                  duration: 300
                  easing.type : Easing.InOutQuad
                }
            }

            ScriptAction {
                script: _notify.visible = false
            }
        }


}
