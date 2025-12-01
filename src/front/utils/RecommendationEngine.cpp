#include "RecommendationEngine.h"
#include <QtMath>
#include <QDebug>
#include "candidate.h"
#include "recommendShow.h"

int partition(QVector<Candidate> &arr, int low, int high) {
    double pivot = arr[high].score;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].score > pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(QVector<Candidate> &rec, int low, int high) {
    if (low < high) {
        int pi = partition(rec, low, high);
        quickSort(rec, low, pi - 1);
        quickSort(rec, pi + 1, high);
    }
}

RecommendationEngine::RecommendationEngine(
    const QHash<QString, QHash<int, double>>& userRatings,
    const QHash<int, Show>& showsById,
    int maxNeighbors,
    int maxResults
) : m_userRatings(userRatings), m_showsById(showsById), m_maxNeighbors(maxNeighbors), m_maxResults(maxResults){}

double RecommendationEngine::cosineSimilarity(const QString& userA, const QString& userB) const
{
    auto itA = m_userRatings.find(userA);
    auto itB = m_userRatings.find(userB);
    if (itA == m_userRatings.end() || itB == m_userRatings.end())
        return 0.0;

    const auto& ratingsA = itA.value();
    const auto& ratingsB = itB.value();
    double dot = 0.0;
    double normA = 0.0;
    double normB = 0.0;

    for (auto it = ratingsA.constBegin(); it != ratingsA.constEnd(); ++it) {
        int animeId = it.key();
        double ratingA = it.value();
        auto itBAnime = ratingsB.find(animeId);
        if (itBAnime != ratingsB.end()) {
            double ratingB = itBAnime.value();
            dot   += ratingA * ratingB;
            normA += ratingA * ratingA;
            normB += ratingB * ratingB;
        }
    }

    if (normA == 0.0 || normB == 0.0)
        return 0.0;
    return dot / (qSqrt(normA) * qSqrt(normB));
}

QVector<Show> RecommendationEngine::recommendForUser(const QString& userId) const
{
    QVector<Show> empty;

    if (!m_userRatings.contains(userId))
        return empty;

    const auto& targetRatings = m_userRatings[userId];

    struct Neighbor {
        QString userId;
        double similarity;
    };

    QVector<Neighbor> neighbors;
    neighbors.reserve(m_userRatings.size() - 1);

    for (auto it = m_userRatings.constBegin(); it != m_userRatings.constEnd(); ++it) {
        const QString& otherUser = it.key();
        if (otherUser == userId)
            continue;

        double sim = cosineSimilarity(userId, otherUser);
        if (sim > 0.0)
            neighbors.push_back({ otherUser, sim });
    }
    
    if (neighbors.size() > m_maxNeighbors)
        neighbors.resize(m_maxNeighbors);

    QHash<int, double> numerators;
    QHash<int, double> denominators;

    for (const Neighbor& n : neighbors) {
        const auto& nRatings = m_userRatings[n.userId];

        for (auto it = nRatings.constBegin(); it != nRatings.constEnd(); ++it) {
            int animeId = it.key();
            double rating = it.value();

            if (targetRatings.contains(animeId))
                continue;
            numerators[animeId]   += n.similarity * rating;
            denominators[animeId] += qAbs(n.similarity);
        }
    }

    QVector<Candidate> candidates;
    candidates.reserve(numerators.size());

    for (auto it = numerators.constBegin(); it != numerators.constEnd(); ++it) {
        int animeId = it.key();
        double num = it.value();
        double denom = denominators.value(animeId);
        if (denom <= 0.0)
            continue;

        candidates.push_back({ animeId, num / denom });
    }

    quickSort(candidates, 0, candidates.size()-1);

    if (candidates.size() > m_maxResults)
        candidates.resize(m_maxResults);

    qDebug() << " Recomendaciones para usuario:" << userId;
    for (const Candidate &c : candidates) {
        auto itShow = m_showsById.find(c.animeId);
        QString title = (itShow != m_showsById.end()) ? itShow->name : QStringLiteral("UNKNOWN");
        qDebug() << "AnimeID:" << c.animeId
                 << ", score:" << c.score
                 << ", title:" << title;
    }
    qDebug() << " Fin recomendaciones para usuario:" << userId;

    QVector<Show> result;
    result.reserve(candidates.size());

    for (const Candidate& c : candidates) {
        auto itShow = m_showsById.find(c.animeId);
        if (itShow != m_showsById.end())
            result.push_back(itShow.value());
    }

    return result;
}