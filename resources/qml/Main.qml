//Main.qml

import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

ApplicationWindow {
    visible: true
    width: 600
    height: 500
    title: "Mouse Remapper"

    property var keyMap: {
        return {
            "Tab": 9, "Enter": 13, "Esc": 27, "Space": 32,
            "A": 65, "B": 66, "C": 67, "D": 68, "E": 69, "F": 70, "G": 71, "H": 72, "I": 73, "J": 74,
            "K": 75, "L": 76, "M": 77, "N": 78, "O": 79, "P": 80, "Q": 81, "R": 82, "S": 83, "T": 84,
            "U": 85, "V": 86, "W": 87, "X": 88, "Y": 89, "Z": 90,
            "0": 48, "1": 49, "2": 50, "3": 51, "4": 52, "5": 53, "6": 54, "7": 55, "8": 56, "9": 57,
            "Left": 37, "Up": 38, "Right": 39, "Down": 40,
            "Shift": 16, "Ctrl": 17
        }
    }

    function refreshUI() {
        let config = hookManager.getConfig();
        for (let i = 0; i < buttonRepeater.count; i++) {
            let item = buttonRepeater.itemAt(i);
            let cfg = config[String(i + 1)];
            if (!cfg) continue;

            item.enableCheckBox.checked = cfg.remap;
            item.repeatCheckBox.checked = cfg.repeat;
            item.untilClickCheckBox.checked = cfg.untilClick;

            for (let j = 0; j < 3; j++) {
                item.keySelections[j].enabled = false;
                item.keySelections[j].key = "A";
                item.comboBoxes[j].currentIndex = 0;
            }

            let keys = cfg.keys;
            for (let j = 0; j < keys.length && j < 3; j++) {
                let vk = keys[j];
                for (let key in keyMap) {
                    if (keyMap[key] === vk) {
                        item.keySelections[j].enabled = true;
                        item.keySelections[j].key = key;
                        item.comboBoxes[j].currentIndex = item.comboBoxes[j].model.indexOf(key);
                        break;
                    }
                }
            }
            item.updateRemap();
        }
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 20

        ColumnLayout {
            id: layoutRoot
            width: parent.width
            spacing: 10
            Layout.alignment: Qt.AlignHCenter

            Repeater {
                id: buttonRepeater
                model: 5
                ColumnLayout {
                    id: remapGroup
                    spacing: 10
                    Layout.alignment: Qt.AlignHCenter

                    property int buttonIndex: index + 1
                    property alias enableCheckBox: enableCheck
                    property alias repeatCheckBox: repeatCheck
                    property alias untilClickCheckBox: untilClickCheck

                    property var keySelections: [
                        { enabled: false, key: "A" },
                        { enabled: false, key: "A" },
                        { enabled: false, key: "A" }
                    ]
                    property var comboBoxes: []

                    function updateRemap() {
                        var keys = []
                        for (var j = 0; j < keySelections.length; j++) {
                            if (keySelections[j].enabled && keyMap[keySelections[j].key]) {
                                keys.push(keyMap[keySelections[j].key]);
                            }
                        }
                        hookManager.setRemap(
                            buttonIndex,
                            keys,
                            enableCheck.checked,
                            repeatCheck.checked,
                            untilClickCheck.checked
                        );
                    }

                    Text {
                        text: "Mouse Button " + buttonIndex
                        font.pixelSize: 18
                        Layout.alignment: Qt.AlignHCenter
                        padding: 10
                    }

                    Repeater {
                        model: 3
                        delegate: RowLayout {
                            Layout.alignment: Qt.AlignHCenter
                            spacing: 10

                            CheckBox {
                                checked: false
                                onCheckedChanged: {
                                    remapGroup.keySelections[index].enabled = checked;
                                    remapGroup.updateRemap();
                                }
                            }

                            ComboBox {
                                id: comboBoxRef
                                model: Object.keys(keyMap)
                                onCurrentTextChanged: {
                                    remapGroup.keySelections[index].key = currentText;
                                    remapGroup.updateRemap();
                                }
                                Component.onCompleted: remapGroup.comboBoxes[index] = comboBoxRef
                                Layout.preferredWidth: 150
                            }
                        }
                    }

                    CheckBox {
                        id: enableCheck
                        text: "Enable remap"
                        Layout.alignment: Qt.AlignHCenter
                        onCheckedChanged: remapGroup.updateRemap()
                    }

                    CheckBox {
                        id: repeatCheck
                        text: "Enable repeat"
                        Layout.alignment: Qt.AlignHCenter
                        onCheckedChanged: remapGroup.updateRemap()
                    }

                    CheckBox {
                        id: untilClickCheck
                        text: "Repeat until click"
                        Layout.alignment: Qt.AlignHCenter
                        onCheckedChanged: remapGroup.updateRemap()
                    }
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignHCenter
                spacing: 20

                Button {
                    id: startBtn
                    text: "Start"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        if (startBtn.text === "Start") {
                            hookManager.startHook();
                            startBtn.text = "Stop";
                        } else {
                            hookManager.stopHook();
                            startBtn.text = "Start";
                        }
                    }
                }

                Button {
                    text: "Save Preset"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: configManager.saveConfig()
                }

                Button {
                    text: "Load Preset"
                    Layout.alignment: Qt.AlignHCenter
                    onClicked: {
                        configManager.loadConfig();
                        Qt.callLater(refreshUI);
                    }
                }
            }
        }
    }
}