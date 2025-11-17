#include "showCard.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

ShowCard::ShowCard(const Show &show, QWidget *parent) : QWidget(parent) {
    this->setProperty("class", "ShowCard");

    setStyleSheet(R"(
        *[class="ShowCard"] {
            background: #1e1e1e;
            border-radius: 10px;
            padding: 14px;
            border: 1px solid #ffffffff;
        }
        *[class="ShowCard"] QLabel {
            color: #eeeeee;
            font-weight:bold;
            font-size: 14px;
        }
    )");
    auto *layout = new QVBoxLayout(this);
    layout->setSpacing(6);
    setMinimumWidth(220);
    setMinimumHeight(600);
    setMaximumHeight(1000);

    QString text;
    text += show.name + "\n\n";
    text += "Genres: " + show.genre + "\n";
    text += "Type: " + show.type + "\n";
    text += "Episodes: " + QString::number(show.episodes) + "\n";
    text += "Rating: " + QString::number(show.rating) + "\n";
    text += "Members: " + QString::number(show.members) + "\n";

    QLabel *content = new QLabel(text);
    content->setTextFormat(Qt::PlainText);
    content->setStyleSheet("font-size: 14px;");
    content->setWordWrap(true);

    layout->addWidget(content);
}
