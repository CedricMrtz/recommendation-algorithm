#pragma once
#include <QWidget>
#include <QVector>
#include "show.h"
#include <QGridLayout>

class recommendShow : public QWidget {
    Q_OBJECT
public:
    explicit recommendShow(QWidget *parent = nullptr);

    void setRecommendations(const QVector<Show> &recs);

private:
    QGridLayout *grid;
};
