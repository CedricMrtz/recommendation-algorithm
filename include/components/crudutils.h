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
    explicit crudUtils(QHash<QString, Show> &movies, QSet<QString> &ratedMovies, QWidget *parent = nullptr);
signals:
    void moviesUpdated();
private:
    QHash<QString, Show> &movies;
    QSet<QString> &ratedMovies;

    void deleteMovie();
    void addMovie();
    void updateMovie();
    bool showMovieDialog(Show &show, bool isUpdate);
};
