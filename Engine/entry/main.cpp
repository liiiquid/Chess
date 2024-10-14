#include <QWidget>
#include <QApplication>
#include "serverentry.h"
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //ServerEntry server;
    QWidget w;
    w.resize(100, 100);
    w.show();

    return app.exec();
}
