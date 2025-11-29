#pragma once
#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include "userManager.h"
#include "ratingutils.h" 
#include "RecommendationEngine.h"
#include "recommendShow.h"

class navBar : public QWidget {
    Q_OBJECT
public:
    explicit navBar(UserManager *manager,
                    KaggleRatings *kaggleRatings,
                    QWidget *parent = nullptr);
    UserManager *userManager;
    KaggleRatings *kaggleRatings;

signals:
    void refreshRecommendationsRequested(const QString &userName);

};
