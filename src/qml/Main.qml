import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window

ApplicationWindow {
    width: 1024
    height: 768
    visible: true
    title: qsTr("Phone Farms")
    

    menuBar: MenuBar {
        background: Rectangle {
            color: "#2b2b2b"
        }
        
        Menu {
            title: qsTr("Devices")
            Action {
                text: qsTr("Scan Devices")
                onTriggered: {
                    deviceScanDialog.open();
                }
            }
        }
    }

    // Main View
    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        // Left panel
        Rectangle {
            id: leftPanel
            SplitView.preferredWidth: parent.width * 0.3
            SplitView.minimumWidth: 100
            color: "#2b2b2b"

            Text {
                anchors.centerIn: parent
                text: qsTr("No Devices")
                color: "white"
            }
        }

        // Right panel
        Rectangle {
            id: rightPanel
            SplitView.fillWidth: true
            color: "#2b2b2b"

            // Scan Devices Button
            Button {
                anchors.centerIn: parent
                text: qsTr("Click To Scan Devices")
                onClicked: {
                    deviceScanDialog.open();
                }
            }
        }
    }

    // Device Scan Dialog
    DeviceScanDialog {
        id: deviceScanDialog
    }
}
