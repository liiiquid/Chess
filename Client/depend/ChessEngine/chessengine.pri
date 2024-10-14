INCLUDEPATH += $$PWD
DEPENDPATH += $$PWD

include(utils\utils.pri)

SOURCES += \
    $$PWD/genmoves.cpp \
    $$PWD/hash/zobristkey.cpp \
    $$PWD/position.cpp \
    $$PWD/pregen.cpp \
    $$PWD/search.cpp \
    $$PWD/serverentry.cpp \
    $$PWD/ucci.cpp

HEADERS += \
    $$PWD/hash/zobristkey.h \
    $$PWD/position.h \
    $$PWD/pregen.h \
    $$PWD/search.h \
    $$PWD/serverentry.h \
    $$PWD/ucci.h
