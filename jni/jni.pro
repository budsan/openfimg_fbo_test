#-------------------------------------------------
#
# Project created by QtCreator 2011-07-11T09:41:31
#
#-------------------------------------------------

QT       -= core gui

TARGET = jni
TEMPLATE = lib

DEFINES += JNI_LIBRARY

SOURCES += \
    importgl.c \
    demo.c \
    app-win32.c \
    app-linux.c \
    app-android.c \
    awesome.c \
    rawtexture.c

HEADERS += \
    importgl.h \
    app.h \
    rawtexture.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE22CF75A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = jni.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
