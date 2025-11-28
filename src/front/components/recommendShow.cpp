#include "recommendShow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include "showCard.h"

recommendShow::recommendShow(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);
    QScrollArea *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget();
    grid = new QGridLayout(container);
    grid->setAlignment(Qt::AlignTop);
    scroll->setWidget(container);
    layout->addWidget(scroll);
}

void recommendShow::setRecommendations(const QVector<Show> &recs) {
    // display
    int row = 0, col = 0;
    for (const Show &s : recs) {
        ShowCard *card = new ShowCard(s);
        grid->addWidget(card, row, col);
        col++;
        if (col == 3) {
            col = 0;
            row++;
        }
    }
}
