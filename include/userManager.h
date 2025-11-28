#pragma once
#include "user.h"
#include <QHash>
#include <QString>

class UserManager {
public:
    QHash<QString, User> users;

    UserManager();

    bool registerUser(const QString& name);
    bool rateMovie(const QString& user, const QString& movie, int rating);

private:
    void loadUsers();
    void loadUserRatings();
    void rewriteUserRatingsFile();
};
