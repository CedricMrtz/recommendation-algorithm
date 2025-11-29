#include "crudutils.h"

crudUtils::crudUtils(QWidget *parent) : QDialog(parent) {
    auto *btnAdd = new QPushButton("Add Movie", this);
    auto *btnDelete = new QPushButton("Delete Movie", this);
    auto *btnUpdate = new QPushButton("Update Movie", this);
    auto *btnClose = new QPushButton("Close", this);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(btnAdd);
    layout->addWidget(btnDelete);
    layout->addWidget(btnUpdate);
    layout->addSpacing(10);
    layout->addWidget(btnClose);

    setLayout(layout);
    setWindowTitle("Movie Manager");

    connect(btnAdd, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Add", "Add Movie clicked (not implemented)");
    });

    connect(btnDelete, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Delete", "Delete Movie clicked (not implemented)");
    });

    connect(btnUpdate, &QPushButton::clicked, [this]() {
        QMessageBox::information(this, "Update", "Update Movie clicked (not implemented)");
    });

    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

void crudUtils::deleteMovie(QHash<QString, Show> &movies, QSet<QString> &ratedMovies) {
    // Implementation for deleting a movie
}

void crudUtils::addMovie(QHash<QString, Show> &movies) {
    // Implementation for adding a movie
}

void crudUtils::updateMovie(QHash<QString, Show> &movies) {
    // Implementation for updating a movie
}