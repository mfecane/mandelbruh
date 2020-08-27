import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.12

import mf.components 1.0

ListView {
    id: appListView
    clip:true
    spacing:0

    model: AppListModel {
        id:appListModel
        appManager: BackEnd.appManager
    }

    Component {
        id: appElement
        Item {
            id:wrapper
            height: 50
            width: appListView.width - 20
            RowLayout {
                anchors.fill: parent
                ColumnLayout {
                    Layout.fillWidth: true
                    Text {
                        Layout.fillWidth: true
                        text: model.text
                        elide: Text.ElideRight
                    }
                    Text {
                        Layout.fillWidth: true
                        text: model.filename
                        color: "gray"
                        elide: Text.ElideRight
                    }
                }
                CheckBox {
                    checked: model.selected
                }
            } // RowLayout
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    appListView.model.select(index)
                }
            } // MouseArea
        } // Item
    }

    delegate: appElement

    ScrollBar.vertical: ScrollBar { }
}
