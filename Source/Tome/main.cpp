#include "Core/mainwindow.h"
#include "Core/controller.h"
#include "Features/Export/Controller/exportcontroller.h"
#include "Features/Export/Model/recordexporttemplate.h"
#include "Util/pathutils.h"


#include <QApplication>

QCoreApplication* createApplication(int argc, char *argv[])
{
    for (int i = 1; i < argc; ++i)
    {
        if (!qstrcmp(argv[i], "-export"))
        {
            // Instantiate a plain QCoreApplication to avoid unnecessarily
            // initializing resources needed for a graphical user interface.
            return new QCoreApplication(argc, argv);
        }
    }

    return new QApplication(argc, argv);
}

int handleCommandLineMode(int argc, char *argv[])
{
    if ( 1 < argc )
    {
        if ( 0 == QString(argv[1]).compare( "-export" ) )
        {
            // cmdline syntax for -export
            // -export [project file path] [export template name] [target file name without extension]
            if ( 5 == argc  )
            {
                // TODO: sytax validation and informative error output
                Tome::Controller controller;
                controller.openProject( argv[2] );

                if ( controller.isProjectLoaded() )
                {
                    // Get export template.
                    QString exportTemplateName = argv[3];
                    const Tome::RecordExportTemplate& exportTemplate =
                            controller.getExportController().getRecordExportTemplate(exportTemplateName);

                    // Build export file path.
                    const QString filePath = QString(argv[4]) + exportTemplate.fileExtension;

                    // Export records.
                    try
                    {
                        controller.getExportController().exportRecords(exportTemplate, filePath);
                    }
                    catch (std::runtime_error&)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    // Start application with command-line support: http://doc.qt.io/qt-5/qapplication.html#details
    QScopedPointer<QCoreApplication> app(createApplication(argc, argv));

    // Set application data.
    app->setApplicationVersion(APP_VERSION);
    app->setOrganizationName("Tome");
    app->setOrganizationDomain("tome-editor.org");
    app->setApplicationName("Tome");

    if (qobject_cast<QApplication *>(app.data()))
    {
        QScopedPointer<Tome::Controller> controller(new Tome::Controller());
        controller->init();
        return app->exec();
    }
    else
    {
       return handleCommandLineMode(argc, argv);
    }
}
