#include "outputdockwidget.h"

#include "../Controller/messagehandlers.h"

using namespace Tome;


OutputDockWidget* OutputDockWidget::logDockWidget = nullptr;


OutputDockWidget::OutputDockWidget(QWidget* parent) :
    QDockWidget(tr("Output"), parent)
{
    // Create layout and widgets.
    this->widget = new QWidget(this);
    this->verticalLayout = new QVBoxLayout();
    this->textBrowser = new QTextBrowser();

    this->verticalLayout->addWidget(this->textBrowser);
    this->widget->setLayout(this->verticalLayout);
    this->setWidget(this->widget);

    // Install message handler.
    MessageHandlers::addMessageHandler(handleMessage);
}

OutputDockWidget::~OutputDockWidget()
{
    delete this->textBrowser;
    delete this->verticalLayout;
    delete this->widget;
}

void OutputDockWidget::init()
{
    // Set static reference to current window.
    // This is required because QtMessageHandlers must be static functions.
    logDockWidget = this;
}

void OutputDockWidget::handleMessage(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    Q_UNUSED(context)

    if (logDockWidget == nullptr)
    {
        return;
    }

    QString colorString;

    switch (type)
    {
        case QtDebugMsg:
            colorString = "<font color=\"#FF00FF\">";
            break;
        case QtInfoMsg:
            colorString = "<font color=\"#000000\">";
            break;
        case QtWarningMsg:
            colorString = "<font color=\"#FFFF00\">";
            break;
        case QtCriticalMsg:
            colorString = "<font color=\"#FF0000\">";
            break;
        case QtFatalMsg:
            colorString = "<font color=\"#FF0000\">";
            break;
    }

    QString s = QString("%0 %1 </font>").arg(colorString, msg);
    logDockWidget->textBrowser->append(s);
}
