#include "Core/mainwindow.h"
#include "Core/controller.h"
#include "Features/Export/Controller/exportcontroller.h"
#include "Features/Export/Model/recordexporttemplate.h"
#include "Util/pathutils.h"


#include <QApplication>


bool handleCommandLineMode(int argc, char *argv[], int &exitCode)
{
    bool handled = false;

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
                        exitCode = -1;
                    }
                }
            }
            handled = true;
        }
    }
    return handled;
}

int main(int argc, char *argv[])
{
    int exitCode = 0;

    QApplication a(argc, argv);

    // Set application data.
    a.setApplicationVersion(APP_VERSION);
    a.setOrganizationName("Tome");
    a.setOrganizationDomain("tome-editor.org");
    a.setApplicationName("Tome");


    // Check command line mode first.
    if ( !handleCommandLineMode(argc, argv, exitCode) )
    {
        MainWindow w;
        w.show();
        exitCode = a.exec();
    }
    return exitCode;
}
