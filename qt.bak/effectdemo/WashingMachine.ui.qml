import QtQuick 2.12
import QtQuick.Studio.Components 1.0
import QtQuick.Timeline 1.0
import QtQuick.Studio.Effects 1.0

Item {
    width: 270
    height: 540

    Image {
        id: background
        source: "background.png"
        scale: 0.5
        transformOrigin: Item.TopLeft
        fillMode: Image.PreserveAspectFit

        Text {
            id: text1
            x: 188
            y: 140
            color: "#ffffff"
            text: qsTrId("COTTON 30°")
            font.pixelSize: 26
            font.bold: true
            font.family: "Tahoma"
        }

        Arcline {
            id: arcline
            x: 67
            y: 309
            value: 60
        }

        Text {
            id: text2
            x: 184
            y: 826
            color: "#2b7524"
            text: qsTrId("PROGRAM HEALTH")
            font.letterSpacing: 1.5
            font.pixelSize: 16
            font.bold: true
            font.family: "Tahoma"
        }

        GlowButton {
            id: button
            x: 216
            y: 896
        }
    }

    Timeline {
        id: timeline
        animations: [
            TimelineAnimation {
                id: timelineAnimation
                pingPong: true
                loops: -1
                duration: 4000
                running: true
                to: 1000
                from: 0
            }
        ]
        enabled: true
        startFrame: 0
        endFrame: 1000

        KeyframeGroup {
            target: arcline
            property: "value"

            Keyframe {
                frame: 0
                value: 90
            }

            Keyframe {
                frame: 237
                easing.bezierCurve: [0.338,0,0.689,1.11,1,1]
                value: 50
            }

            Keyframe {
                frame: 436
                easing.bezierCurve: [0.324,0.116,0.665,1,1,1]
                value: 76.02679
            }

            Keyframe {
                frame: 666
                easing.bezierCurve: [0.344,0,0.656,1,1,1]
                value: 10
            }

            Keyframe {
                frame: 880
                easing.bezierCurve: [0.334,0,0.666,1,1,1]
                value: 0
            }

            Keyframe {
                frame: 1000
                easing.bezierCurve: [0.361,0,0.639,1,1,1]
                value: 50
            }
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#000000";formeditorZoom:1.33}D{i:8;unified:"010000"}D{i:6}
}
##^##*/

