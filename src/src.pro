QT += core gui sql widgets

TARGET = SerpentLib
CONFIG += c++17

# Явно указываем линковку с SQLite3
unix {
    # Для Linux и macOS
    # LIBS += -lsqlite3
    # Если библиотека в нестандартном месте:
    LIBS += -L/usr/lib64 -lsqlite3
    INCLUDEPATH += /usr/include
}

win32 {
    # Для Windows
    LIBS += -lsqlite3
    # Или если используете свою сборку:
    # LIBS += -L$$PWD/../sqlite3 -lsqlite3
    # INCLUDEPATH += $$PWD/../sqlite3/include
}

SOURCES += \
    database/database.cpp \
    database/models/author.cpp \
    gui/author_form.cpp \
    gui/author_selection_widget.cpp \
    main.cpp \
    gui/serpentlibwindow.cpp \
    settings_application.cpp

HEADERS += \
    database/database.h \
    database/dbexception.h \
    database/models/author.h \
    database/models/base_model.h \
    gui/author_form.h \
    gui/author_selection_widget.h \
    gui/serpentlibwindow.h \
    lib/field.h \
    settings_application.h

FORMS += \
    gui/ui/author_form.ui \
    gui/ui/serpentlibwindow.ui
