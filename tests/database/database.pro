QT += testlib sql
TARGET = tst_database
CONFIG += console c++17 warn_on
CONFIG -= app_bundle

# Пути для выходных файлов
DESTDIR = $$OUT_PWD/bin
OBJECTS_DIR = $$OUT_PWD/obj/tests/database
MOC_DIR = $$OUT_PWD/moc

# Исходники тестов
SOURCES += \
    main.cpp \
    test_author.cpp

HEADERS += \
    test_author.h

# Подключаем тестируемый класс
INCLUDEPATH += $$PWD/../../src
SOURCES += $$PWD/../../src/database/models/author.cpp
HEADERS += $$PWD/../../src/database/models/author.h

# Автоматическая генерация moc-файлов
CONFIG += automoc

# Для MacOS
macx {
    QMAKE_INFO_PLIST = Info_test.plist
}

# Очистка тестовой БД после выполнения
QMAKE_POST_LINK += $$QMAKE_DEL_FILE $$DESTDIR/test_author.db
