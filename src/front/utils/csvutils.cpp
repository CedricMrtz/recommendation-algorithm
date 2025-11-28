#include "csvutils.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>
#include <QFileInfo>

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

QVector<Show> readShowsFromCsv(const QString &path) {
    QVector<Show> shows;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open file:" << path << "-" << file.errorString();
        return {};
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

        shows.append(s);
    }

return shows;
}

static bool saveShowsToBinary(const QString &binPath, const QVector<Show> &shows) {
    QFile file(binPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open bin for write:" << binPath << "-" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10); 

    qint32 count = shows.size();
    out << count;
    for (const Show &s : shows) {
        out << s.anime_id
            << s.name
            << s.genre
            << s.type
            << s.episodes
            << s.rating
            << s.members;
    }

    return true;
}

static QVector<Show> readShowsFromBinary(const QString &binPath) {
    QVector<Show> shows;

    QFile file(binPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open bin for read:" << binPath << "-" << file.errorString();
        return {};
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_5);

    qint32 count = 0;
    in >> count;
    if (count < 0) {
        qWarning() << "Invalid count in bin";
        return {};
    }

    shows.reserve(count);
    for (qint32 i = 0; i < count; ++i) {
        Show s;
        in >> s.anime_id
           >> s.name
           >> s.genre
           >> s.type
           >> s.episodes
           >> s.rating
           >> s.members;
        shows.append(s);
    }

    return shows;
}


QVector<Show> readShowsWithCache(const QString &csvPath, const QString &binPath) {
    QFileInfo binInfo(binPath);

    if (binInfo.exists() && binInfo.size() > 0) {
        qDebug() << "abriendo bin (anime):" << binPath;
        return readShowsFromBinary(binPath);
    }

    qDebug() << "leyendo csv para crear bin:" << csvPath;
    QVector<Show> shows = readShowsFromCsv(csvPath);

    if (!shows.isEmpty()) {
        if (saveShowsToBinary(binPath, shows)) {
            qDebug() << "Bin created at:" << binPath;
        } else {
            qWarning() << "Failed to create bin";
        }
    }

    return shows;
}

