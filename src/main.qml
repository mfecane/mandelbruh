import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

import mf.components 1.0
import "Components"
import Theme 1.0

Window {
    visible:true
    width:480
    height:480
    title: "Test application"
    id:wnd

    color: Theme.background_color

    ColumnLayout {

        anchors.fill: parent;

//        Button {
//            text:"Refresh"
//            onClicked: mandelbruhview.updateImage()
//        }

//        Image {
//            Layout.fillHeight: true
//            Layout.fillWidth: true
//            id:image
//            source: BackEnd.src
//            fillMode: Image.PreserveAspectFit
//        }

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
    }
}
