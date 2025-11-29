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

int partition(QVector<Show> &arr, int low, int high) {
    double pivot = arr[high].rating; // sort by rating
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].rating > pivot) {  // highest first
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(QVector<Show> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}
void recommendShow::sortRecommendations(QVector<Show> &recs) {
    if (recs.size() > 1)
        quickSort(recs, 0, recs.size() - 1);    
}

void recommendShow::setRecommendations(const QVector<Show> &recs, const QSet<QString> &ratedMovies) {
    // display
    int row = 0, col = 0;
    for (const Show &s : recs) {
        if (ratedMovies.contains(s.name)) continue; // skip rated shows
        ShowCard *card = new ShowCard(s);
        grid->addWidget(card, row, col);
        col++;
        if (col == 3) {
            col = 0;
            row++;
        }
    }
}
