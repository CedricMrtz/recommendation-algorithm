#pragma once
#include "user.h"
#include <QHash>
#include <QString>
#include <QFile>
#include <QTextStream>

class UserManager {
public:
    QHash<QString, User> users;

    UserManager() {
        loadUsers();
        loadRatings();
    }
    bool registerUser(const QString& name) {
        if (users.contains(name)) return false;

        users[name] = User{name};

        QFile file(":/front/data/users.csv");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << name << "\n";
        }

        return true;
    }

    bool rateMovie(const QString& user, const QString& movie, int rating) {
        if (!users.contains(user)) return false;

        users[user].ratings[movie] = rating;

        QFile file(":/front/data/user_ratings.csv");
        if (file.open(QIODevice::Append | QIODevice::Text)) {
            QTextStream out(&file);
            out << user << "," << movie << "," << rating << "\n";
        }

        return true;
    }


private:

    void loadUsers() {
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

    void loadRatings() {
        QFile file(":/front/data/rating.csv");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return;

        QTextStream in(&file);
        while (!in.atEnd()) {
            QString line = in.readLine().trimmed();
            if (line.isEmpty()) continue;

            auto parts = line.split(",");
            if (parts.size() != 3) continue;

            QString username = parts[0];
            QString movie = parts[1];
            int rating = parts[2].toInt();

            users[username].ratings[movie] = rating;
        }
    }
};
