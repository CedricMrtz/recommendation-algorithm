#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>
#include "userManager.h"

class navBar : public QWidget {
    Q_OBJECT
public:
    explicit navBar(UserManager* manager, QWidget *parent = nullptr);

private:
    UserManager* userManager;
};
