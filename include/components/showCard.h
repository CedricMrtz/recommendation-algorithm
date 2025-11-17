#pragma once
#include <QWidget>
#include "show.h"
class ShowCard : public QWidget {
    Q_OBJECT
public:
    explicit ShowCard(const Show &show, QWidget *parent = nullptr);
};
