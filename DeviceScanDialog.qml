import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MobileFarm 1.0

// Scan Devices Dialog
Dialog {
    id: deviceScanDialog
    title: qsTr("Scan Devices")
    width: 450
    height: 350
    modal: true

    anchors.centerIn: parent

    header: Item {
        height: 50
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12
            Label {
                text: qsTr("Scan Devices")
                font.bold: true
                font.pixelSize: 16
                Layout.fillWidth: true
            }
            Button {
                text: qsTr("Scan")
                background: Rectangle {
                    radius: 6
                    color: "grey"
                }
                onClicked: {
                    deviceScanner.scan();
                }
            }
        }
    }

    Rectangle {
        width: parent.width
        height: 1
        color: "grey"
    }

    // Content area with margin
    Item {
        anchors.fill: parent
        anchors.margins: 12

        // Add your dialog content here
        Rectangle {
            anchors.fill: parent
            color: "transparent"
            border.color: "#333"
            border.width: 1
            radius: 4

            Text {
                anchors.centerIn: parent
                text: "Dialog content area"
                color: "#666"
            }
        }
    }

    // Footer
    footer: Item {
        height: 50
        RowLayout {
            anchors.fill: parent
            anchors.margins: 12

            // Add Devices Button
            Button {
                text: qsTr("Add Devices")
                background: Rectangle {
                    radius: 6
                    color: "green"
                }
                onClicked: {
                    console.log("Add Devices clicked");
                }
            }

            // Spacer
            Item { Layout.fillWidth: true }

            // Close Button
            Button {
                text: qsTr("Close")
                background: Rectangle {
                    radius: 6
                    color: "grey"
                }
                onClicked: {
                    console.log("Close clicked");
                }
            }
        }
    }

    DeviceScanner {
        id: deviceScanner
        onScanCompleted: {
            console.log("Scan completed");
            // Handle scan results here
        }
    }
}
