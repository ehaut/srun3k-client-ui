#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNetwork/QNetworkAccessManager"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void GET_INFO_Finished(QNetworkReply *);

    void GET_MESSAGE_Finished(QNetworkReply *);

    void GET_ACID_Finished(QNetworkReply *);

    void TimeSlot();

    void on_AUTO_START_clicked();

    void on_AUTO_LOGIN_clicked();

    void on_SERVICE_clicked();

    void on_GET_MESSAGE_clicked();

    void on_LOGOUT_clicked();

    void POST_LOGOUT_Finished(QNetworkReply *);

    void POST_LOGIN_Finished(QNetworkReply *);

    void on_LOGIN_clicked();

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QTimer *meTimer;
};

#endif // MAINWINDOW_H
