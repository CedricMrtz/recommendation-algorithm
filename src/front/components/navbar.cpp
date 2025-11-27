    #include "navbar.h"
    #include "csvutils.h"
    #include <QVector>
    #include <QStringList>
    #include <QInputDialog>
    #include <QMessageBox>


    navBar::navBar(UserManager *manager, QWidget *parent)
        : QWidget(parent), userManager(manager) 
    {
        auto *layout = new QHBoxLayout(this);

        auto *addButton = new QPushButton(this);
        addButton->setIcon(QIcon(":/front/public/add.svg"));

        auto *profileButton = new QPushButton(this);
        profileButton->setIcon(QIcon(":/front/public/profile.svg"));

        auto *searchButton = new QPushButton(this);
        searchButton->setIcon(QIcon(":/front/public/search.svg"));

        auto *rateButton = new QPushButton(this);
        rateButton->setIcon(QIcon(":/front/public/rating.svg"));

        layout->addWidget(addButton);
        layout->addWidget(profileButton);
        layout->addWidget(searchButton);
        layout->addWidget(rateButton);

        setLayout(layout);

        connect(profileButton, &QPushButton::clicked, this, [=]() {
            QString name = QInputDialog::getText(this, "Registrar usuario",
                                                "Nombre del usuario:");

            if (name.isEmpty()) return;

            if (!userManager->registerUser(name)) {
                QMessageBox::warning(this, "Error",
                                    "Ese nombre ya existe.");
            } else {
                QMessageBox::information(this, "OK",
                                        "Usuario registrado.");
            }
        });

        connect(rateButton, &QPushButton::clicked, this, [=]() {
            if (userManager->users.isEmpty()) {
                QMessageBox::warning(this, "Error",
                                    "Primero registra un usuario.");
                return;
            }

            QStringList userNames = userManager->users.keys();
            QString user = QInputDialog::getItem(this, "Usuario",
                                                "Selecciona usuario:",
                                                userNames, 0, false);
            if (user.isEmpty()) return;

            QVector<Show> shows = readShowsFromCsv(":/front/data/anime.csv");
            QStringList movieNames;
            for (auto &s : shows) movieNames << s.name;

            QString movie = QInputDialog::getItem(this, "Película",
                                                "Selecciona película:",
                                                movieNames, 0, false);
            if (movie.isEmpty()) return;

            double total = 0;
            int count = 0;

            for (auto &u : userManager->users) {
                if (u.ratings.contains(movie)) {
                    total += u.ratings[movie];
                    count++;
                }
            }

            double avg = count ? total / count : 0;

            QMessageBox::information(this, "Promedio",
                                    "Promedio actual: " + QString::number(avg));

            int rating = QInputDialog::getInt(this, "Calificar",
                                            "Tu calificación (1–10):",
                                            5, 1, 10);

            userManager->rateMovie(user, movie, rating);

            QMessageBox::information(this, "OK", "Calificación guardada.");
        });
    }
