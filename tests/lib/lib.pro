QT += testlib

TARGET = tst_lib
CONFIG += console c++17 warn_on
CONFIG -= app_bundle

# Включить автоматическую генерацию moc
CONFIG += automoc

# Исходники тестов
SOURCES += \
    main.cpp \
    test_normalize_string.cpp

HEADERS += \
    test_normalize_string.h

# Правильные пути к исходникам основного проекта
INCLUDEPATH += $$PWD/../../src
DEPENDPATH += $$PWD/../../src

# Подключаемые классы
HEADERS += $$PWD/../../src/lib/field.h

# Для тестов определяем флаг
DEFINES += QT_TESTLIB_LIB
