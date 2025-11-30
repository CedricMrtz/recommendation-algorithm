#pragma once
#include <QVector>
#include <QString>
#include "show.h"

QVector<Show> readShowsFromCsv(const QString &path);
QVector<Show> readShowsWithCache(const QString &csvPath,
                                 const QString &binPath = "anime.bin");
