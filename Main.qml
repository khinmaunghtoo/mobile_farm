import QtQuick.Controls

ApplicationWindow {
    width: 1024
    height: 768
    visible: true
    title: qsTr("Phone Farms")

    menuBar: MenuBar {

        Menu {
            title: qsTr("Devices")
            Action {
                text: qsTr("Scan Devices")
                onTriggered: {
                    console.log("Scan Devices clicked")
                    deviceScanDialog.open();
                }
            }
        }

    }

    DeviceScanDialog {
        id: deviceScanDialog
    }

}
