#pragma once

#include <QHash>
#include <QVector>
#include <QString>
#include "show.h"

class RecommendationEngine {
public:
    RecommendationEngine(
        const QHash<QString, QHash<int, double>>& userRatings,
        const QHash<int, Show>& showsById,
        int maxNeighbors = 10,
        int maxResults = 10
    );
    QVector<Show> recommendForUser(const QString& userId) const;

private:
    double cosineSimilarity(const QString& userA, const QString& userB) const;
    QHash<QString, QHash<int, double>> m_userRatings;
    QHash<int, Show> m_showsById;

    int m_maxNeighbors; 
    int m_maxResults;   
};
