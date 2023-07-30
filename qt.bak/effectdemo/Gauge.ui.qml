import QtQuick 2.12
import QtQuick.Studio.Components 1.0
import QtQuick.Shapes 1.0
import QtQuick.Studio.Effects 1.0
import QtQuick.Timeline 1.0
import QtQuick.Studio.LogicHelper 1.0

Rectangle {
    width: 400
    height: 400
    color: "#000000"

    GroupItem {
        x: 51
        y: 54
        layer.effect: GaussianBlurEffect {
            id: gaussianBlur
            radius: rectangle.width
            samples: 20
        }
        layer.enabled: true

        Item {
            id: item2
            width: 320
            height: 320
            layer.enabled: true
            layer.effect: OpacityMaskEffect {
                id: opacityMask
                maskSource: mask
            }

            PieItem {
                id: pie
                x: 5
                y: 5
                width: 315
                height: 315
                layer.enabled: true
                gradient: ConicalGradient {
                    centerY: 160
                    centerX: 160
                    GradientStop {
                        position: 0
                        color: "#87d080"
                    }

                    GradientStop {
                        position: 0.23974
                        color: "#3cd62d"
                    }

                    GradientStop {
                        position: 0.88553
                        color: "#30000000"
                    }

                    angle: -90
                    GradientStop {
                        position: 1.00648
                        color: "#00000000"
                    }
                }
                end: 360
                strokeColor: "#00000000"
            }
        }
        Item {
            id: mask
            width: 320
            height: 320
            visible: true

            ArcItem {
                id: arc
                width: 320
                height: 320
                end: 66
                strokeColor: "#ffffff"
                begin: -176
                strokeWidth: 60
                fillColor: "#00000000"
            }

            ArcItem {
                id: arc1
                x: 40
                y: 45
                width: 241
                height: 230
                end: arc.end
                strokeColor: "#8d1c1c1c"
                begin: -174
                strokeWidth: 40
                fillColor: "#00000000"
            }
        }

        Rectangle {
            id: rectangle
            width: 10
            height: 200
            visible: false
            color: "#ffffff"
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
        startFrame: 0
        endFrame: 1000
        enabled: true

        KeyframeGroup {
            target: arc
            property: "end"
            Keyframe {
                value: -120
                frame: 0
            }

            Keyframe {
                value: 120
                frame: 1000
            }
        }

        KeyframeGroup {
            target: rectangle
            property: "width"
            Keyframe {
                value: 2
                frame: 0
            }

            Keyframe {
                value: 18
                frame: 1000
            }
        }

        KeyframeGroup {
            target: rot
            property: "rotation"
            Keyframe {
                value: 119
                frame: 1000
            }

            Keyframe {
                value: -120
                frame: 0
            }
        }

        KeyframeGroup {
            target: mask
            property: "visible"
            Keyframe {
                value: false
                frame: 0
            }
        }

        KeyframeGroup {
            target: arc2
            property: "strokeColor"
            Keyframe {
                value: "#081e0b"
                frame: 0
            }

            Keyframe {
                value: "#0b2c0f"
                frame: 1000
            }
        }

        KeyframeGroup {
            target: text2
            property: "value"
            Keyframe {
                frame: 0
                value: 10
            }

            Keyframe {
                frame: 66
                value: 0
            }
        }

        KeyframeGroup {
            target: text3
            property: "value"
            Keyframe {
                frame: 117
                value: 10
            }

            Keyframe {
                frame: 183
                value: 0
            }

            Keyframe {
                frame: 66
                value: 0
            }
        }

        KeyframeGroup {
            target: text4
            property: "value"
            Keyframe {
                frame: 229
                value: 10
            }

            Keyframe {
                frame: 295
                value: 0
            }

            Keyframe {
                frame: 178
                value: 0
            }
        }

        KeyframeGroup {
            target: text5
            property: "value"
            Keyframe {
                frame: 346
                value: 10
            }

            Keyframe {
                frame: 412
                value: 0
            }

            Keyframe {
                frame: 295
                value: 0
            }
        }

        KeyframeGroup {
            target: text6
            property: "value"
            Keyframe {
                frame: 508
                value: 10
            }

            Keyframe {
                frame: 562
                value: 0
            }

            Keyframe {
                frame: 432
                value: 0
            }
        }

        KeyframeGroup {
            target: text7
            property: "value"
            Keyframe {
                frame: 632
                value: 10
            }

            Keyframe {
                frame: 698
                value: 0
            }

            Keyframe {
                frame: 569
                value: 0
            }
        }

        KeyframeGroup {
            target: text8
            property: "value"
            Keyframe {
                frame: 766
                value: 10
            }

            Keyframe {
                frame: 805
                value: 0
            }

            Keyframe {
                frame: 700
                value: 0
            }
        }

        KeyframeGroup {
            target: text9
            property: "value"
            Keyframe {
                frame: 870
                value: 10
            }

            Keyframe {
                frame: 911
                value: 0
            }

            Keyframe {
                frame: 807
                value: 0
            }
        }

        KeyframeGroup {
            target: text10
            property: "value"
            Keyframe {
                frame: 1000
                value: 10
            }

            Keyframe {
                frame: 920
                value: 0
            }
        }
    }

    Rectangle {
        id: rectangle1
        x: 56
        y: 60
        width: 314
        height: 313
        color: "#00000000"
        radius: 157
        border.color: "#ffffff"
    }

    Rectangle {
        id: rectangle3
        x: 128
        y: 136
        width: 170
        height: 170
        color: "#000000"
        radius: 85
    }

    PieItem {
        id: rectangle2
        x: 128
        y: 132
        width: 170
        height: 170
        antialiasing: true
        fillColor: "#000000"
        strokeColor: "#ffffff"
        strokeWidth: 1
        begin: -90
        hideLine: true
    }

    ArcItem {
        id: arc2
        x: 173
        y: 181
        width: 80
        height: 80
        end: 359.9
        strokeColor: "#0b2c0f"
        fillColor: "#00000000"
    }

    ArcItem {
        id: arc3
        x: 183
        y: 191
        width: 60
        height: 60
        begin: -160
        fillColor: "#00000000"
        end: 30
        strokeColor: "#0b2c0f"
    }

    Text {
        id: text1
        x: 175
        y: 207
        width: 78
        height: 44
        color: "#ffffff"
        text: stringMapper.text
        font.pixelSize: 24
        horizontalAlignment: Text.AlignHCenter
        font.family: "Verdana"

        StringMapper {
            id: stringMapper
            input: rangeMapper.output
            decimals: 0
        }

        RangeMapper {
            id: rangeMapper
            outputMaximum: 160
            inputMaximum: 1000
            input: timeline.currentFrame
        }
    }

    GroupItem {
        x: 51
        y: 65

        Rectangle {
            id: tick01
            x: 5
            y: 156
            width: 10
            height: 1
            color: "#ffffff"
        }

        Rectangle {
            id: tick1
            x: 24
            y: 222
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -26.704
        }

        Rectangle {
            id: tick2
            x: 20
            y: 86
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -331.74
        }

        Rectangle {
            id: tick3
            x: 68
            y: 29
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -304.254
        }

        Rectangle {
            id: tick4
            x: 158
            y: 0
            width: 10
            height: 1
            color: "#ffffff"
            rotation: 90
        }

        Rectangle {
            id: tick5
            x: 246
            y: 28
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -230.142
        }

        Rectangle {
            id: tick6
            x: 294
            y: 87
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -27.702
        }

        Rectangle {
            id: tick7
            x: 309
            y: 151
            width: 10
            height: 1
            color: "#ffffff"
        }

        Rectangle {
            id: tick8
            x: 294
            y: 215
            width: 10
            height: 1
            color: "#ffffff"
            rotation: -330.232
        }

        TickLabel {
            id: text2
            x: 40
            y: 196
            value: 10
        }

        TickLabel {
            id: text3
            x: 21
            y: 141
            text: "20"
        }

        TickLabel {
            id: text4
            x: 36
            y: 76
            width: 78
            height: 44
            text: "40"
        }

        TickLabel {
            id: text5
            x: 75
            y: 35
            width: 78
            height: 44
            text: "60"
        }

        TickLabel {
            id: text6
            x: 151
            y: 8
            width: 78
            height: 44
            text: "80"
        }

        TickLabel {
            id: text7
            x: 204
            y: 28
            width: 78
            height: 44
            text: "100"
        }

        TickLabel {
            id: text8
            x: 252
            y: 81
            width: 78
            height: 44
            text: "120"
        }

        TickLabel {
            id: text9
            x: 266
            y: 137
            width: 78
            height: 44
            text: "140"
        }

        TickLabel {
            id: text10
            x: 248
            y: 191
            width: 78
            height: 44
            text: "160"
        }
    }
    Item {
        id: rot
        x: 111
        y: 117
        width: 200
        height: 200

        Rectangle {
            id: arrow
            x: 98
            y: -55
            width: 4
            height: 100
            layer.effect: DirectionalBlurEffect {
                id: directionalBlur
                length: 3
            }
            gradient: Gradient {
                GradientStop {
                    position: 0.99784
                    color: "#ffffff"
                }

                GradientStop {
                    position: 0
                    color: "#7b7b7b"
                }
            }
            layer.enabled: true

            Rectangle {
                id: rectangle6
                x: 3
                width: 1
                height: 100
                layer.enabled: true
            }
        }
    }

    ArcItem {
        id: arc4
        x: 75
        y: 83
        width: 280
        height: 280
        begin: 130
        strokeWidth: 16
        fillColor: "#00000000"
        end: 230
        strokeColor: "#07340c"
    }
}

/*##^##
Designer {
    D{i:0;height:1080;width:1920}D{i:15}
}
##^##*/
