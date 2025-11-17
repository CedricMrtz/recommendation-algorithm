#pragma once
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QPushButton>

class navBar : public QWidget {
    Q_OBJECT
public:
    explicit navBar(QWidget *parent = nullptr);
};
