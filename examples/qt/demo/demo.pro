QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    finclip-qt-demo.cpp

HEADERS += \
    finclip-qt-demo.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32 {
    contains(QMAKE_TARGET.arch, x86_64) {
        message("x86_64 build")
        ## Windows x64 (64bit) specific build here
        LIBS += "../../../vendor/win/x64/FinClipSDKWrapper.lib"
        INCLUDEPATH += "../../../src"
        QMAKE_POST_LINK=robocopy ../../../vendor/win/x64 . /E
    } else {
        message("x86 build")
        ## Windows x86 (32bit) specific build here
    }

}
