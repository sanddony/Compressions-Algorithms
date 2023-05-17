qt_add_library(Pages STATIC)

set_target_properties(Pages PROPERTIES AUTOMOC ON)
target_link_libraries(Pages PRIVATE Qt6::Quick)

qt6_add_qml_module(Pages
    URI "Pages"
    VERSION 1.0
    QML_FILES
        FileInputPage.qml
        MessagePage.qml
        BasePage.qml
        ByteOperationPage.qml
)
