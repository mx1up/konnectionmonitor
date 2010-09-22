TEMPLATE = app
TARGET = KonnectionMonitor
HEADERS += ConnectionListProvider.h \
    ConnectionListModel.h \
    Connection.h \
    konnectionmonitor.h \
 hostnameresolver.h
SOURCES += ConnectionListProvider.cpp \
    ConnectionListModel.cpp \
    Connection.cpp \
    main.cpp \
    konnectionmonitor.cpp \
 hostnameresolver.cpp
FORMS += konnectionmonitor.ui
QT += gui \
    core \
    network
CONFIG += console
