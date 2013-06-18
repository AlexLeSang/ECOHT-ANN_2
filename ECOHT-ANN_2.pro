QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

debug {
    QT       += testlib
    CONFIG   += console
}

TARGET = ECOHT-ANN_2
TEMPLATE = app


QMAKE_CXXFLAGS += -std=c++0x

LIBS += -L/usr/lib -lqwt

INCLUDEPATH += /usr/include/qwt
DEPENDPATH += /usr/include/qwt

FORMS += \
    mainwindow.ui


SOURCES += main.cpp\
      mainwindow.cpp \
    Facade.cpp

HEADERS += \
    Utils.hpp \
    mainwindow.hpp \
    Facade.hpp \
    ANNetwork.hpp


OTHER_FILES += \
    data1.dat \
    data.dat \
    data2.dat \
    plotres.m \
    data4.dat \
    simple_data.dat \
    parabolic_data.dat \
    data7.dat \
    data6.dat
