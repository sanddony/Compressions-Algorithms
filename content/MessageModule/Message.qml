import QtQuick 6.2
import QtQuick.Controls 6.3
import QtQuick.Layouts 1.0
import CompressionAlgorithms
import EnsembleController 1.0
import MessageController 1.0

Rectangle {
    id: root

    property MessageController conrtroller: MessageController{}
    property int _margin: 30

    ColumnLayout {
        anchors.top: root.top
        anchors.topMargin: _margin

        anchors.left: root.left
        anchors.leftMargin: _margin


        spacing: 10


        TextField {
            id: _inputText
            placeholderText: qsTr("Введите текст")

            background: Rectangle {
                implicitWidth: 400
                implicitHeight: 40
                color: _inputText.enabled ? "transparent" : "#353637"
                border.color: _inputText.enabled ? Style.primaryColor : "transparent"
            }

        }

        GreenButton {
            id: _but
            text: qsTr("Calculate")

            onClicked:{
                conrtroller.setProperties(_inputText.text)

            }
            color: "white"

        }

        Ensemble{
            width: root.width - _margin*2
            height: 150
            _ensembleModel: conrtroller.ensemble
            color: root.color
        }

        Ensemble{
            width: root.width - _margin*2
            height: 150
            _ensembleModel: conrtroller.bigramEnsemble
            color: root.color

        }

        Parameter{
            width: 100
            height: 50
            parameterName: "Абсолютная энтропия"
            parameterValue: conrtroller.unconditionalEntropy
            color: root.color
        }

        Parameter{
            width: 100
            height: 50
            parameterName: "Максимальная энтропия"
            parameterValue: conrtroller.maximumEntropy
            color: root.color
        }

        Parameter{
            width: 100
            height: 50
            parameterName: "Энтропия первого порядка"
            parameterValue: conrtroller.firstOrderEntropy
            color: root.color
        }

    }


}
