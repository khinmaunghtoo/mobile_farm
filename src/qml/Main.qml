import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window
import MobileFarm 1.0

ApplicationWindow {
    id: mainWindow
    width: 1024
    height: 768
    visible: true
    title: qsTr("Phone Farms")

    DeviceManager {
        id: deviceManager
    }
    
    // menuBar
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

            // no devices label
            Text {
                anchors.centerIn: parent
                text: qsTr("No Devices")
                color: "white"
                visible: deviceManager.devices.length === 0
            }

            // device list
            ListView {
                id: deviceListView
                anchors.fill: parent
                anchors.margins: 10
                model: deviceManager.devices
                delegate: Text {
                    text: modelData.serial
                    color: "white"
                    font.pixelSize: 14
                    padding: 5
                }
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
        deviceManager: deviceManager
    }
}
