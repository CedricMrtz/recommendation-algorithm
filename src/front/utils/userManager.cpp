#include "userManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

UserManager::UserManager() {
    loadUsers();
    loadUserRatings();
}

bool UserManager::registerUser(const QString& name) {
    if (users.contains(name))
        return false;

    users[name] = User{name};

    QFile file(":/front/data/users.csv");
    if (file.open(QIODevice::Append | QIODevice::Text)) {
        QTextStream out(&file);
        out << name << "\n";
    } else {
        qWarning() << "Could not open users.csv for append:" << file.errorString();
    }

    return true;
}

bool UserManager::rateMovie(const QString& user, const QString& movie, int rating) {
    if (!users.contains(user))
        return false;

    bool existed = users[user].ratings.contains(movie);
    users[user].ratings[movie] = rating;

    if (!existed) {
        QFile file(":/front/data/user_ratings.csv");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << user << "," << movie << "," << rating << "\n";
        } else {
            qWarning() << "No se pudo abrir user_ratings.csv (añadir):" << file.errorString();
        }
    } else {
        rewriteUserRatingsFile();
    }

    return true;
}

void UserManager::loadUsers() {
    QFile file(":/front/data/users.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString name = in.readLine().trimmed();
        if (!name.isEmpty()) {
            users[name] = User{name};
        }
    }
}

void UserManager::loadUserRatings() {
    QFile file(":/front/data/user_ratings.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        auto parts = line.split(",");
        if (parts.size() != 3) continue;

        QString username = parts[0];
        QString movie    = parts[1];
        int rating       = parts[2].toInt();

        if (!users.contains(username)) {
            users[username] = User{username};
        }
        users[username].ratings[movie] = rating;
    }
}

void UserManager::rewriteUserRatingsFile() {
    QFile file(":/front/data/user_ratings.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
        qWarning() << "No se pudeo abrir user_ratings.csv: (cambiar)" << file.errorString();
        return;
    }

    QTextStream out(&file);

    for (auto it = users.cbegin(); it != users.cend(); ++it) {
        const QString &username = it.key();
        const User &u = it.value();

        for (auto it2 = u.ratings.cbegin(); it2 != u.ratings.cend(); ++it2) {
            const QString &movie = it2.key();
            int rating = it2.value();
            out << username << "," << movie << "," << rating << "\n";
        }
    }
}

QHash<QString, int> UserManager::getUserRatings(const QString& user) const
{
    auto it = users.find(user);
    if (it == users.end()) {
        return {};
    }
    return it.value().ratings;
}
