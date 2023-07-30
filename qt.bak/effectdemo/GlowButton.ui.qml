import QtQuick 2.12
import QtQuick.Studio.Components 1.0
import QtQuick.Timeline 1.0
import QtQuick.Studio.Effects 1.0

Item {
    id: button
    width: 110
    height: 110

    property bool glow: mouseArea.containsMouse

    Rectangle {
        id: rectangle
        x: 5
        y: 5
        width: 100
        height: 100
        color: "#00000000"
        radius: 50
        border.color: "#ffffff"
        border.width: 2
        layer.enabled: button.glow
        layer.effect: GlowEffect {
            id: glow
            color: "#80ffffff"
            radius: 3
            samples: 4
            spread: 0.5
        }

        Text {
            id: text3
            x: 184
            y: 826
            color: "#ffffff"
            text: qsTrId("Start")
            anchors.verticalCenter: parent.verticalCenter
            font.letterSpacing: 1.5
            font.pixelSize: 18
            anchors.verticalCenterOffset: 0
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            font.bold: true
            font.family: "Tahoma"
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
    }
}

/*##^##
Designer {
    D{i:0;height:110;width:110}D{i:4}
}
##^##*/

