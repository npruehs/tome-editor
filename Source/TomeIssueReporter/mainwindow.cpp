#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QUrl>

#include "config.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup network access.
    this->networkAccessManager = new QNetworkAccessManager(this);

    connect(this->networkAccessManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(onNetworkReply(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete this->networkAccessManager;

    delete this->ui;
}

void MainWindow::on_buttonBox_accepted()
{
    // Get data.
    const QString& title = this->ui->lineEditTitle->text();
    QString description = this->ui->plainTextEditDescription->toPlainText();
    const bool includeLog = this->ui->checkBoxIncludeLog->isChecked();

    // Add log contents.
    if (includeLog)
    {
        QFile logFile(LOG_PATH);

        if (logFile.exists() && logFile.open(QIODevice::ReadOnly))
        {
            description += "\r\n";
            description += "\r\n";
            description += "output.log:\r\n";
            description += "\r\n";
            description += "```";
            description += logFile.readAll();
            description += "```";
        }
    }

    qInfo(QString("Creating GitHub issue %1.").arg(title).toUtf8().constData());

    // Build request JSON.
    QJsonObject json;
    json["title"] = title;
    json["body"] = description;

    QJsonDocument jsonDocument(json);
    QByteArray jsonBytes = jsonDocument.toJson();

    // Create request.
    QUrl serviceURL("https://api.github.com/repos/npruehs/tome-editor/issues");
    QNetworkRequest request(serviceURL);

    QByteArray authBytes;
    authBytes.append(GITHUB_USER);
    authBytes.append(":");
    authBytes.append(GITHUB_PASSWORD);
    QByteArray authBytesBase64 = authBytes.toBase64();
    QString auth = "Basic " + QString(authBytesBase64);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setHeader(QNetworkRequest::ContentLengthHeader, jsonBytes.size());
    request.setRawHeader(QString("Accept").toUtf8(), QString("application/vnd.github.v3+json").toUtf8());
    request.setRawHeader(QString("Authorization").toUtf8(), auth.toUtf8());

    this->networkAccessManager->post(request, jsonBytes);
}

void MainWindow::on_buttonBox_rejected()
{
    this->close();
}

void MainWindow::onNetworkReply(QNetworkReply* reply)
{
    Q_UNUSED(reply)

    this->close();
}
