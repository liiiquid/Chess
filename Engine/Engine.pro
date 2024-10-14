QT += quick quickcontrols2
QT += network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        entry/serverentry.cpp \
        entry/ucci.cpp \
        evaluate.cpp \
        genmoves.cpp \
        hash/hash.cpp \
        hash/zobristkey.cpp \
        main.cpp \
        movegen.cpp \
        position.cpp \
        preeval.cpp \
        pregen.cpp \
        search.cpp \
        utils/randomgenerator.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    ../Client/tcpsocket.h \
    entry/serverentry.h \
    entry/ucci.h \
    hash/hash.h \
    hash/zobristkey.h \
    movegen.h \
    position.h \
    pregen.h \
    search.h \
    utils/randomgenerator.h \
    utils/utils.h \
    x86asm.h
