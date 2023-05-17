import QtQuick 6.2
import QtQuick.Controls 6.3
import QtQuick.Layouts 1.0
import CompressionAlgorithms

Rectangle {
    id: root

    property string parameterName
    property string parameterValue

    RowLayout {
        spacing: 10
        StandartText{
            text: root.parameterName + ":"
        }
        TextField {
            id: _inputText
            enabled: false
            text: root.parameterValue

            background: Rectangle {
                implicitHeight: 40
                implicitWidth: 200
                color: "transparent"
                border.color: Style.primaryColor
            }
        }
    }

}
