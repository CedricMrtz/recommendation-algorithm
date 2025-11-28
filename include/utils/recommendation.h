#pragma once
#include <QWidget>
#include "show.h"

class RecommendationEngine {
public:
    QVector<Show> recommendForUser(const QString& user);
};
// Recommendation logic