#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoCodingManager>
#include <QtLocation/QGeoCodeReply>
#include <QtPositioning/QGeoCoordinate>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Q_PROPERTY(QGeoCoordinate mapCenter READ mapCenter WRITE setMapCenter NOTIFY mapCenterChanged)

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    QGeoCoordinate mapCenter() const;
    void setMapCenter(const QGeoCoordinate& coordinate);

signals:
    void mapCenterChanged();

public slots:
    void searchButtonClicked(bool checked);
    void geoLocationFound(QGeoCodeReply* reply);
    void error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString = QString());
    void comboBoxIndexChanged(const QString &text);
    void locationClicked(const QModelIndex &index);

private:
    QStandardItemModel* createServiceProviderList();
    void setGeoServiceProvider(QGeoServiceProvider* serviceProvider);

private:
    Ui::MainWindow *ui;
    QLineEdit* _addressField;
    QComboBox* _geoServiceComboBox;
    QGeoServiceProvider* _geoServiceProvider;
    QGeoCodingManager* _geoCodingManager;
    QListView* _geoQueryResultsView;
    QGeoCoordinate _mapCenter;
};

#endif // MAINWINDOW_H
