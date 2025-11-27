#pragma once
#include <QString>
#include <QHash>

struct User {
    QString name;
    QHash<QString, int> ratings;  
};
