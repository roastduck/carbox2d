TEMPLATE = app
QT += widgets opengl
HEADERS = \
    world.h \
    track.h \
    mainwindow.h \
    contactlistener.h \
    car.h \
    algorithm.h \
    externalalgorithm.h \
    geneticalgorithm.h \
    render.h

SOURCES = \
    main.cpp \
    world.cpp \
    track.cpp \
    mainwindow.cpp \
    contactlistener.cpp \
    car.cpp \
    algorithm.cpp \
    externalalgorithm.cpp \
    geneticalgorithm.cpp \
    render.cpp

CONFIG += c++14

LIBS += -L$$PWD/lib/ -lBox2D

INCLUDEPATH += $$PWD/Box2D
DEPENDPATH += $$PWD/Box2D
