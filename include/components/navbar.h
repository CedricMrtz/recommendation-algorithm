#pragma once

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGridLayout>
#include <QHash>

#include "userManager.h"
#include "ratingutils.h"
#include "show.h"

class navBar : public QWidget {
    Q_OBJECT
public:
    explicit navBar(UserManager *manager,
                    KaggleRatings *kaggleRatings,
                    QWidget *parent = nullptr);

    void refreshDisplay(const QHash<QString, Show> &movies);

    QHash<QString, Show> movies;  
    QGridLayout *grid = nullptr;  

private:
    UserManager *userManager;
    KaggleRatings *kaggleRatings;
};
