TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    Preprocessor.cpp

HEADERS += \
    Network.hpp \
    Utils.hpp \
    Preprocessor.hpp

#DEFINES += "FAKE_RAND"
