#include <QApplication>
#include <QPushButton>
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QPushButton button("Click me!");
    button.resize(200, 60);

    QObject::connect(&button, &QPushButton::clicked, []() {
        QMessageBox::information(nullptr, "Hello", "Hello from Qt!");
    });

    button.show();

    return app.exec();
}
