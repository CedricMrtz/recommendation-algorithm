#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>

#include "navbar.h"
#include "showCard.h"
#include "csvutils.h"
#include "userManager.h"
#include "ratingutils.h"
#include "recommendShow.h"
#include <QHash>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KaggleRatings kaggleRatings;
    loadKaggleRatingsWithCache(":/front/data/rating.csv", "ratings.bin", kaggleRatings);

    QVector<Show> shows = readShowsWithCache(":/front/data/anime.csv", "anime.bin");

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

    navbar->grid = grid;  

    grid->setHorizontalSpacing(20);
    grid->setVerticalSpacing(25);
    grid->setAlignment(Qt::AlignTop);
    scroll->setWidget(container);
    layout->addWidget(scroll);

    QHash<QString, Show> moviesByName;
    for (const Show &s : shows) {
        moviesByName.insert(s.name, s);
    }
    navbar->movies = moviesByName;

    QVector<Show> showsVec = shows;
    recommendShow::sortRecommendations(showsVec);  

    int row = 0;
    int col = 0;
    for (int i = 0; i < showsVec.size(); ++i) {
        ShowCard *card = new ShowCard(showsVec[i]);
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
