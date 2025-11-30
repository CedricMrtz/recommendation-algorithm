#include <QApplication>
#include <QPushButton>
#include <QMessageBox>
#include <QWidget>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>

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


    QHBoxLayout *recHeader = new QHBoxLayout();

    QLabel *recLabel = new QLabel("Recomendación", container);
    recLabel->setStyleSheet("font-size: 24px; font-weight: bold;");

    QComboBox *genreBox = new QComboBox(container);
    genreBox->addItem("Todos");
    genreBox->addItem("Action");
    genreBox->addItem("Comedy");
    genreBox->addItem("Romance");
    genreBox->addItem("Drama");
    genreBox->addItem("Slice of Life");
    genreBox->addItem("Adventure");

    recHeader->addWidget(recLabel);
    recHeader->addStretch();
    recHeader->addWidget(genreBox);

    containerLayout->addLayout(recHeader);

    QWidget *recWidget = new QWidget(container);
    QGridLayout *recGrid = new QGridLayout(recWidget);
    recGrid->setHorizontalSpacing(20);
    recGrid->setVerticalSpacing(25);
    recGrid->setAlignment(Qt::AlignTop);

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
    auto refillRecommendations = [&](const QString &userName) {
    clearGridLayout(recGrid);

    QVector<Show> recommended = createRecommendations(
        userName,
        manager,
        kaggleRatings,
        shows
    );

    QString selectedGenre = genreBox->currentText();
    int row = 0, col = 0;

    qDebug() << "Mostrando recomendaciones para usuario" << userName
             << "con género filtrado:" << selectedGenre;

    for (const Show &s : recommended) {
        if (selectedGenre != "Todos" &&
            !s.genre.contains(selectedGenre, Qt::CaseInsensitive))
            continue;

        qDebug() << "Shows :" << s.anime_id << ", " << s.name << ", generos:" << s.genre;

        ShowCard *card = new ShowCard(s);
        recGrid->addWidget(card, row, col);
        if (++col == 3) { col = 0; row++; }
    }
};

    auto refillCatalog = [&]() {
        clearGridLayout(catGrid);

        int row = 0, col = 0;
        for (const Show &s : shows) {
            ShowCard *card = new ShowCard(s);
            catGrid->addWidget(card, row, col);
            if (++col == 3) { col = 0; row++; }
        }
    };
    refillCatalog();
    refillRecommendations(currentUser);

    QObject::connect(genreBox, &QComboBox::currentTextChanged,
                     [&](const QString &) {
        refillRecommendations(currentUser);
    });


    QObject::connect(navbar, &navBar::refreshRecommendationsRequested,
        &window, [&](const QString &userName) {
            currentUser = userName;
            refillRecommendations(userName);   
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
