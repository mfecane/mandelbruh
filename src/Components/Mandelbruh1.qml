import QtQuick 2.12
import QtQuick.Layouts 1.12

import mf.components 1.0

MandelbruhView {
    id: mandelbruhview
    Layout.fillHeight: true
    Layout.fillWidth: true

    MouseArea{
        anchors.fill: parent
        onWheel: {
            if (wheel.angleDelta.y > 0) {
                mandelbruhview.zoomIn(wheel.x, wheel.y)
            } else {
                mandelbruhview.zoomOut(wheel.x, wheel.y)
            }
            wheel.accepted=true
        } // onWheel
    } // MouseArea
} // MandelbruhView
