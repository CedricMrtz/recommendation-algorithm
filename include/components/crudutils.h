#pragma once
#include <QHash>
#include <QSet>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDialog>
#include "show.h"

class crudUtils : public QDialog {
    Q_OBJECT
public:
    explicit crudUtils(QWidget *parent = nullptr);

    void deleteMovie(QHash<QString, Show> &movies, QSet<QString> &ratedMovies);
    void addMovie(QHash<QString, Show> &movies);
    void updateMovie(QHash<QString, Show> &movies);
};
