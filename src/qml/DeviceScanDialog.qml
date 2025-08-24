import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import MobileFarm 1.0

// Scan Devices Dialog
Dialog {
    id: deviceScanDialog

    // properties
    property bool selectAll: false
    property bool dontEffectAllCheckBox: false
    property var selectedDevices: []

    onSelectAllChanged: {
        if (dontEffectAllCheckBox) { return }
        for (var i = 0; i < deviceListView.count; i++) {
            var item = deviceListView.itemAtIndex(i);
            if (item) {
                item.checked = selectAll;
                if (selectAll) {
                    selectedDevices.push(item.text);
                } else {
                    selectedDevices = [];
                }
            }
        }
    }

    onOpened: {
        deviceScanner.scan(); // scan devices when dialog openned
    }
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

            BusyIndicator {
                visible: deviceScanner.isScanning
                running: deviceScanner.isScanning
                Layout.preferredWidth: 24
                Layout.preferredHeight: 24
            }

            // Scan Button
            Button {
                text: qsTr("Scan")
                background: Rectangle {
                    radius: 6
                    color: "grey"
                }
                enabled: !deviceScanner.isScanning
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

    // Select All Check Box
    Item {
        width: parent.width
        height: selectAllCheckBox.visible ? selectAllCheckBox.implicitHeight : 0
        
        CheckBox {
            id: selectAllCheckBox
            leftPadding: 20
            topPadding: 12
            bottomPadding: 12
            visible: deviceScanner.devices.length > 1
            checked: deviceScanDialog.selectAll
            onCheckedChanged: {
                console.log("Select All check box trigger:", checked);
                deviceScanDialog.dontEffectAllCheckBox = false;
                deviceScanDialog.selectAll = checked;
            }
            text: "Select All"
        }
    }

    // body
    Item {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            bottom: parent.bottom
            topMargin: selectAllCheckBox.visible ? selectAllCheckBox.height : 0
            margins: 12
        }

        // no devices found label
        Label {
            anchors.centerIn: parent
            text: qsTr("No devices found")
            visible: deviceScanner.devices.length === 0
            color: "gray"
            font.italic: true
        }

        // Device List
        ListView {
            id: deviceListView
            anchors.fill: parent
            anchors.leftMargin: 12
            model: deviceScanner.devices
            clip: true
            spacing: 5
            delegate: CheckBox {
                checked: false
                onCheckedChanged: {
                    // TODO: handle device selection
                    // if !checked, selectAll should be false, but, when selectAll change to false , should not effect other checkboxes.
                    if (!checked) {
                        dontEffectAllCheckBox = true;
                        deviceScanDialog.selectAll = false;
                        // remove device from selectedDevices
                        selectedDevices = selectedDevices.filter(function(value) {
                            return value !== modelData;
                        });
                    }
                }
                text: modelData
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
                }
            }

            // Spacer
            Item {
                Layout.fillWidth: true
            }

            // Close Button
            Button {
                text: qsTr("Close")
                background: Rectangle {
                    radius: 6
                    color: "grey"
                }
                onClicked: {
                    deviceScanDialog.close();
                }
            }
        }
    }

    // error dialog
    Dialog {
        id: errorDialog
        title: qsTr("Error")
        property string text: ""
        modal: true
        anchors.centerIn: parent
        standardButtons: Dialog.Ok
        contentItem: Text {
            text: errorDialog.text
            color: "white"
            wrapMode: Text.WordWrap
        }
    }

    DeviceScanner {
        id: deviceScanner
        onError: function (message) {
            errorDialog.text = message;
            errorDialog.open();
        }
    }

    DeviceManager {
        id: deviceManager
    }
}
