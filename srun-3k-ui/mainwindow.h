#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QPushButton>
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QPainter>
#include <QPixmap>
#include <QDesktopServices>
#include <QTimer>
#include <QSettings>
#include "network.h"
#include "version.h"
#include "storage.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QStringList serverConfig;
    QStringList userConfig;

protected:
    Ui::MainWindow *ui;
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private slots:
    void on_showMainAction();
    void Close();
    void Min();
    void on_activatedSysTrayIcon(QSystemTrayIcon::ActivationReason reason);
    void on_checkUpdateButton_clicked();
    void anchorClickedSlot(const QUrl &url);
    void on_setDefaultButton_clicked();
    void on_saveButtonInadvanceSettingsPage_clicked();
    void getServerInfo();
    void getUserInfo();
    void showMessage();
    void on_ABOUT_clicked();
    void on_ADVANCED_clicked();
    void on_ENTER_clicked();
    void TimeSlot();
    void on_RetryButton_clicked();
    void on_SERVICE_clicked();
    void on_mABOUT_clicked();
    void on_logoutButton_clicked();

    void on_autoStartCheckBox_clicked();

    void on_autoLoginCheckBox_clicked();

    void on_loginButton_clicked();

private:
    bool mDrag;
    bool isUserinfoExists;
    bool isServerinfoExist;
    QPoint mDragPos;
    QRect mLocation;
    QFont mIconFont;
    QPushButton *closeButton;
    QPushButton *minButton;
    QSystemTrayIcon *mSysTrayIcon;
    QMenu *mMenu=nullptr;
    QAction *mShowMainAction;
    QAction *mExitAppAction;
    QAction *mServiceAction;
    QAction *mAboutAction;
    QPushButton *AboutButton;
    QPushButton *AdvancedButton;
    network *n;
    storage *s;
    int usedtime;
    bool isOnline=false;
    int status;
    QTimer *meTimer=nullptr;
    QString logoutname;
    bool set;
};

#endif // MAINWINDOW_H
