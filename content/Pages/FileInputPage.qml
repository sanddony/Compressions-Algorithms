import QtCore
import QtQuick 6.2
import QtQuick.Layouts 1.0
import QtQuick.Controls
import QtQuick.Dialogs
import CompressionAlgorithms

import FilesInputController 1.0
import FilesListController 1.0
import FileController 1.0

BasePage{
    id: rootPage
    property int limitFilesUpload: 3
    property FilesInputController conrtroller: FilesInputController{}

    function loadFile(file) {
//        filesListContainer._listController = mainController.filesList
        console.log(_listView.model.rowCount())
        if(_listView.model.rowCount() < limitFilesUpload)
            _listView.model.setData(_listView.model.rowCount()-1,file,_listView.model.rowCount())

        else
            contentDialog.open();
    }

    Rectangle{ // mainContainer
        id:root
        anchors.fill: parent
        color: Style.backgroundColor;


        ColumnLayout{
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 10
            spacing: 10

            Rectangle{ // folder area
                id: folderArea
                color: root.color;
                implicitHeight: 150
                implicitWidth: 500

                border.color: Style.primaryColor
                border.width: 5

                ColumnLayout{

                    anchors.centerIn: folderArea

                    StandartText {
                        property var fullFolderName: folderDialog.selectedFolder
                        property var parts: fullFolderName.toString().split('/');
                        property string _folderName: '/' + parts[parts.length-2]+ '/' + parts[parts.length-1];
                        id: folderText
                        text: "Current folder: " + _folderName
                        color: "white"
                    }

                    Button{
                        id: _button
                        font: Constants.font
                        text: "Select"

                        contentItem: StandartText {
                            text: _button.text
                            opacity: enabled ? 1.0 : 0.3
                            color: _button.down ? Qt.darker("white",1.2) : "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            opacity: enabled ? 1 : 0.3
                            border.color: _button.down ? Qt.darker(Style.primaryColor,1.2) : Style.primaryColor
                            border.width: 1
                            radius: 30
                            color: Style.primaryColor
                            Material.background: Style.primaryColor
                        }
                        onClicked:{
                            folderDialog.open()
                        }
                    }

                    FolderDialog{
                        id: folderDialog
                        currentFolder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
                        onAccepted: {
//                            loadFile(selectedFile)
                            console.log(folderDialog.selectedFolder)
                            conrtroller.setFolderPath(folderDialog.selectedFolder)
//                            console.log(conrtroller.folderPath)
                        }
                    }
                }


            }

            Rectangle{
                color: root.color;
                implicitHeight: 120
                implicitWidth: dropArea.width

                border.color: Style.primaryColor
                border.width: 5

                Tumbler {

                    id: tumblerAlgorithm
                    model: ["Huffman's algorithm", "LZW"]
                    anchors.fill: parent
                    width: parent.width

                }

            }

            Rectangle{ // files dropArea
                id: dropArea
                color: root.color;
                implicitHeight: 100
                implicitWidth: 500

                border.color: Style.primaryColor
                border.width: 5




                RowLayout{

                    anchors.centerIn: dropArea

                    StandartText {
                        text: "Drag files here or "
                        color: "white"
                    }

                    Button{
                        id: _but
                        font: Constants.font
                        text: "Browse"

                        contentItem: StandartText {
                            text: _but.text
                            opacity: enabled ? 1.0 : 0.3
                            color: _but.down ? Qt.darker("white",1.2) : "white"
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        background: Rectangle {
                            implicitWidth: 100
                            implicitHeight: 40
                            opacity: enabled ? 1 : 0.3
                            border.color: _but.down ? Qt.darker(Style.primaryColor,1.2) : Style.primaryColor
                            border.width: 1
                            radius: 30
                            color: Style.primaryColor
                            Material.background: Style.primaryColor
                        }
                        onClicked:{
                            fileDialog.open()
                        }
                    }

                    FileDialog{
                        id: fileDialog
                        currentFolder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
                        onAccepted: {
                            loadFile(selectedFile)
                        }
                    }
                }

                Dialog {
                    id: contentDialog

                    x: (parent.width - width) / 2
                    y: (parent.height - height) / 2
                    width: Math.min(rootPage.width, rootPage.height) / 3 * 2
                    contentHeight: contentId.height * 2
                    parent: Overlay.overlay

                    modal: true
                    title: "Limits"
                    standardButtons: Dialog.Close

                    Flickable {
                        id: flickable
                        clip: true
                        anchors.fill: parent
                        contentHeight: column.height

                        Column {
                            id: column
                            spacing: 20
                            width: parent.width

                            Label {
                                id: contentId
                                width: parent.width
                                text: "Ограничения на загрузку файлов: " + limitFilesUpload.toString()
                                wrapMode: Label.Wrap
                            }
                        }

                        ScrollIndicator.vertical: ScrollIndicator {
                            parent: contentDialog.contentItem
                            anchors.top: flickable.top
                            anchors.bottom: flickable.bottom
                            anchors.right: parent.right
                            anchors.rightMargin: -contentDialog.rightPadding + 1
                        }
                    }
                }

                DropArea {
                    anchors.fill: parent
                    onEntered: (drag) => {
                        dropArea.color = "gray";
                        drag.accept (Qt.LinkAction);
                    }
                    onDropped: (drop) => {

                        for(let i = 0; i < drop.urls.length; ++i){
                            loadFile(drop.urls[i])
                        }
                        dropArea.color = Qt.darker(Style.backgroundColor,1.2)

                    }
                    onExited: {
                        dropArea.color = Style.backgroundColor;
                    }
                }


            }

            Rectangle{ // list Container
                id: filesListContainer
                implicitHeight: 200
                width: dropArea.width
                color: root.color

                property FilesListController _listController: FilesListController{}

                ListView{
                    id: _listView
                    spacing: 8

                    anchors.fill: parent
                    anchors.margins: 5

                    clip: true

                    model: filesListContainer._listController

                    ScrollBar.vertical: ScrollBar {
                            id: vsb
                            size: 0.1
                    }

                    delegate: Rectangle {
                        id: fileDelgate
                        property var fullFileName: modelData
                        property var parts: fullFileName.toString().split('/');
                        property var _fileName: parts[parts.length-1];

                        height: fileName.implicitHeight * 2
                        width: Math.max(fileName.implicitWidth, dropArea.width)
                        color: root.color


                        Rectangle{
                            id: delegateBackground
                            anchors.centerIn: parent
                            width: fileDelgate.width - 20
                            height: fileDelgate.height

                            color: Style.primaryColor
                            opacity: 0.7
                            radius: 10


                        }

                        StandartText{
                            id: fileName
                            text: _fileName
                            color: "white"
                            opacity: 1.0
                            anchors.centerIn: parent
                            width: fileDelgate.width
                        }

                        Component.onCompleted: {
                            if (fileName.paintedWidth > dropArea.width) {
                                width = dropArea.width
                            } else {
                                width = dropArea.width
                            }
                            root.height+=fileName.height
                        }

                    }
                }

            }

        }



        GreenButton{
            id: _calculateResult
            font: Constants.font
            text: "Compress"

            anchors.bottom: root.bottom
            anchors.right: root.right
            anchors.margins: 30

            onClicked:{
                console.log("AAAAAAA" + dropArea.y - _calculateResult.y)
                console.log(dropArea.y)
                console.log(_calculateResult.y)
            }
        }

    }


}

