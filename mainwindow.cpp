#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _geoServiceProvider = new QGeoServiceProvider("osm");
    _geoCodingManager = _geoServiceProvider->geocodingManager();
    connect(_geoCodingManager, SIGNAL(error(QGeoCodeReply*,QGeoCodeReply::Error,QString)), this, SLOT(error(QGeoCodeReply*,QGeoCodeReply::Error,QString)));
    connect(_geoCodingManager, SIGNAL(finished(QGeoCodeReply*)), this, SLOT(geoLocationFound(QGeoCodeReply*)));

    _geoServiceComboBox = findChild<QComboBox*>("geoServiceComboBox");
    connect(_geoServiceComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(comboBoxIndexChanged(const QString&)));
    _geoServiceComboBox->setModel(createServiceProviderList());

    _addressField = findChild<QLineEdit*>("addressField");

    QPushButton* searchButton = findChild<QPushButton*>("searchButton");
    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(searchButtonClicked(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _geoServiceProvider;
}

QStandardItemModel* MainWindow::createServiceProviderList()
{
    QStandardItemModel* serviceProviderModel = new QStandardItemModel();
    QStringList qGeoSrvList = QGeoServiceProvider::availableServiceProviders();
    for (QString entry : qGeoSrvList)
    {
        QStandardItem* item = new QStandardItem(entry);
        serviceProviderModel->appendRow(item);
    }
    return serviceProviderModel;
}

void MainWindow::searchButtonClicked(bool checked)
{
    QString address = _addressField->text();
    QGeoCodeReply* reply = _geoCodingManager->geocode(address);
}

void MainWindow::geoLocationFound(QGeoCodeReply* reply)
{
    cout << reply->locations().size() << "error=" << reply->errorString().toStdString() << endl;
    for (auto location : reply->locations())
    {
        cout << location.coordinate().latitude() << "," << location.coordinate().longitude() << endl;
    }
}

void MainWindow::error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString)
{
    cout << "error=" << errorString.toStdString() << endl;
}

void MainWindow::comboBoxIndexChanged(const QString &text)
{
    cout << text.toStdString() << " selected in combobox" << endl;
}
