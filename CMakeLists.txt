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
#kde4_install_icons( ${ICON_INSTALL_DIR} )
