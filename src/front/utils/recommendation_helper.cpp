#include "recommendation_helper.h"

QVector<Show> createRecommendations( const QString& appUserName,
    const UserManager& userManager,
    const KaggleRatings& kaggleRatings,
    const QVector<Show>& shows
){
    QHash<int, Show> showsById;
    for (const Show& s : shows) {
        showsById.insert(s.anime_id, s);
    }

    QHash<QString, QHash<int, double>> engineRatings;

    for (auto it = kaggleRatings.constBegin(); it != kaggleRatings.constEnd(); ++it) {
        int kaggleUserId = it.key(); 
        QString engineUserId = QStringLiteral("k_%1").arg(kaggleUserId);

        QHash<int, double> userMap;
        const auto& originalMap = it.value();

        for (auto it2 = originalMap.constBegin(); it2 != originalMap.constEnd(); ++it2) {
            int animeId = it2.key();
            int rating  = it2.value();
            userMap.insert(animeId, static_cast<double>(rating));
        }

        engineRatings.insert(engineUserId, userMap);
    }

    QHash<QString, int> appUserRatings = userManager.getUserRatings(appUserName);
    QHash<int, double> activeUserMap;
    for (auto it = appUserRatings.constBegin(); it != appUserRatings.constEnd(); ++it) {
        const QString& movieKey = it.key();
        int rating = it.value();

        bool ok = false;
        int animeId = movieKey.toInt(&ok);
        if (!ok) {
            continue;
        }

        activeUserMap.insert(animeId, static_cast<double>(rating));
    }

    QString activeEngineId = QStringLiteral("u_%1").arg(appUserName);
    engineRatings.insert(activeEngineId, activeUserMap);

    RecommendationEngine engine(engineRatings, showsById);
    return engine.recommendForUser(activeEngineId);
}
