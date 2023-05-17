// Copyright (C) 2021 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR GPL-3.0-only

import QtCore
import QtQuick 6.2
import QtQuick.Layouts 1.0
import QtQuick.Controls
import CompressionAlgorithms
import MessageModule 1.0
import Pages


ApplicationWindow {
    id: window
    width: Constants.width
    height: Constants.height

    visible: true
    title: "CompressionAlgorithms"

    header: ToolBar {
        RowLayout {
            spacing: 20
            anchors.fill: parent

            ToolButton {
                action: navigateBackAction
            }

            Label {
                id: titleLabel
                text: listView.currentItem ? listView.currentItem.text : "Gallery"
                font.pixelSize: 20
                elide: Label.ElideRight
                horizontalAlignment: Qt.AlignHCenter
                verticalAlignment: Qt.AlignVCenter
                Layout.fillWidth: true
            }

            ToolButton {
                action: optionsMenuAction

                Menu {
                    id: optionsMenu
                    x: parent.width - width
                    transformOrigin: Menu.TopRight

                    Action {
                        text: "Settings"
//                        onTriggered: settingsDialog.open()
                    }
                    Action {
                        text: "Help"
//                        onTriggered: help()
                    }
                    Action {
                        text: "About"
//                        onTriggered: aboutDialog.open()
                    }
                }
            }
        }
    }

    Action {
        id: optionsMenuAction
        icon.name: "menu"
        onTriggered: optionsMenu.open()
    }



    StackView {
        id: stackView
        anchors.fill: parent

        initialItem: Pane {
            id: pane

            Label {
                text: "Это учебное приложение содержит в себе различные алгоритмы, такие как Алгоритм Хаффмана, Алгоритм Дейкстры и др."
                anchors.margins: 20
                anchors.left: parent.left
                anchors.right: parent.right
                horizontalAlignment: Label.AlignHCenter
                verticalAlignment: Label.AlignVCenter
                wrapMode: Label.Wrap
            }


        }
    }


    Action {
        id: navigateBackAction
        icon.name: stackView.depth > 1 ? "back" : "drawer"


        onTriggered: {
            if (stackView.depth > 1) {
                stackView.pop()
                listView.currentIndex = -1
            } else {
                drawer.open()
            }
        }
    }


    Drawer {
        id: drawer
        width: Math.min(window.width, window.height) / 3 * 2
        height: window.height
        interactive: stackView.depth === 1

        ListView {
            id: listView

            focus: true
            currentIndex: -1
            anchors.fill: parent

            delegate: ItemDelegate {
                width: listView.width
                text: model.title
                highlighted: ListView.isCurrentItem
                onClicked: {
                    listView.currentIndex = index
                    stackView.push(model.source)
                    drawer.close()
                }
            }

            model: ListModel {
                ListElement { title: "Message"; source: "qrc:Pages/MessagePage.qml"}
                ListElement { title: "Compress"; source: "qrc:Pages/FileInputPage.qml"}
                ListElement { title: "ByteOperationPage"; source: "qrc:Pages/ByteOperationPage.qml"}
            }

            ScrollIndicator.vertical: ScrollIndicator { }
        }
    }

}

