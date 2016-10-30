#include "filemessagehandler.h"

#include <QTextStream>

using namespace Tome;


bool FileMessageHandler::initialized = false;
QFile FileMessageHandler::logFile("output.log");


bool FileMessageHandler::init()
{
    initialized = logFile.open(QIODevice::ReadWrite | QIODevice::Truncate);
    return initialized;
}

void FileMessageHandler::handleMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (!initialized)
    {
        return;
    }

    QTextStream stream(&logFile);

    switch (type)
    {
        case QtDebugMsg:
            stream << "[DEBUG] ";
            break;
        case QtInfoMsg:
            stream << "[INFO] ";
            break;
        case QtWarningMsg:
            stream << "[WARNING] ";
            break;
        case QtCriticalMsg:
            stream << "[CRITICAL] ";
            break;
        case QtFatalMsg:
            stream << "[FATAL] ";
            break;
    }

    stream << context.file << ":" << context.line << " - " << msg << endl;
}
