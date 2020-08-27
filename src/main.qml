import QtQuick 2.12
import QtQuick.Window 2.2
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Universal 2.12

import mf.components 1.0
import "Components"

Window {
    visible:true
    width:480
    height:480
    title: "Test application"
    id:wnd

    ColumnLayout {

        anchors.fill: parent;

        MandelbruhView2
        {
            SequentialAnimation on t {
                NumberAnimation { to: 1; duration: 2500; easing.type: Easing.InQuad }
                NumberAnimation { to: 0; duration: 2500; easing.type: Easing.OutQuad }
                loops: Animation.Infinite
                running: true
            }
        }
    }
}
