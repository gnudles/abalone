TEMPLATE = app
CONFIG += console c++1z
CONFIG -= app_bundle
CONFIG -= qt
LIBS += -lfann -lreadline
DEFINES += ENABLE_AI ENABLE_XML

SOURCES += main.cpp \
    BoardPrinterConsole.cpp \
    MoveRecorder.cpp \
    AIPlayer.cpp \
    HumanConsolePlayer.cpp \
    DefaultPlacer.cpp \
    OffBoardPlacer.cpp \
    BelgianDaisyPlacer.cpp \
    BoardFactory.cpp

HEADERS += \
    AbaloneBoard.h \
    BoardPrinterConsole.h \
    IAbaloneBoard.h \
    IPlacer.h \
    terminal_colors.h \
    MoveRecorder.h \
    AIPlayer.h \
    HumanConsolePlayer.h \
    AbalonePlayer.h \
    DefaultPlacer.h \
    OffBoardPlacer.h \
    BelgianDaisyPlacer.h \
    BoardFactory.h

DISTFILES += \
    original_makeMove.txt

unix {
    CONFIG += link_pkgconfig
    PKGCONFIG += libxml++-2.6
}
