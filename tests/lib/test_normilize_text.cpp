#include "test_normilize_text.h"

#include "../../src/lib/field.h" // Предполагается, что функция находится здесь

void TestNormalizeText::testRussianText()
{
    QStringList input = {
        "  ПРИВЕТ,  МИР!  Как ДЕЛА?  ",
        "  ЭТО   ТЕСТОВЫЙ   пример...  "
    };

    QString expected =
        "Привет мир как дела\n"
        "Это тестовый пример";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testEnglishText()
{
    QStringList input = {
        "  HELLO,  WORLD!  How ARE YOU?  ",
        "  THIS is a TEST...  "
    };

    QString expected =
        "Hello world how are you\n"
        "This is a test";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testMixedText()
{
    QStringList input = {
        "  HELLO, МИР!  This is СМЕШАННЫЙ текст  ",
        "  Test Тест 123  " // Цифры должны быть удалены
    };

    QString expected =
        "Hello мир this is смешанный текст\n"
        "Test тест 123";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testPunctuation()
{
    QStringList input = {
        "  Что? Нет! Может...  ",
        "  \"Цитаты\", (скобки), запятые,  "
    };

    QString expected =
        "Что нет может\n"
        "Цитаты скобки запятые";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testEmptyLines()
{
    QStringList input = {
        "",
        "  Только эта строка должна остаться  ",
        "    ",
        "  и эта тоже  "
    };

    QString expected =
        "Только эта строка должна остаться\n"
        "И эта тоже";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testCaseNormalization()
{
    QStringList input = {
        "  FIRST WORD  ",
        "  Another Line  ",
        "  ALL CAPS LINE  "
    };

    QString expected =
        "First word\n"
        "Another line\n"
        "All caps line";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testHyphenAndApostrophe()
{
    QStringList input = {
        "  user-FRIENDLY interface  ",
        "  O'CONNOR surname  "
    };

    QString expected =
        "User-friendly interface\n"
        "O'connor surname";

    QCOMPARE(normalizeText(input), expected);
}

void TestNormalizeText::testSingleWordLines()
{
    QStringList input = {
        "  ОДНОСЛОВНАЯ  ",
        "  single  ",
        "  "
    };

    QString expected =
        "Однословная\n"
        "Single";

    QCOMPARE(normalizeText(input), expected);
}

