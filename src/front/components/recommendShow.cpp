#include "recommendShow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <QGridLayout>
#include <QPushButton>
#include <QComboBox>
#include <QSet>
#include <QDebug>

#include "showCard.h"
#include "recommendation_helper.h"

static void clearGridLayout(QGridLayout *layout) {
    if (!layout) return;

    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (QWidget *w = item->widget()) {
            w->deleteLater();
        }
        delete item;
    }
}

recommendShow::recommendShow(const QString &userName, UserManager *userManager, KaggleRatings *kaggleRatings,
                             const QVector<Show> &shows,
                             QWidget *parent): QWidget(parent),m_userName(userName), m_userManager(userManager),
      m_kaggleRatings(kaggleRatings),
      m_shows(shows)
{
    setWindowTitle("Recomendaciones");
    setStyleSheet("background-color: #121212; color: white;");

    auto *mainLayout = new QVBoxLayout(this);

    auto *header = new QHBoxLayout();
    auto *backButton = new QPushButton("Regresar", this);
    backButton->setStyleSheet(
        "background-color: #c0392b; "
        "color: white; "
        "font-weight: bold; "
        "padding: 8px 16px; "
        "border-radius: 6px;"
    );

    auto *title = new QLabel(QString("Recomendaciones para %1").arg(userName), this);
    title->setStyleSheet("font-size: 22px; font-weight: bold;");

    auto *genreLabel = new QLabel("Género:", this);

    genreBox = new QComboBox(this);
    genreBox->addItem("Todos");

    QSet<QString> genreSet;
    for (const Show &s : m_shows) {
        const auto parts = s.genre.split(',', Qt::SkipEmptyParts);
        for (const QString &g : parts) {
            genreSet.insert(g.trimmed());
        }
    }
    QStringList genreList = QStringList(genreSet.begin(), genreSet.end());
    genreList.sort(Qt::CaseInsensitive);
    for (const QString &g : genreList) {
        genreBox->addItem(g);
    }

    header->addWidget(backButton);
    header->addStretch();
    header->addWidget(title);
    header->addStretch();
    header->addWidget(genreLabel);
    header->addWidget(genreBox);

    mainLayout->addLayout(header);

    auto *scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);

    auto *container = new QWidget();
    grid = new QGridLayout(container);
    grid->setAlignment(Qt::AlignTop);
    grid->setHorizontalSpacing(20);
    grid->setVerticalSpacing(25);

    scroll->setWidget(container);
    mainLayout->addWidget(scroll);
    connect(backButton, &QPushButton::clicked, this, &QWidget::close);
    connect(genreBox, &QComboBox::currentTextChanged,
            this, [this](const QString &) {
        refillRecommendations();
    });

    refillRecommendations();
}

void recommendShow::refillRecommendations()
{
    clearGridLayout(grid);

    if (!m_userManager || !m_kaggleRatings) {
        qWarning() << "recommendShow: UserManager o KaggleRatings nulo";
        return;
    }

    QVector<Show> recommended = createRecommendations(
        m_userName,
        *m_userManager,
        *m_kaggleRatings,
        m_shows
    );

    QString selectedGenre = genreBox->currentText();
    qDebug() << "Mostrando recomendaciones para usuario" << m_userName
             << "con género filtrado:" << selectedGenre;

    int row = 0, col = 0;
    for (const Show &s : recommended) {
        if (selectedGenre != "Todos" &&
            !s.genre.contains(selectedGenre, Qt::CaseInsensitive))
            continue;

        qDebug() << "Anime:" << s.anime_id << "," << s.name << ", géneros:" << s.genre;

        auto *card = new ShowCard(s);
        grid->addWidget(card, row, col);
        if (++col == 3) {
            col = 0;
            ++row;
        }
    }
}

int partition(QVector<Show> &arr, int low, int high) {
    double pivot = arr[high].rating;
    int i = low - 1;

    for (int j = low; j < high; j++) {
        if (arr[j].rating > pivot) {
            i++;
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return i + 1;
}

void quickSort(QVector<Show> &arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void recommendShow::sortRecommendations(QVector<Show> &recs) {
    if (recs.size() > 1)
        quickSort(recs, 0, recs.size() - 1);
}

void recommendShow::setRecommendations(const QVector<Show> &recs, const QSet<QString> &ratedMovies) {
    int row = 0, col = 0;
    for (const Show &s : recs) {
        if (ratedMovies.contains(s.name)) continue;
        ShowCard *card = new ShowCard(s);
        grid->addWidget(card, row, col);
        if (++col == 3) {
            col = 0;
            ++row;
        }
    }
}
