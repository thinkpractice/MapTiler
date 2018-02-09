#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItem>
#include <QGeoAddress>
#include <QQmlContext>
#include <iostream>

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    _geoServiceProvider(NULL)
{
    ui->setupUi(this);
    _mapCenter = QGeoCoordinate(50.0, 5.0);
    setGeoServiceProvider(new QGeoServiceProvider("osm"));

    _geoServiceComboBox = findChild<QComboBox*>("geoServiceComboBox");
    connect(_geoServiceComboBox, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(comboBoxIndexChanged(const QString&)));
    _geoServiceComboBox->setModel(createServiceProviderList());

    _addressField = findChild<QLineEdit*>("addressField");
    _geoQueryResultsView = findChild<QListView*>("geoQueryResultsView");
    connect(_geoQueryResultsView, SIGNAL(clicked(QModelIndex)), this, SLOT(locationClicked(QModelIndex)));

    QPushButton* searchButton = findChild<QPushButton*>("searchButton");
    connect(searchButton, SIGNAL(clicked(bool)), this, SLOT(searchButtonClicked(bool)));

    QQuickWidget* mapWidget = findChild<QQuickWidget*>("mapView");
    mapWidget->rootContext()->setContextProperty("mainWindow", this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _geoServiceProvider;
}

QGeoCoordinate MainWindow::mapCenter() const
{
    return _mapCenter;
}

void MainWindow::setMapCenter(const QGeoCoordinate &coordinate)
{
    if (_mapCenter != coordinate)
    {
        _mapCenter = coordinate;
        emit mapCenterChanged();
    }
}

void MainWindow::setGeoServiceProvider(QGeoServiceProvider* serviceProvider)
{
    if (_geoServiceProvider)
        delete _geoServiceProvider;
    _geoServiceProvider = serviceProvider;
    _geoCodingManager = _geoServiceProvider->geocodingManager();
    connect(_geoCodingManager, SIGNAL(error(QGeoCodeReply*,QGeoCodeReply::Error,QString)), this, SLOT(error(QGeoCodeReply*,QGeoCodeReply::Error,QString)));
    connect(_geoCodingManager, SIGNAL(finished(QGeoCodeReply*)), this, SLOT(geoLocationFound(QGeoCodeReply*)));
}

void MainWindow::locationClicked(const QModelIndex &index)
{
    QStandardItemModel* model = (QStandardItemModel*)_geoQueryResultsView->model();
    QStandardItem* locationItem = model->itemFromIndex(index);
    QVariant data = locationItem->data();
    QPoint location = data.toPoint();
    setMapCenter(QGeoCoordinate(location.x(), location.y()));
}

QStandardItemModel* MainWindow::createServiceProviderList()
{
    QStandardItemModel* serviceProviderModel = new QStandardItemModel();
    QStringList qGeoSrvList = QGeoServiceProvider::availableServiceProviders();
    for (QString entry : qGeoSrvList)
    {
        QGeoServiceProvider provider(entry);
        if (provider.geocodingFeatures().testFlag(QGeoServiceProvider::NoGeocodingFeatures))
            continue;
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
    QStandardItemModel* geoQueryResultsModel = new QStandardItemModel();
    cout << reply->locations().size() << "error=" << reply->errorString().toStdString() << endl;
    for (auto location : reply->locations())
    {
        QString locationString = QString("%1 - (%2, %3)").arg(location.address().text()).arg(location.coordinate().latitude()).arg(location.coordinate().longitude());
        QStandardItem* queryResult = new QStandardItem(locationString);

        QGeoCoordinate coordinate = location.coordinate();
        QPoint p (coordinate.latitude(), coordinate.longitude());
        QVariant data = p;
        queryResult->setData(data);

        geoQueryResultsModel->appendRow(queryResult);
    }
    _geoQueryResultsView->setModel(geoQueryResultsModel);
}

void MainWindow::error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString)
{
    cout << "error=" << errorString.toStdString() << endl;
}

void MainWindow::comboBoxIndexChanged(const QString &text)
{
    setGeoServiceProvider(new QGeoServiceProvider(text));
}
