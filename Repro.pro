QT       += core gui
QT       += multimedia multimediawidgets
QT       += multimediawidgets
QT       += core concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    PlayerVLC.cpp \
    archivo.cpp \
    dialog.cpp \
    hilo_ajustar.cpp \
    hilo_tiempo.cpp \
    hilos.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    PlayerVLC.h \
    archivo.h \
    dialog.h \
    hilo_ajustar.h \
    hilo_tiempo.h \
    hilos.h \
    mainwindow.h

FORMS += \
    dialog.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




    INCLUDEPATH += $$OUT_PWD/include
    LIBS += -L$$PWD/API/VLC
    LIBS += -laxvlc -llibvlc -llibvlccore -lnpvlc



RESOURCES += \
    Resources.qrc
