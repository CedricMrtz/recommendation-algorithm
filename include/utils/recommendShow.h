#pragma once
#include <QWidget>
#include <QVector>
#include "show.h"
#include <QGridLayout>
#include <QSet>
#include <QString>

class recommendShow : public QWidget {
    Q_OBJECT
public:
    explicit recommendShow(QWidget *parent = nullptr);

    void setRecommendations(const QVector<Show> &recs, const QSet<QString> &ratedMovies);

private:
    QGridLayout *grid;
};
