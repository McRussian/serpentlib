QT += core gui sql widgets

TARGET = SerpentLib
CONFIG += c++17

SOURCES += \
    database/database.cpp \
    database/models/author.cpp \
    gui/author_form.cpp \
    main.cpp \
    gui/serpentlibwindow.cpp

HEADERS += \
    database/database.h \
    database/models/author.h \
    gui/author_form.h \
    gui/serpentlibwindow.h \
    lib/field.h

FORMS += \
    gui/ui/author_form.ui \
    gui/ui/serpentlibwindow.ui
