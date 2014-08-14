project(konnectionmonitor)
cmake_minimum_required(VERSION 2.6)
# Find the required Libaries
find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)

# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE(${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})

include(SourceFiles.cmake)

include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${Qt4_INCLUDES}
   )

qt4_wrap_cpp(konnectionmonitor_HEADERS_MOC ${konnectionmonitor_HEADERS})
qt4_wrap_ui(konnectionmonitor_SOURCES forms/konnectionmonitor.ui)

add_executable(konnectionmonitor ${konnectionmonitor_SOURCES} ${konnectionmonitor_HEADERS_MOC})
target_link_libraries(konnectionmonitor
	${QT_LIBRARIES}
)

install(TARGETS konnectionmonitor
        DESTINATION bin)

install(FILES konnectionmonitor.desktop
        DESTINATION share/applications)
install(FILES   konnectionmonitor16.png   DESTINATION "share/icons/hicolor/16x16/apps" RENAME "konnectionmonitor.png")
install(FILES   konnectionmonitor22.png   DESTINATION "share/icons/hicolor/22x22/apps" RENAME "konnectionmonitor.png")
install(FILES   konnectionmonitor32.png   DESTINATION "share/icons/hicolor/32x32/apps" RENAME "konnectionmonitor.png")
install(FILES   konnectionmonitor48.png   DESTINATION "share/icons/hicolor/48x48/apps" RENAME "konnectionmonitor.png")
install(FILES   konnectionmonitor64.png   DESTINATION "share/icons/hicolor/64x64/apps" RENAME "konnectionmonitor.png")
install(FILES   konnectionmonitor128.png   DESTINATION "share/icons/hicolor/128x128/apps" RENAME "konnectionmonitor.png")

#kde4_install_icons( ${ICON_INSTALL_DIR} )
