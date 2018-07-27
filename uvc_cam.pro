TEMPLATE = app

linux-g++: OS_SUFFIX = linux


CONFIG += console c++14 strict_c++
CONFIG -= app_bundle


linux-g++: QMAKE_CXXFLAGS += -c -Wall -Wextra --pedantic -std=c++14 -O3 -g3

CONFIG += link_pkgconfig
PKGCONFIG += opencv x11


SOURCES += main.cpp \
    uvc_capturer.cpp

INCLUDEPATH += /usr/include/spinnaker

LIBS += \
    -lpthread \
    -lboost_system \
    -lSpinnaker \
    -lstdc++fs \
    -lueye_api

SUBDIRS += \
    uvc_cam.pro

DISTFILES += \
    uvc_cam.pro.user

HEADERS += \
    uvc_capturer.h
