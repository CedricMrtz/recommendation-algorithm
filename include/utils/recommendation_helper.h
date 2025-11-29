#pragma once
#include <QVector>
#include <QHash>
#include <QString>
#include "show.h"
#include "ratingutils.h" 
#include "userManager.h"
#include "RecommendationEngine.h"

QVector<Show> createRecommendations(
    const QString& appUserName,
    const UserManager& userManager,
    const KaggleRatings& kaggleRatings,
    const QVector<Show>& shows
);
