#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include "./components/navbar.h"
#include "components/showCard.h"
#include "utils/csvutils.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QWidget window;
    window.setWindowTitle("Recommend");
    QVBoxLayout *layout = new QVBoxLayout(&window);

    navBar *navbar = new navBar();
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
    QVector<Show> shows = readShowsFromCsv("src/front/data/anime.csv");

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
