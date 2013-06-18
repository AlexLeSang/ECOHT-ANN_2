TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp

HEADERS += \
    Network.hpp \
    Utils.hpp \
    ActivationFunction.hpp

DEFINES += "FAKE_RAND"
