QT += testlib sql

TARGET = tst_database
CONFIG += console c++17 warn_on
CONFIG -= app_bundle

# Включить автоматическую генерацию moc
CONFIG += automoc

# Исходники тестов
SOURCES += \
    test_author.cpp \
    main.cpp

HEADERS += \
    test_author.h

# Правильные пути к исходникам основного проекта
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

# Подключаемые классы
SOURCES += $$PWD/../../src/database/models/author.cpp
HEADERS += $$PWD/../../src/database/models/author.h
SOURCES += $$PWD/../../src/database/database.cpp
HEADERS += $$PWD/../../src/database/database.h

# Для тестов определяем флаг
DEFINES += QT_TESTLIB_LIB

# Для временной БД в памяти
DEFINES += IN_MEMORY_DB

# Очистка тестовой БД после выполнения
QMAKE_POST_LINK += $$QMAKE_DEL_FILE $$DESTDIR/test_author.db
