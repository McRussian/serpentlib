#ifndef TEST_NORMILIZE_TEXT_H
#define TEST_NORMILIZE_TEXT_H

#include <QtTest>
#include <QStringList>

class TestNormalizeText : public QObject
{
    Q_OBJECT

private slots:
    // Тестовые случаи
    void testRussianText();
    void testEnglishText();
    void testMixedText();
    void testPunctuation();
    void testEmptyLines();
    void testCaseNormalization();
    void testHyphenAndApostrophe();
    void testSingleWordLines();
};

#endif // TEST_NORMILIZE_TEXT_H
