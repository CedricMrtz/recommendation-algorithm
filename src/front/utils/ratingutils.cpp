#include "ratingutils.h"
#include <QFile>
#include <QTextStream>
#include <QDataStream>
#include <QFileInfo>
#include <QDebug>

static bool saveRatingsToBinary(const QString &binPath, const KaggleRatings &ratings)
{
    QFile file(binPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to open" << binPath << "for write:" << file.errorString();
        return false;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    qint32 userCount = ratings.size();
    out << userCount;

    for (auto it = ratings.constBegin(); it != ratings.constEnd(); ++it) {
        qint32 userId = it.key();
        const auto &userMap = it.value();

        out << userId;
        qint32 count = userMap.size();
        out << count;
        for (auto it2 = userMap.constBegin(); it2 != userMap.constEnd(); ++it2) {
            qint32 animeId = it2.key();
            qint32 rating  = it2.value();
            out << animeId << rating;
        }
    }

    return true;
}

static bool loadRatingsFromBinary(const QString &binPath, KaggleRatings &ratings)
{
    QFile file(binPath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Failed to open" << binPath << "for read:" << file.errorString();
        return false;
    }

    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_6_10);

    qint32 userCount = 0;
    in >> userCount;
    if (userCount < 0) {
        qWarning() << "Invalid userCount in" << binPath;
        return false;
    }

    for (qint32 i = 0; i < userCount; ++i) {
        qint32 userId;
        in >> userId;

        qint32 count;
        in >> count;

        QHash<AnimeId, RatingValue> userMap;
        for (qint32 j = 0; j < count; ++j) {
            qint32 animeId;
            qint32 rating;
            in >> animeId >> rating;
            userMap[animeId] = rating;
        }
        ratings[userId] = userMap;
    }

    return true;
}

bool loadKaggleRatingsWithCache(const QString &csvPath, const QString &binPath, KaggleRatings &out)
{
    QFileInfo binInfo(binPath);

    if (binInfo.exists() && binInfo.size() > 0) {
        qDebug() << "abriendo ratings desde bin:" << binPath;
        return loadRatingsFromBinary(binPath, out);
    }

    qDebug() << "viendo el csv:" << csvPath;

    QFile csv(csvPath);
    if (!csv.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open" << csvPath << "for read:" << csv.errorString();
        return false;
    }

    QTextStream in(&csv);
    if (!in.atEnd())
        in.readLine();

    qint64 counter = 0;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty())
            continue;

        const auto parts = line.split(',');
        if (parts.size() < 3)
            continue;

        bool ok1 = false, ok2 = false, ok3 = false;
        int userId  = parts[0].toInt(&ok1);
        int animeId = parts[1].toInt(&ok2);
        int rating  = parts[2].toInt(&ok3);

        if (!ok1 || !ok2 || !ok3)
            continue;

        out[userId][animeId] = rating;
    }

    qDebug() << "Filas leidas:" << counter;
    csv.close();

    if (!saveRatingsToBinary(binPath, out)) {
        qWarning() << "Failed to save ratings binary cache";
    } else {
        qDebug() << "Ratings binary cache created at:" << binPath;
    }

    return true;
}
