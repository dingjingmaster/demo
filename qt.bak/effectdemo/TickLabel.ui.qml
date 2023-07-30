import QtQuick 2.12
import QtQuick.Studio.Components 1.0
import QtQuick.Shapes 1.0
import QtQuick.Studio.Effects 1.0
import QtQuick.Timeline 1.0
import QtQuick.Studio.LogicHelper 1.0

Item {
    property alias text: textel1.text
    property alias value: timeline.currentFrame

    width: text1.width
    height: text1.height

    Text {
        id: textel1
        color: "#ffffff"
        text: "0"

        font.pixelSize: 20

        layer.effect: GlowEffect {
            id: glow
        }
        layer.enabled: true
        font.family: "Verdana"
    }

    Text {
        id: textel

        color: "#ffffff"
        text: textel1.text
        font.pixelSize: 20

        font.family: "Verdana"
    }

    Timeline {
        id: timeline
        endFrame: 10
        startFrame: 0
        enabled: true

        KeyframeGroup {
            target: textel1
            property: "opacity"
            Keyframe {
                frame: 0
                value: 0
            }

            Keyframe {
                frame: 10
                value: 1
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;height:44;width:78}D{i:1}D{i:4}
}
##^##*/

