#pragma once
#include "user.h"
#include <QHash>
#include <QString>
#include <QSet>

class UserManager {
public:
    UserManager();

    bool registerUser(const QString& name);
    bool rateMovie(const QString& user, const QString& movie, int rating);

    QSet<QString> ratedMovies(const QString &username) const {
        QSet<QString> result;
        auto it = users.find(username);
        if (it != users.end()) {
            for (auto r_it = it->ratings.cbegin(); r_it != it->ratings.cend(); ++r_it) {
                result.insert(r_it.key());
            }
        }
        return result;
    }

    QHash<QString, int> getUserRatings(const QString& user) const;
    QHash<QString, User> users;

private:
    void loadUsers();
    void loadUserRatings();
    void rewriteUserRatingsFile();
};
