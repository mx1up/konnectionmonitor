cmake_minimum_required(VERSION 3.1.0)
project(konnectionmonitor VERSION 0.3.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

include(SourceFiles.cmake)

find_package(Qt5 COMPONENTS Core Widgets Network REQUIRED)

add_custom_target(useful_files ALL
    SOURCES
      CHANGELOG
      README
      konnectionmonitor.spec
)
add_executable(konnectionmonitor ${konnectionmonitor_SOURCES} ${konnectionmonitor_HEADERS} ${konnectionmonitor_FORMS} ${konnectionmonitor_RESOURCES} )

target_link_libraries(konnectionmonitor Qt5::Widgets Qt5::Network Qt5::Core)

install(TARGETS konnectionmonitor
        DESTINATION bin)

install(FILES konnectionmonitor.desktop
        DESTINATION share/applications)
install(FILES   res/konnectionmonitor16.png   DESTINATION "share/icons/hicolor/16x16/apps" RENAME "konnectionmonitor.png")
install(FILES   res/konnectionmonitor22.png   DESTINATION "share/icons/hicolor/22x22/apps" RENAME "konnectionmonitor.png")
install(FILES   res/konnectionmonitor32.png   DESTINATION "share/icons/hicolor/32x32/apps" RENAME "konnectionmonitor.png")
install(FILES   res/konnectionmonitor48.png   DESTINATION "share/icons/hicolor/48x48/apps" RENAME "konnectionmonitor.png")
install(FILES   res/konnectionmonitor64.png   DESTINATION "share/icons/hicolor/64x64/apps" RENAME "konnectionmonitor.png")
install(FILES   res/konnectionmonitor128.png   DESTINATION "share/icons/hicolor/128x128/apps" RENAME "konnectionmonitor.png")

#kde4_install_icons( ${ICON_INSTALL_DIR} )
