#ifndef FIELD_H
#define FIELD_H

#include <QString>
#include <QRegularExpression>

inline QString normalizeName(const QString &input)
{
    if (input.isEmpty())
        return input;

    // Удаляем начальные и конечные пробелы
    QString normalized = input.trimmed();

    // Заменяем множественные пробелы на одинарные
    normalized.replace(QRegularExpression("\\s+"), " ");

    // Разбиваем на слова
    QStringList words = normalized.split(' ', Qt::SkipEmptyParts);

    // Обрабатываем каждое слово
    for (int i = 0; i < words.size(); ++i) {
        if (!words[i].isEmpty()) {
            // Первая буква - заглавная, остальные - строчные
            words[i] = words[i].at(0).toUpper() +
                       words[i].mid(1).toLower();
        }
    }

    // Собираем обратно в строку
    return words.join(' ');
}

inline QString normalizeText(const QStringList& inputLines)
{
    QStringList normalizedLines;

    // Регулярное выражение для сохранения букв (русских и английских), апострофов и дефисов
    QRegularExpression keepChars("[^\\w\\sа-яА-ЯёЁa-zA-Z'-]");

    for (const QString& line : inputLines) {
        if (line.trimmed().isEmpty()) {
            continue; // Пропускаем пустые строки
        }

        // 1. Удаляем нежелательные символы (сохраняем буквы, пробелы, апострофы и дефисы)
        QString cleaned = line;
        cleaned.remove(keepChars);

        // 2. Удаляем лишние пробелы и нормализуем оставшиеся
        cleaned = cleaned.simplified();

        // 3. Разбиваем на слова
        QStringList words = cleaned.split(' ', Qt::SkipEmptyParts);

        if (words.isEmpty()) {
            continue;
        }

        // 4. Обрабатываем первое слово - первая буква заглавная, остальные строчные
        if (!words.first().isEmpty()) {
            QString firstWord = words.first().toLower();
            if (!firstWord.isEmpty()) {
                // Определяем, является ли первый символ буквой (русской или английской)
                QChar firstChar = firstWord[0];
                if (firstChar.isLetter()) {
                    firstWord[0] = firstChar.toUpper();
                }
            }
            words[0] = firstWord;
        }

        // 5. Остальные слова в нижний регистр
        for (int i = 1; i < words.size(); ++i) {
            words[i] = words[i].toLower();
        }

        // 6. Собираем строку обратно
        normalizedLines << words.join(' ');
    }

    // 7. Объединяем все строки через \n
    return normalizedLines.join('\n');
}

#endif // FIELD_H
