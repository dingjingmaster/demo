import QtQuick 2.12
import QtQuick3D 1.15

Rectangle {
    width: 1024
    height: 768
    color: "#000000"

    WashingMachine {
        id: washingMachine
        x: 555
        y: 80
        width: 271
        height: 535
    }

    Gauge {
        id: gauge
        x: 26
        y: 8
    }
}
