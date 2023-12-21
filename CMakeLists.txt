cmake_minimum_required(VERSION 3.5)

set (THISPROJECT "crioGui2")

project(${THISPROJECT} VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_MAIN_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui)

#Program sources directory
set (SOURCES_DIRECTORY ./src)


#recursively fill a list of all current project headers
message (STATUS "")
message (STATUS "*****************************************")
message (STATUS "* recursively load project headers: *.h *")
message (STATUS "*****************************************")
message (STATUS "")
file(GLOB_RECURSE PROJECT_HEADERS ${SOURCES_DIRECTORY}/*.h)
foreach( files ${PROJECT_HEADERS})
    get_filename_component(barename ${files} NAME)
    message(STATUS "found : " ${barename})
endforeach( files ${PROJECT_HEADERS} )

#recursively fill a list of all current project headers
message (STATUS "")
message (STATUS "********************************************")
message (STATUS "* recursively load project sources : *.cpp *")
message (STATUS "********************************************")
message (STATUS "")
file(GLOB_RECURSE PROJECT_SOURCES ${SOURCES_DIRECTORY}/*.cpp)
foreach( files ${PROJECT_SOURCES})
    get_filename_component(barename ${files} NAME)
    message(STATUS "found : " ${barename})
endforeach( files ${PROJECT_SOURCES} )




if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(${THISPROJECT}
        MANUAL_FINALIZATION
        ${PROJECT_MAIN_SOURCES}
        ${PROJECT_HEADERS}
        ${PROJECT_SOURCES}
        styles.qrc
        crio-gui2.qrc
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ${THISPROJECT} APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(${THISPROJECT} SHARED
            ${PROJECT_MAIN_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_SOURCES}
            crio-gui2.qrc
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(${THISPROJECT}
            ${PROJECT_MAIN_SOURCES}
            ${PROJECT_HEADERS}
            ${PROJECT_SOURCES}
            crio-gui2.qrc
        )
    endif()
endif()

target_include_directories(${THISPROJECT} PUBLIC ${SOURCES_DIRECTORY})


find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Concurrent)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Network)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Charts REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS SerialBus)


target_link_libraries(${THISPROJECT} PRIVATE Qt${QT_VERSION_MAJOR}::Core
                                             Qt${QT_VERSION_MAJOR}::Gui
                                             Qt${QT_VERSION_MAJOR}::Widgets
                                             Qt${QT_VERSION_MAJOR}::Network
                                             Qt${QT_VERSION_MAJOR}::Concurrent
                                             Qt${QT_VERSION_MAJOR}::Charts
                                             Qt${QT_VERSION_MAJOR}::SerialBus)




set_target_properties(${THISPROJECT} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${THISPROJECT})
endif()
