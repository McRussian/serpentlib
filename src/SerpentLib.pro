QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../tests/database/main.cpp \
    ../tests/database/test_author.cpp \
    database/database.cpp \
    database/models/author.cpp \
    main.cpp \
    gui/serpentlibwindow.cpp

HEADERS += \
    ../tests/database/test_author.h \
    database/database.h \
    database/models/author.h \
    gui/serpentlibwindow.h

FORMS += \
    gui/ui/serpentlibwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    ../tests/database/database.pro
