#include "field.h"

QString normalize(const QString &input)
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
