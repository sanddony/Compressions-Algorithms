import QtQuick 6.2
import CompressionAlgorithms

Rectangle {
    id: root
    border.color : Style.primaryColor
    border.width: 1
    radius: 2
    implicitWidth: Math.max(_freq.implicitWidth, _symb.implicitWidth) + 10
    z:1

    property string _symbol: ""
    property double _frequency: 0.0


    StandartText{
        id: _symb
        text: _symbol
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 10
    }

    Rectangle {
        height:1
        color: Style.primaryColor
        width: root.width
        anchors.centerIn: parent

    }

    StandartText{
        id: _freq
        text: _frequency
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
    }



}
