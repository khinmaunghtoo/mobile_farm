
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Window
import MobileFarm 1.0


// Device List (Left Panel)
Rectangle {
    id: leftPanel
    SplitView.preferredWidth: parent.width * 0.3
    SplitView.minimumWidth: 100
    color: "#3d1d1d"

    property var deviceManager

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
        delegate: Row {
            spacing: 10
            padding: 5

            property var device: modelData

            // device serial
            Text {
                text: device.serial
                color: "white"
                font.pixelSize: 14
                anchors.verticalCenter: parent.verticalCenter
            }
            
            // start/stop scrcpy server button
            Button {
                text: device.running ? "⏹" : "▶"
                width: 20
                height: 20
                background: Rectangle {
                    radius: 4
                    color: device.running ? "#f44336" : "#4CAF50"
                }
                onClicked: {
                    console.log("Play device:", device.serial);
                    if (device.running) {
                        device.stopScrcpySession();
                    } else {
                        device.startScrcpySession();
                    }
                }
            }

        }
    }
}
