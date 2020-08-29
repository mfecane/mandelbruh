cmake_minimum_required(VERSION 3.18)

project("mandelbruh_qt_gpu")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_definitions(-DUNICODE -D_UNICODE)
add_definitions(-DMANDELBRUH1)

find_package(Qt5 COMPONENTS
    Widgets
    Quick
    Core
    QuickControls2
    Multimedia
    winextras
    REQUIRED)
    
qt5_add_resources(QTRESOURCES src/res.qrc)

file(GLOB_RECURSE SRC_UI
    src/main.qml
    src/Components/Mandelbruh1.qml
)

add_executable(${PROJECT_NAME} WIN32 
    src/main.cpp
    src/backend.cpp
    src/backend.h
    src/pch.h
    src/pch.cpp
    src/mandelbruh1/mandelbruh.h
    src/mandelbruh1/mandelbruh.cpp
    src/mandelbruh1/mandelbruhview.h
    src/mandelbruh1/mandelbruhview.cpp

    src/mandelbruhview2.h
    src/mandelbruhview2.cpp

    ${SRC_UI}
    ${RESOURCES}
    ${QTRESOURCES}
    )

    
source_group("ui" FILES ${SRC_UI})

set(QML_IMPORT_PATH 
    "${CMAKE_SOURCE_DIR}/src/imports" 
    CACHE STRING "" FORCE)

target_precompile_headers(${PROJECT_NAME}
    PUBLIC src/pch.h
    )

set(LIBS
    Qt5::Widgets
    Qt5::Core
    Qt5::Qml
    Qt5::Quick
    Qt5::Multimedia
    Qt5::WinExtras
    )

target_link_libraries(${PROJECT_NAME} ${LIBS})


# set startup project
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

if(CMAKE_BUILD_TYPE STREQUAL "Release")

# RUNNUNG WINDEPLOYQT

# Retrieve the absolute path to qmake and then use that path to find
# the binaries

     get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
     get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
     find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")

     add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
         COMMAND "${CMAKE_COMMAND}" -E
             env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
                 "$<TARGET_FILE:${PROJECT_NAME}>" ARGS --qmldir ${CMAKE_SOURCE_DIR}/src/
         COMMENT "Running windeployqt..."
     )

endif()
