import QtCore
import QtQuick 6.2
import QtQuick.Dialogs
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.0
import CompressionAlgorithms

BasePage {

    Rectangle{
        id: root

        ColumnLayout {
            anchors.top: root.top
            anchors.topMargin: 30

            anchors.left: root.left
            anchors.leftMargin: 30


            spacing: 10


            TextField {
                id: _inputPolynom
                placeholderText: qsTr("Введите полином")

                background: Rectangle {
                    implicitWidth: 400
                    implicitHeight: 40
                    color: _inputPolynom.enabled ? "transparent" : "#353637"
                    border.color: _inputPolynom.enabled ? Style.primaryColor : "transparent"
                }

            }

            TextField {
                id: _inputGeneratingPolynomial
                placeholderText: qsTr("Введите образующий полином")
//                text.color: "black"
                background: Rectangle {
                    implicitWidth: 400
                    implicitHeight: 40
                    color: _inputGeneratingPolynomial.enabled ? "transparent" : "#353637"
                    border.color: _inputGeneratingPolynomial.enabled ? Style.primaryColor :
                                                                       "transparent"
                }

            }

            GreenButton {
                id: _but
                text: qsTr("Найти остаток от деления")
//                fillColor: "black"

                onClicked:{
                    conrtroller.setProperties(_inputText.text)

                }

            }
        }
    }

}
