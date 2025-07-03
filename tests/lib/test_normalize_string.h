#ifndef TEST_NORMALIZE_STRING_H
#define TEST_NORMALIZE_STRING_H

#include <QtTest>
#include "../../src/lib/field.h" // Предполагается, что функция находится здесь

class TestNormalizeString : public QObject
{
    Q_OBJECT

private slots:
    void test_emptyString();
    void test_singleWord();
    void test_multipleWords();
    void test_leadingTrailingSpaces();
    void test_multipleSpacesBetweenWords();
    void test_mixedCase();
    void test_allUpperCase();
    void test_allLowerCase();
    void test_singleLetterWords();
};
#endif // TEST_NORMALIZE_STRING_H
