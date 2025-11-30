#include "userManager.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QDataStream>

UserManager::UserManager()
    : m_usersBinPath("users.bin"),
      m_ratingsBinPath("user_ratings.bin")
{
    loadUsersAndRatings();
}

void UserManager::loadUsersAndRatings()
{
    users.clear();

    // 1) Cargar usuarios
    {
        QFile file(m_usersBinPath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_6_10);

            qint32 userCount = 0;
            in >> userCount;

            for (qint32 i = 0; i < userCount; ++i) {
                QString username;
                in >> username;
                users[username] = User{username};
            }
        } else {
            qDebug() << "No se pudo abrir" << m_usersBinPath << "para leer (iniciando sin usuarios)";
        }
    }

    // 2) Cargar ratings
    {
        QFile file(m_ratingsBinPath);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream in(&file);
            in.setVersion(QDataStream::Qt_6_10);

            qint32 userCount = 0;
            in >> userCount;

            for (qint32 i = 0; i < userCount; ++i) {
                QString username;
                in >> username;

                qint32 ratingCount = 0;
                in >> ratingCount;

                if (!users.contains(username)) {
                    users[username] = User{username};
                }

                User &u = users[username];
                for (qint32 j = 0; j < ratingCount; ++j) {
                    QString movie;
                    qint32 rating;
                    in >> movie >> rating;
                    u.ratings[movie] = rating;
                }
            }
        } else {
            qDebug() << "No se pudo abrir" << m_ratingsBinPath << "para leer (iniciando sin ratings)";
        }
    }
}

void UserManager::saveUsersToBinary() const
{
    QFile file(m_usersBinPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "No se pudo abrir" << m_usersBinPath << "para escribir:"
                   << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    qint32 userCount = users.size();
    out << userCount;

    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        const QString &username = it.key();
        out << username;
    }
}

void UserManager::saveRatingsToBinary() const
{
    QFile file(m_ratingsBinPath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        qWarning() << "No se pudo abrir" << m_ratingsBinPath << "para escribir:"
                   << file.errorString();
        return;
    }

    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_6_10);

    qint32 userCount = users.size();
    out << userCount;

    for (auto it = users.constBegin(); it != users.constEnd(); ++it) {
        const QString &username = it.key();
        const User &u = it.value();

        out << username;

        qint32 ratingCount = u.ratings.size();
        out << ratingCount;

        for (auto rit = u.ratings.constBegin(); rit != u.ratings.constEnd(); ++rit) {
            const QString &movie = rit.key();
            qint32 rating = rit.value();
            out << movie << rating;
        }
    }
}

bool UserManager::registerUser(const QString& name)
{
    if (users.contains(name))
        return false;

    users[name] = User{name};

    saveUsersToBinary();
    saveRatingsToBinary();

    return true;
}

bool UserManager::rateMovie(const QString& user, const QString& movie, int rating)
{
    if (!users.contains(user))
        return false;

    users[user].ratings[movie] = rating;

    saveUsersToBinary();
    saveRatingsToBinary();

    return true;
}

QHash<QString, int> UserManager::getUserRatings(const QString& user) const
{
    auto it = users.find(user);
    if (it == users.end()) {
        return {};
    }
    return it.value().ratings;
}
