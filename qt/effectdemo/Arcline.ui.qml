import QtQuick 2.12
import QtQuick.Studio.Components 1.0
import QtQuick.Timeline 1.0
import QtQuick.Studio.Effects 1.0
import QtQuick.Studio.LogicHelper 1.0

Item {
    id: item2
    width: 412
    height: 452
    property alias value: rangeMapper.input

    Item {
        id: item1
        x: 0
        y: 48
        width: 399
        height: 395
        layer.enabled: true

        Image {
            id: needle
            x: 202
            y: 6
            source: "needle.png"

            fillMode: Image.PreserveAspectFit
        }

        layer.effect: OpacityMaskEffect {
            id: opacityMask
            x: 202
            y: 6
            invert: false
            maskSource: arc
        }
    }

    ArcItem {
        id: arc
        x: 0
        y: 48
        width: 399
        height: 395
        visible: false
        begin: rangeMapper.output
        strokeWidth: 26
        end: 110
        strokeColor: "#ffffff"
        fillColor: "#00000000"
    }

    RangeMapper {
        id: rangeMapper
        inputMinimum: -20
        outputMaximum: 110
    }
}



