#include "Core/commandlineoptions.h"
#include "Core/mainwindow.h"
#include "Core/controller.h"
#include "Features/Export/Controller/exportcontroller.h"
#include "Features/Export/Model/recordexporttemplate.h"
#include "Util/pathutils.h"


#include <QApplication>

QCoreApplication* createApplication(Tome::CommandLineOptions* options)
{
    if (options->noGui)
    {
        // Instantiate a plain QCoreApplication to avoid unnecessarily
        // initializing resources needed for a graphical user interface.
        return new QCoreApplication(options->argc, options->argv);
    }

    return new QApplication(options->argc, options->argv);
}

int main(int argc, char *argv[])
{
    // Parse command line options.
    Tome::CommandLineOptions* options = new Tome::CommandLineOptions();
    options->parse(argc, argv);

    // Start application with command-line support: http://doc.qt.io/qt-5/qapplication.html#details
    QScopedPointer<QCoreApplication> app(createApplication(options));

    // Set application data.
    app->setApplicationVersion(APP_VERSION);
    app->setOrganizationName("Tome");
    app->setOrganizationDomain("tome-editor.org");
    app->setApplicationName("Tome");

    QScopedPointer<Tome::Controller> controller(new Tome::Controller(options));
    int exitCode = controller->start();

    if (exitCode != 0 || options->noGui)
    {
        return exitCode;
    }

    return app->exec();
}
