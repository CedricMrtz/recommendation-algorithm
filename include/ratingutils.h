#pragma once
#include <QHash>
#include <QString>

using UserId = int;
using AnimeId = int;
using RatingValue = int;


using KaggleRatings = QHash<UserId, QHash<AnimeId, RatingValue>>;

bool loadKaggleRatingsWithCache(const QString &csvPath, const QString &binPath, KaggleRatings &out);
