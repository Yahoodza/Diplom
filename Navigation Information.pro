QT       += core gui

win32:RC_ICONS += Satellite.ico

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    MainWindow.cpp \
    graphdialog.cpp \
    main.cpp \
    qcustomplot.cpp \
    setupdialog.cpp

HEADERS += \
    MainWindow.h \
    Structs.h \
    graphdialog.h \
    qcustomplot.h \
    setupdialog.h

FORMS += \
    MainWindow.ui \
    graphdialog.ui \
    setupdialog.ui


QMAKE_LFLAGS_RELEASE += -static -static-libgcc  #Для статической сборки проекта

RESOURCES += \
    resource.qrc
