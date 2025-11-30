#pragma once
#include <QWidget>
#include <QVector>
#include <QString>

#include "show.h"
#include "userManager.h"
#include "ratingutils.h"

class QGridLayout;
class QComboBox;

class recommendShow : public QWidget {
    Q_OBJECT
public:
    explicit recommendShow(const QString &userName,
                           UserManager *userManager,
                           KaggleRatings *kaggleRatings,
                           const QVector<Show> &shows,
                           QWidget *parent = nullptr);

    static void sortRecommendations(QVector<Show> &recs);

    void setRecommendations(const QVector<Show> &recs, const QSet<QString> &ratedMovies);

private:
    QString m_userName;
    UserManager *m_userManager = nullptr;
    KaggleRatings *m_kaggleRatings = nullptr;
    QVector<Show> m_shows;

    QGridLayout *grid = nullptr;
    QComboBox *genreBox = nullptr;

    void refillRecommendations();
};
