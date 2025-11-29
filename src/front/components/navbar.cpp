#include "navbar.h"
#include "csvutils.h"
#include <QVector>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include <QMap>

navBar::navBar(UserManager *manager, KaggleRatings *ratings, QWidget *parent) : QWidget(parent),
      userManager(manager),
      kaggleRatings(ratings)
{
    setStyleSheet("background-color: #de2c2cff;");

    auto *layout = new QHBoxLayout(this);

    auto *profileButton = new QPushButton(this);
    profileButton->setIcon(QIcon(":/front/public/profile.svg"));

    auto *recommendButton = new QPushButton(this);
    recommendButton->setIcon(QIcon(":/front/public/search.svg"));

    auto *rateButton = new QPushButton(this);
    rateButton->setIcon(QIcon(":/front/public/rating.svg"));

    layout->addWidget(profileButton);
    layout->addWidget(recommendButton);
    layout->addWidget(rateButton);

    setLayout(layout);

    connect(recommendButton, &QPushButton::clicked, this, [=]() {
        if (userManager->users.isEmpty()) {
            QMessageBox::warning(this, "Error", "Primero registra un usuario.");
            return;
        }

        QStringList userNames = userManager->users.keys();
        QString currentUser = QInputDialog::getItem(this, "Usuario", "Selecciona usuario:", userNames, 0, false);
        if (currentUser.isEmpty()) return;

        QVector<Show> allShows = readShowsWithCache(":/front/data/anime.csv", "anime.bin");

        auto *screen = new recommendShow();
        QSet<QString> ratedMovies = userManager->ratedMovies(currentUser);
        screen->sortRecommendations(allShows);
        screen->setRecommendations(allShows, ratedMovies);
        screen->show();
    });

    connect(profileButton, &QPushButton::clicked, this, [=]() {
        QString name = QInputDialog::getText(this, "Registrar usuario", "Nombre del usuario:");

        if (name.isEmpty()) return;
        if (!userManager->registerUser(name)) {
            QMessageBox::warning(this, "Error", "Ese nombre ya existe.");
        } else {
            QMessageBox::information(this, "OK", "Usuario registrado.");
        }
    });

    connect(rateButton, &QPushButton::clicked, this, [=]() {
        if (userManager->users.isEmpty()) {
            QMessageBox::warning(this, "Error", "Primero registra un usuario.");
            return;
        }

        QStringList userNames = userManager->users.keys();
        QString user = QInputDialog::getItem(this, "Usuario", "Selecciona usuario:", userNames, 0, false);
        if (user.isEmpty()) return;

        QVector<Show> shows = readShowsWithCache(":/front/data/anime.csv", "anime.bin");

        QStringList movieNames;
        QHash<QString, int> movieToId;
        for (const auto &s : shows) {
            movieNames << s.name;
            movieToId[s.name] = s.anime_id;
        }

        QString movie = QInputDialog::getItem(this, "Película", "Selecciona película:", movieNames, 0, false);
        if (movie.isEmpty()) return;

        int animeId = movieToId.value(movie, -1);

        double kaggleSum = 0.0;
        int kaggleCount = 0;
        if (kaggleRatings && animeId != -1) {
            for (auto it = kaggleRatings->cbegin(); it != kaggleRatings->cend(); ++it) {
                const auto &userMap = it.value();
                auto it2 = userMap.find(animeId);
                if (it2 != userMap.end()) {
                    kaggleSum += it2.value();
                    ++kaggleCount;
                }
            }
        }
        double localSum = 0.0;
        int localCount = 0;
        for (auto it = userManager->users.cbegin(); it != userManager->users.cend(); ++it) {
            const User &u = it.value();
            auto it2 = u.ratings.find(movie);
            if (it2 != u.ratings.end()) {
                localSum += it2.value();
                ++localCount;
            }
        }

        int rating = QInputDialog::getInt(this, "Calificar", "Tu calificación (1–10):", 5, 1, 10);
        double totalSum = kaggleSum + localSum + rating;
        int totalCount = kaggleCount + localCount + 1;
        double avg = totalCount ? (totalSum / totalCount) : 0.0;

        QMessageBox::information(this, "Promedio", "Promedio actual (incluyendo tu calificación): " + QString::number(avg));

        userManager->rateMovie(user, movie, rating);

        QMessageBox::information(this, "OK", "Calificación guardada.");
    });
}
