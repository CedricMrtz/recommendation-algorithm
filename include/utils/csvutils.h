#pragma once
#include <QVector>
#include <QString>
#include "show.h"
#include <QHash>

QHash<QString, Show> readShowsFromCsv(const QString &path);