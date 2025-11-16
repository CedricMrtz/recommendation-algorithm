#include "navbar.h"

navBar::navBar(QWidget *parent) : QWidget(parent) {
    auto *layout = new QHBoxLayout(this);

    auto *addButton = new QPushButton(this);
    addButton->setIcon(QIcon(":/front/public/add.svg"));

    auto *profileButton = new QPushButton(this);
    profileButton->setIcon(QIcon(":/front/public/profile.svg"));

    auto *searchButton = new QPushButton(this);
    searchButton->setIcon(QIcon(":/front/public/search.svg"));

    layout->addWidget(addButton);
    layout->addWidget(profileButton);
    layout->addWidget(searchButton);

    setLayout(layout);
}