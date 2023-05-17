import QtQuick 6.2
import QtQuick.Controls 6.3
import QtQuick.Layouts 1.0
import CompressionAlgorithms

Button {
    id: _but
    font: Constants.font
    property string color: "white"

    contentItem: Text {
        text: _but.text
        font: _but.font
        opacity: enabled ? 1.0 : 0.3
        color: _but.down ? Qt.darker(Style.primaryColor,1.2) : Style.primaryColor
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
        radius: 2
        color: _but.color
        Material.background: Style.primaryColor
    }

}
