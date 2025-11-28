#include "recommendShow.h"
#include <QVBoxLayout>
#include <QLabel>

recommendShow::recommendShow(QWidget *parent)
    : QWidget(parent)
{
    auto *layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("Recommended movies will appear here."));
    setLayout(layout);
}

void recommendShow::setRecommendations(const QVector<Show> &recs) {
    // display them
}
