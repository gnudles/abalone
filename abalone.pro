TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lfann -lreadline

SOURCES += main.cpp \
    BoardPrinterConsole.cpp \
    HexDefaultPlacer.cpp \
    MoveRecorder.cpp

HEADERS += \
    AbaloneBoard.h \
    BoardPrinterConsole.h \
    IAbaloneBoard.h \
    HexDefaultPlacer.h \
    IPlacer.h \
    terminal_colors.h \
    MoveRecorder.h
