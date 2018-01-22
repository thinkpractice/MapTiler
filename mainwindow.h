#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QListView>
#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoCodingManager>
#include <QtLocation/QGeoCodeReply>
#include <QStandardItemModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void searchButtonClicked(bool checked);
    void geoLocationFound(QGeoCodeReply* reply);
    void error(QGeoCodeReply *reply, QGeoCodeReply::Error error, QString errorString = QString());
    void comboBoxIndexChanged(const QString &text);

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
};

#endif // MAINWINDOW_H
