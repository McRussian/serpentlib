#include "test_normalize_string.h"

void TestNormalizeString::test_emptyString()
{
    QString input = "";
    QString expected = "";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_singleWord()
{
    QString input = "hello";
    QString expected = "Hello";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_multipleWords()
{
    QString input = "hello world";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_leadingTrailingSpaces()
{
    QString input = "  hello world  ";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_multipleSpacesBetweenWords()
{
    QString input = "hello   world";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_mixedCase()
{
    QString input = "hElLo WoRlD";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_allUpperCase()
{
    QString input = "HELLO WORLD";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_allLowerCase()
{
    QString input = "hello world";
    QString expected = "Hello World";
    QCOMPARE(normalize(input), expected);
}

void TestNormalizeString::test_singleLetterWords()
{
    QString input = "a b c d";
    QString expected = "A B C D";
    QCOMPARE(normalize(input), expected);
}
