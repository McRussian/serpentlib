#include "test_normalize_string.h"
#include "../../src/lib/field.h" // Предполагается, что функция находится здесь

void TestNormalizeName::test_emptyString()
{
    QString input = "";
    QString expected = "";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_singleWord()
{
    QString input = "hello";
    QString expected = "Hello";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_multipleWords()
{
    QString input = "hello world";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_leadingTrailingSpaces()
{
    QString input = "  hello world  ";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_multipleSpacesBetweenWords()
{
    QString input = "hello   world";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_mixedCase()
{
    QString input = "hElLo WoRlD";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_allUpperCase()
{
    QString input = "HELLO WORLD";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_allLowerCase()
{
    QString input = "hello world";
    QString expected = "Hello World";
    QCOMPARE(normalizeName(input), expected);
}

void TestNormalizeName::test_singleLetterWords()
{
    QString input = "a b c d";
    QString expected = "A B C D";
    QCOMPARE(normalizeName(input), expected);
}
