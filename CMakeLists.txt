cmake_minimum_required(VERSION 3.16)

set(TARGET_NAME ReZonator) 

file(READ ${CMAKE_CURRENT_SOURCE_DIR}/release/version.txt versionString)
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)[-]([0-9a-z]+)" MY_PROGRAM_VERSION_MATCH ${versionString})

set(PROJECT_VERSION ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}) # -beta is skipped

message(STATUS "Project version:  ${CMAKE_MATCH_1}.${CMAKE_MATCH_2}.${CMAKE_MATCH_3}-${CMAKE_MATCH_4}")
add_definitions(-DAPP_VER_MAJOR="${CMAKE_MATCH_1}")
add_definitions(-DAPP_VER_MINOR="${CMAKE_MATCH_2}")
add_definitions(-DAPP_VER_PATCH="${CMAKE_MATCH_3}")
add_definitions(-DAPP_VER_CODENAME="${CMAKE_MATCH_4}")

string(TIMESTAMP YEAR "%Y")
add_definitions(-DAPP_VER_YEAR=${YEAR})

string(TIMESTAMP DATE "%m-%d")
add_definitions(-DBUILDDATE="${DATE}")

string(TIMESTAMP TIME "%H:%M")
add_definitions(-DBUILDTIME="${TIME}")


project(${TARGET_NAME} VERSION ${PROJECT_VERSION} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(QT_VERSION_MAJOR 5)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui Printsupport Network LinguistTools REQUIRED)

set(TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/languages/rezonator_ru.ts)

set(PROJECT_SOURCES
    ${TS_FILES}
)


add_subdirectory(src)


set(RESOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/src/app.rc
)

set(APP_ICON_RESOURCE_WINDOWS 
    "${CMAKE_CURRENT_SOURCE_DIR}/img/icon/main_2.icns"
)


if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(ReZonator
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET ReZonator APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(ReZonator SHARED
            ${SOURCE} ${HEADERS}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(ReZonator
            ${SOURCE} ${HEADERS}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

endif()

set_target_properties(ReZonator PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(ReZonator)
endif()
