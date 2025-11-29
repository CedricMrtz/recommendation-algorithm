#pragma once
#include "user.h"
#include <QHash>
#include <QString>

class UserManager {
public:
    UserManager();

    bool registerUser(const QString& name);
    bool rateMovie(const QString& user, const QString& movie, int rating);

    QHash<QString, int> getUserRatings(const QString& user) const;
    QHash<QString, User> users;

private:
    void loadUsers();
    void loadUserRatings();
    void rewriteUserRatingsFile();
};
