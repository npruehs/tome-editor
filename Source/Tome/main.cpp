#include "Core/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Set application data.
    a.setApplicationVersion(APP_VERSION);
    a.setOrganizationName("Tome");
    a.setOrganizationDomain("tome-editor.org");
    a.setApplicationName("Tome");

    MainWindow w;
    w.show();

    return a.exec();
}
