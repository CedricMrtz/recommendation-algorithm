#include "crudutils.h"
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QDialogButtonBox>
#include <QInputDialog>

crudUtils::crudUtils(QHash<QString, Show> &movies, QSet<QString> &ratedMovies, QWidget *parent) : QDialog(parent), movies(movies), ratedMovies(ratedMovies) {
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

    connect(btnAdd, &QPushButton::clicked, this, &crudUtils::addMovie);
    connect(btnDelete, &QPushButton::clicked, this, &crudUtils::deleteMovie);
    connect(btnUpdate, &QPushButton::clicked, this, &crudUtils::updateMovie);

    connect(btnClose, &QPushButton::clicked, this, &QDialog::accept);
}

bool crudUtils::showMovieDialog(Show &movie, bool isUpdate) {
    QDialog dialog(this);
    dialog.setWindowTitle(isUpdate ? "Update Movie" : "Add Movie");

    QFormLayout *form = new QFormLayout(&dialog);

    // anime_id (readonly)
    QLineEdit *idEdit = new QLineEdit(QString::number(movie.anime_id));
    idEdit->setReadOnly(true);
    form->addRow("Anime ID:", idEdit);

    QLineEdit *nameEdit = new QLineEdit(movie.name);
    form->addRow("Name:", nameEdit);

    QLineEdit *genreEdit = new QLineEdit(movie.genre);
    form->addRow("Genre:", genreEdit);

    QLineEdit *typeEdit = new QLineEdit(movie.type);
    form->addRow("Type:", typeEdit);

    QSpinBox *episodesEdit = new QSpinBox();
    episodesEdit->setMaximum(10000);
    episodesEdit->setValue(movie.episodes);
    form->addRow("Episodes:", episodesEdit);

    QDoubleSpinBox *ratingEdit = new QDoubleSpinBox();
    ratingEdit->setRange(0, 10);
    ratingEdit->setDecimals(2);
    ratingEdit->setValue(movie.rating);
    form->addRow("Rating:", ratingEdit);

    QSpinBox *membersEdit = new QSpinBox();
    membersEdit->setMaximum(50000000);
    membersEdit->setValue(movie.members);
    form->addRow("Members:", membersEdit);

    QDialogButtonBox *buttons =
        new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    form->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() == QDialog::Rejected)
        return false;

    // Write changes back into movie struct
    movie.name = nameEdit->text();
    movie.genre = genreEdit->text();
    movie.type = typeEdit->text();
    movie.episodes = episodesEdit->value();
    movie.rating = ratingEdit->value();
    movie.members = membersEdit->value();

    return true;
}

void crudUtils::deleteMovie() {
    QString name = QInputDialog::getText(this, "Delete Movie", "Enter Anime name:");

    if (name.isEmpty())
        return;

    // Search by name, not key
    QString keyToRemove;
    for (auto it = movies.begin(); it != movies.end(); ++it) {
        if (it.value().name.compare(name, Qt::CaseInsensitive) == 0) {
            keyToRemove = it.key();
            break;
        }
    }

    if (keyToRemove.isEmpty()) {
        QMessageBox::warning(this, "Error", "Movie not found");
        return;
    }

    if (ratedMovies.contains(keyToRemove)) {
        QMessageBox::warning(
            this,
            "Acción no permitida",
            "No puedes eliminar este show porque tiene calificaciones asignadas."
        );
        return;
    }

    movies.remove(keyToRemove);

    QMessageBox::information(this, "Success", "Movie deleted!");
    emit moviesUpdated();
}

void crudUtils::addMovie() {
    int maxId = 0;
    for (const auto &show : movies)
        if (show.anime_id > maxId)
            maxId = show.anime_id;

    Show newShow;
    newShow.anime_id = maxId + 1;

    if (!showMovieDialog(newShow, false))
        return;

    movies.insert(QString::number(newShow.anime_id), newShow);

    QMessageBox::information(this, "Success", "Movie added!");

    emit moviesUpdated();
}

void crudUtils::updateMovie() {
    QString name = QInputDialog::getText(this, "Update Movie", "Enter Anime name:");

    if (name.isEmpty())
        return;

    // Find key by matching movie.name
    QString keyToEdit;
    for (auto it = movies.begin(); it != movies.end(); ++it) {
        if (it.value().name.compare(name, Qt::CaseInsensitive) == 0) {
            keyToEdit = it.key();
            break;
        }
    }

    if (keyToEdit.isEmpty()) {
        QMessageBox::warning(this, "Error", "Movie not found");
        return;
    }

    Show updated = movies[keyToEdit];

    if (!showMovieDialog(updated, true))
        return;

    movies[keyToEdit] = updated;

    QMessageBox::information(this, "Success", "Movie updated!");
    emit moviesUpdated();
}