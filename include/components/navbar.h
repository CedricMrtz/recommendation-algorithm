#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "userManager.h"
#include "ratingutils.h" 
#include "recommendation.h"
#include "recommendShow.h"

class navBar : public QWidget {
    Q_OBJECT
public:
    explicit navBar(UserManager *manager,
                    KaggleRatings *kaggleRatings,
                    QWidget *parent = nullptr);

private:
    UserManager *userManager;
    KaggleRatings *kaggleRatings;
};
