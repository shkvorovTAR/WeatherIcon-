#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    manager = new QNetworkAccessManager();
    QObject::connect(manager, SIGNAL(finished(QNetworkReply*)),
                     this, SLOT(managerFinished(QNetworkReply*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString url = "http://api.openweathermap.org/data/2.5/weather?id=" + ui->lineEdit->text() + "&units=metric&APPID=27e8b6674f0f44f79e51e6994b0eb8d6";
    qDebug() << url;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void MainWindow::managerFinished(QNetworkReply *reply)
{
    if (reply->error()) {
        qDebug() << reply->errorString();
        return;
    }

    QString answer = reply->readAll();
    QJsonDocument jsonResponse = QJsonDocument::fromJson(answer.toUtf8());
    QString description = jsonResponse.object()["weather"].toArray()[0].toObject()["description"].toString();
    ui->label_3->setText(description);

    QString name = "";
    if(description.contains("clouds")) {
        name = "cloudiness";
    } else if(description.contains("fog")) {
        name = "fog";
    } else if(description.contains("rain")) {
        name = "rain";
    } else if(description.contains("snow")) {
        name = "snow";
    } else if(description.contains("clear")) {
        name = "sun";
    } else if(description.contains("thunderstorm")) {
        name = "thunderstorm";
    }

    if(!name.isEmpty()) {
        QPixmap pixmap(":/images/icons/" + name + ".png");
        ui->label_2->setPixmap(pixmap);
    } else {
        ui->label_2->setPixmap(QPixmap(0, 0));
    }
}
