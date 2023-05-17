qt_add_library(MessageModule STATIC)

set_target_properties(MessageModule PROPERTIES AUTOMOC ON)
target_link_libraries(MessageModule PRIVATE Qt6::Quick)

qt6_add_qml_module(MessageModule
    URI "MessageModule"
    VERSION 1.0
    QML_FILES
        Message.qml
        Ensemble.qml
        SymbolEntity.qml
        Parameter.qml
)
