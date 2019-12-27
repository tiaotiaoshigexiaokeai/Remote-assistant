QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RemoteAssistance
TEMPLATE = app
CONFIG += c++11 link_pkgconfig
PKGCONFIG += dtkwidget

SOURCES += \
        main.cpp \
    assistassistant.cpp

RESOURCES +=         resources.qrc

HEADERS += \
    assistassistant.h

DISTFILES += \
    images/hand.png
