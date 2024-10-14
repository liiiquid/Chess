QT += quick quickcontrols2
QT += network
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(depend\frameless\frameless.pri)

SOURCES += \
        ./main.cpp \
        board/board.cpp \
        board/marka.cpp \
        board/piece.cpp

HEADERS += \
    board/board.h \
    board/marka.h \
    board/piece.h \
    global.h \
    tcpsocket.h

RESOURCES += qml.qrc \
    img.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
