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
#include "recommendation_helper.h"
#include "RecommendationEngine.h"

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

    QLabel *recLabel = new QLabel("Recomendación", container);
    recLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 10px 0;");

    QWidget *recWidget = new QWidget(container);
    QGridLayout *recGrid = new QGridLayout(recWidget);
    recGrid->setHorizontalSpacing(20);
    recGrid->setVerticalSpacing(25);
    recGrid->setAlignment(Qt::AlignTop);

    containerLayout->addWidget(recLabel);
    containerLayout->addWidget(recWidget);

    QLabel *catLabel = new QLabel("Catálogo", container);
    catLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin: 20px 0 10px 0;");

    QWidget *catWidget = new QWidget(container);
    QGridLayout *catGrid = new QGridLayout(catWidget);
    catGrid->setHorizontalSpacing(20);
    catGrid->setVerticalSpacing(25);
    catGrid->setAlignment(Qt::AlignTop);

    containerLayout->addWidget(catLabel);
    containerLayout->addWidget(catWidget);

    scroll->setWidget(container);
    layout->addWidget(scroll);

    QString currentUser = "ruben";

    auto refillGridsForUser = [&](const QString &userName) {
        clearGridLayout(recGrid);
        clearGridLayout(catGrid);

        QVector<Show> recommended = createRecommendations(
            userName,
            manager,
            kaggleRatings,
            shows
        );

        int row = 0;
        int col = 0;
        for (int i = 0; i < recommended.size(); ++i) {
            ShowCard *card = new ShowCard(recommended[i]);
            recGrid->addWidget(card, row, col);
            col++;
            if (col == 3) {
                col = 0;
                row++;
            }
        }
        row = 0;
        col = 0;
        for (int i = 0; i < shows.size(); ++i) {
            ShowCard *card = new ShowCard(shows[i]);
            catGrid->addWidget(card, row, col);
            col++;
            if (col == 3) {
                col = 0;
                row++;
            }
        }
    };

    refillGridsForUser(currentUser);

    QObject::connect(navbar, &navBar::refreshRecommendationsRequested,
                     &window, [&](const QString &userName) {
        currentUser = userName;
        refillGridsForUser(userName);
    });

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
