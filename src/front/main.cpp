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

static void clearGridLayout(QGridLayout *layout);

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
    QVBoxLayout *containerLayout = new QVBoxLayout(container);

    QLabel *catLabel = new QLabel("Catálogo", container);
    catLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0 10px 0;");
    containerLayout->addWidget(catLabel);

    QWidget *catWidget = new QWidget(container);
    QGridLayout *catGrid = new QGridLayout(catWidget);
    catGrid->setHorizontalSpacing(20);
    catGrid->setVerticalSpacing(25);
    catGrid->setAlignment(Qt::AlignTop);
    containerLayout->addWidget(catWidget);

    scroll->setWidget(container);
    layout->addWidget(scroll);

    auto refillCatalog = [&]() {
        clearGridLayout(catGrid);

        int row = 0, col = 0;
        for (const Show &s : shows) {
            ShowCard *card = new ShowCard(s);
            catGrid->addWidget(card, row, col);
            if (++col == 3) {
                col = 0;
                ++row;
            }
        }
    };

    refillCatalog();

    window.showMaximized();
    return app.exec();
}

static void clearGridLayout(QGridLayout *layout) {
    if (!layout) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }
}
