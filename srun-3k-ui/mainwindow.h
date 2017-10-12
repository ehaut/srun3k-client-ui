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

    void on_ABOUT_clicked();

    void on_SERVICE_clicked();

    void Start();

    void GetServerInfo();

    void on_AUTO_START_clicked();

    void on_AUTO_LOGIN_clicked();

    void on_RetryButton_clicked();

//    void GET_ACID_Finished(QNetworkReply*);

    void GET_INFO_Finished(QNetworkReply*);

    void TimeSlot();
    void on_LogoutButton_clicked();
    void POST_LOGOUT_Finished(QNetworkReply *);
    void on_LoginButton_clicked();
    void POST_LOGIN_Finished(QNetworkReply *);
    void on_Enter_clicked();

    void on_SERVICE_2_clicked();

private:
    Ui::MainWindow *ui;

//    QNetworkAccessManager *manager;
    QTimer *meTimer;
//     QTimer *RetryTimer;
//     QTimer *WaitTimer;
};

#endif // MAINWINDOW_H
