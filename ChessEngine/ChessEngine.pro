QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    entry/main.cpp \
    entry/serverentry.cpp \
    entry/ucci.cpp \
    utils/randomgenerator.cpp \
    genmoves.cpp \
    hash/zobristkey.cpp \
    position.cpp \
    pregen.cpp \
    search.cpp \

HEADERS += \
    entry/serverentry.h \
    entry/ucci.h \
    utils/randomgenerator.h \
    hash/zobristkey.h \
    position.h \
    pregen.h \
    search.h \
INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
