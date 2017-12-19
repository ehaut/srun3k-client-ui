#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QtNetwork/QNetworkAccessManager"
#include "QSystemTrayIcon"
#include "QMenu"
#include "QPushButton"
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
    void Min();
    void on_ABOUT_clicked();

    void on_SERVICE_clicked();

    void Start();

    void GetServerInfo();

    void on_AUTO_START_clicked();

    void on_AUTO_LOGIN_clicked();

    void on_RetryButton_clicked();

//    void GET_ACID_Finished(QNetworkReply*);

    void GET_INFO_Finished(QNetworkReply*);
    void anchorClickedSlot(const QUrl &);
    void TimeSlot();
//    void AutoJumpTimeSlot();
    void on_LogoutButton_clicked();
    void POST_LOGOUT_Finished(QNetworkReply *);
    void on_LoginButton_clicked();
    void POST_LOGIN_Finished(QNetworkReply *);
    void on_Enter_clicked();
    void on_ADVANCED_clicked();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason);
    void on_SERVICE_2_clicked();
    void createActions();
    void createMenu();
    void on_showMainAction();
    void on_exitAppAction();
    void on_aboutAppAction();
    void on_advanced_save_clicked();

    void on_setdefaults_clicked();

    void on_Enter_2_clicked();

    void on_advanced_back_clicked();


    void on_ShowServerMessage_clicked();

private:
    Ui::MainWindow *ui;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu;
      QAction *mShowMainAction;
      QAction *mExitAppAction;
      QAction *mServiceAction;
      QAction *mAboutAction;
      QPushButton *AboutButton;
      QPushButton *AdvancedButton;
//    QNetworkAccessManager *manager;
    QTimer *meTimer;
//    QTimer *AutoJumpTimer;
//     QTimer *RetryTimer;
//     QTimer *WaitTimer;
};

#endif // MAINWINDOW_H
