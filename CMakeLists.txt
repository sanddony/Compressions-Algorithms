cmake_minimum_required(VERSION 3.21.1)

set(BUILD_QDS_COMPONENTS ON CACHE BOOL "Build design studio components")
set(CMAKE_CXX_STANDARD 20)
project(CompressionAlgorithmsApp LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(QT NAMES Qt6 COMPONENTS Gui Qml Quick)
find_package(Qt6 REQUIRED COMPONENTS Core Qml Quick)

# To build this application you need Qt 6.2.0 or higher
if (Qt6_VERSION VERSION_LESS 6.2.0)
message(FATAL_ERROR "You need Qt 6.2.0 or newer to build the application.")
endif()

include_directories(src/Controllers)
include_directories(src/Models)
include_directories(src/Services)
include_directories(src/Utility)
include_directories(src/Services/Compression/Huffman)


file(GLOB
        CPP_SOURCES "src/Utility/*.cpp"
                    "src/Services/Entropy/*.cpp"
                    "src/Services/Compression/*.cpp"
                    "src/Services/Compression/Huffman/*.cpp"
                    "src/Controllers/*.cpp"
                    "src/Models/*.cpp"
                    "src/*.cpp")

qt_add_executable(${CMAKE_PROJECT_NAME} ${CPP_SOURCES})


# qt_standard_project_setup() requires Qt 6.3 or higher. See https://doc.qt.io/qt-6/qt-standard-project-setup.html for details.
if (${QT_VERSION_MINOR} GREATER_EQUAL 3)
qt6_standard_project_setup()
endif()

qt_add_resources(${CMAKE_PROJECT_NAME} "configuration"
    PREFIX "/"
    FILES
        qtquickcontrols2.conf
)


qt_add_resources(${CMAKE_PROJECT_NAME} "images"
    PREFIX "/image"
    FILES
        asset_imports/icons/gallery/20x20/back.png
        asset_imports/icons/gallery/20x20/drawer.png
        asset_imports/icons/gallery/20x20/menu.png
        asset_imports/icons/gallery/20x20@2/back.png
        asset_imports/icons/gallery/20x20@2/drawer.png
        asset_imports/icons/gallery/20x20@2/menu.png
        asset_imports/icons/gallery/20x20@3/back.png
        asset_imports/icons/gallery/20x20@3/drawer.png
        asset_imports/icons/gallery/20x20@3/menu.png
        asset_imports/icons/gallery/20x20@4/back.png
        asset_imports/icons/gallery/20x20@4/drawer.png
        asset_imports/icons/gallery/20x20@4/menu.png
        asset_imports/image/arrow.png
        asset_imports/icons/gallery/index.theme
)

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui
    Qt${QT_VERSION_MAJOR}::Quick
    Qt${QT_VERSION_MAJOR}::Qml
)

if (${BUILD_QDS_COMPONENTS})
    include(${CMAKE_CURRENT_SOURCE_DIR}/qmlcomponents)
endif ()

include(${CMAKE_CURRENT_SOURCE_DIR}/qmlmodules)
