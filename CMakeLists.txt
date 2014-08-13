project(konnectionmonitor)
cmake_minimum_required(VERSION 2.6)
# Find the required Libaries
find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)

# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE(${QT_USE_FILE})
add_definitions (${QT_DEFINITIONS})
include_directories(
   ${CMAKE_SOURCE_DIR}
   ${CMAKE_BINARY_DIR}
   ${Qt4_INCLUDES}
   )

set(konnectionmonitor_SRCS
	config.cpp
	Connection.cpp
	ConnectionListModel.cpp
	ConnectionListProvider.cpp
	hostnameresolver.cpp
	konnectionmonitor.cpp
        aboutwidget.cpp
	main.cpp
)

set(konnectionmonitor_HEADERS
	konnectionmonitor.h
	hostnameresolver.h
	ConnectionListModel.h
        aboutwidget.h
)
qt4_wrap_cpp(konnectionmonitor_HEADERS_MOC ${konnectionmonitor_HEADERS})
qt4_wrap_ui(konnectionmonitor_SRCS konnectionmonitor.ui aboutwidget.ui)

add_executable(konnectionmonitor ${konnectionmonitor_SRCS} ${konnectionmonitor_HEADERS_MOC})
target_link_libraries(konnectionmonitor
	${QT_LIBRARIES}
)

install(TARGETS konnectionmonitor
        DESTINATION bin)

install(FILES konnectionmonitor.desktop
        DESTINATION share/applications)
#kde4_install_icons( ${ICON_INSTALL_DIR} )
