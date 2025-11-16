#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

#include "./components/navbar.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    navBar navbar;
    navbar.show();

    return app.exec();
}
