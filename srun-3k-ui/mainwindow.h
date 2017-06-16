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

protected:
    void mousePressEvent(QMouseEvent *event);

private slots:
    void Close();

    void GET_INFO_Finished(QNetworkReply *);

    void on_pushButton_clicked();

    void on_ABOUT_clicked();

    void on_SERVICE_clicked();

     void TimeSlot();

     void GetTryTimeSlot();

     void PostTryTimeSlot();

     void ReLoginTimeSlot();

     void LogoutTimeSlot();

     void on_AUTO_START_clicked();

     void on_AUTO_LOGIN_clicked();

     void POST_LOGOUT_Finished(QNetworkReply *);

     void POST_LOGIN_Finished(QNetworkReply *);

private:
    Ui::MainWindow *ui;
    QNetworkAccessManager *manager;
    QTimer *Timer;
     QTimer *RetryTimer;
     QTimer *WaitTimer;
};

#endif // MAINWINDOW_H
