#include "csvutils.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>
#include <QFileInfo>
#include <QHash>
#include <QString>

static QStringList splitCsvLine(const QString &line) {
    QStringList fields;
    QString current;
    bool inQuotes = false;

    for (QChar c : line) {
        if (c == '"') {
            inQuotes = !inQuotes;
        } else if (c == ',' && !inQuotes) {
            fields.append(current.trimmed());
            current.clear();
        } else {
            current.append(c);
        }
    }
    fields.append(current.trimmed());
    return fields;
}

QHash<QString, Show> readShowsFromCsv(const QString &path) {
    QHash<QString, Show> shows;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << path << "-" << file.errorString();
        return shows;
    }

    QTextStream in(&file);

    if (!in.atEnd())
        in.readLine();
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        QStringList f = splitCsvLine(line);
        if (f.size() < 7)
            continue;

        Show s;
        s.anime_id = f[0].toInt();
        s.name     = f[1];
        s.genre    = f[2];  
        s.type     = f[3];
        s.episodes = f[4].toInt();
        s.rating   = f[5].toDouble();
        s.members  = f[6].toInt();

        shows.insert(s.name, s);
    }

return shows;
}