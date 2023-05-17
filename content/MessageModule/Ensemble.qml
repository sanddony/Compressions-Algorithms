import QtQuick 6.2
import QtQuick.Controls 6.3
import QtQuick.Layouts 1.0
import CompressionAlgorithms
import EnsembleController 1.0

Rectangle{
    id: root
    border.color: Style.primaryColor
    border.width: 1
    radius: 2


    property EnsembleModel _ensembleModel: EnsembleModel

    property string _firstElemName: "Symbols"
    property string _secondElemName: "Frequency"

    ListView {
        id: _listView
        spacing: 8
        section.criteria: ViewSection.FirstCharacter
        orientation: ListView.Horizontal

        anchors.fill: parent
        anchors.margins: 5
        clip: true

        model: root._ensembleModel


        ScrollBar.horizontal: ScrollBar {
                id: vsb
                size: 0.1
        }

        header: Rectangle {
          id: headerItem
          width: 110
          height: _listView.height
          z:2
          color: "transparent"

          StandartText{
              text: _firstElemName
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.top: parent.top
              anchors.topMargin: 20
          }

          Rectangle {
              height:1
              color: Style.primaryColor
              width: parent.width
              anchors.centerIn: parent

          }

          StandartText{
              text: _secondElemName
              anchors.horizontalCenter: parent.horizontalCenter
              anchors.bottom: parent.bottom
              anchors.bottomMargin: 20
          }

        }

        delegate: SymbolEntity{
            height: root.height-10
            _frequency: frequency
            _symbol: symbol
            color: root.color
        }
    }

}

