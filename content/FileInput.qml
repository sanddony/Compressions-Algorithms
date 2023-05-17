import QtCore
import QtQuick 6.2
import QtQuick.Dialogs
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import CompressionAlgorithms

Rectangle {
    id: root
    border.color: "black"
    border.width: 5


    implicitHeight: 100
    implicitWidth: 250


    property string textHeader: "Header"

    ColumnLayout{
        anchors.top: root.top
        anchors.topMargin: 30

        anchors.left: root.left
        anchors.leftMargin: 30


        spacing: 10
        StandartText{
            text:textHeader
        }
        GreenButton{
            id: _but
            text: qsTr("Select file")

            onClicked:{
                fileDialog.open()
            }
        }
//        FileDialog{
//            id: fileDialog
//            currentFolder: StandardPaths.writableLocation(StandardPaths.DesktopLocation)
//        }
    }


}
