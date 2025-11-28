#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include "navbar.h"
#include "showCard.h"
#include "csvutils.h"
#include "userManager.h"
#include "ratingutils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    KaggleRatings kaggleRatings;
    loadKaggleRatingsWithCache(":/front/data/rating.csv", "ratings.bin", kaggleRatings);

    QWidget window;
    window.setWindowTitle("Recommend");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    UserManager manager;
    navBar *navbar = new navBar(&manager, &kaggleRatings);
    layout->addWidget(navbar);

    QScrollArea *scroll = new QScrollArea(&window);
    scroll->setWidgetResizable(true);
    QWidget *container = new QWidget();
    QGridLayout *grid = new QGridLayout(container);
    grid->setHorizontalSpacing(20);
    grid->setVerticalSpacing(25);
    grid->setAlignment(Qt::AlignTop);
    scroll->setWidget(container);
    layout->addWidget(scroll);

    QVector<Show> shows = readShowsWithCache(":/front/data/anime.csv", "anime.bin");

    int row = 0;
    int col = 0;
    for (int i = 0; i < shows.size(); ++i) {
        ShowCard *card = new ShowCard(shows[i]);
        grid->addWidget(card, row, col);
        col++;
        if (col == 3) {
            col = 0;
            row++;
        }
    }

    window.showMaximized();
    return app.exec();
}
